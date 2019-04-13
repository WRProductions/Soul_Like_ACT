// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulSerializerBpLib.h"
#include "Item/ItemBasic.h"
#include "Abilities/SoulGameplayAbility.h"

void USoulSerializerBpLib::ToString_Modifier(const TPair<TSubclassOf<USoulGameplayAbility>, int32>& InputAbilityInfo,
                                             FString& ModifierName, FString& ModifierLevel)
{
	ModifierName = (InputAbilityInfo.Key)->GetName();

	ModifierLevel = FString::FromInt(InputAbilityInfo.Value);
}

void USoulSerializerBpLib::ToString_Modifier(const USoulItem* ItemRef,
	TArray<FString>& ModifierNames, TArray<FString>& ModifierLevels, bool& Successful)
{
	const TMap<TSubclassOf<USoulGameplayAbility>, int>& TargetGA = ItemRef->GrantedAbility;
	int32 GA_Size = TargetGA.Num();
	if (GA_Size <= 0)
	{
		Successful = false;
		return;
	}
	else
	{
		ModifierLevels.SetNum(GA_Size, false);
		ModifierNames.SetNum(GA_Size, false);

		uint8 i = 0;
		for (auto Element : TargetGA)
		{
			ToString_Modifier(Element, ModifierNames[i], ModifierLevels[i]);

			++i;
		}

		Successful = true;
	}
}
