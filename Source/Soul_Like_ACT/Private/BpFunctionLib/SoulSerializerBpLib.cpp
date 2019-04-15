// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulSerializerBpLib.h"
#include "Item/ItemBasic.h"
#include "Abilities/SoulGameplayAbility.h"

void USoulSerializerBpLib::GetModifiersFromItem(const USoulItem* ItemRef, TArray<FString>& ModifierNames,
                                                TArray<FString>& ModifierLevels, bool& Successful)
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
			ModiferToString(Element, ModifierNames[i], ModifierLevels[i]);

			++i;
		}

		Successful = true;
	}
}

void USoulSerializerBpLib::ModiferToString(const TPair<TSubclassOf<USoulGameplayAbility>, int32>& InputAbilityInfo,
                                           FString& ModifierName, FString& ModifierLevel)
{
	ModifierName = (InputAbilityInfo.Key)->GetName();

	ModifierLevel = FString::FromInt(InputAbilityInfo.Value);
}

void USoulSerializerBpLib::AttributeToString(FGameplayAttribute Attribute, FString& Output)
{
	if (Attribute == USoulAttributeSet::GetHealthAttribute())
		Output = "Health";

	else if (Attribute == USoulAttributeSet::GetStaminaAttribute())
		Output = "Stamina";

	else if (Attribute == USoulAttributeSet::GetLeechAttribute())
		Output = "Leech";

	else if (Attribute == USoulAttributeSet::GetMoveSpeedAttribute())
		Output = "Move Speed";

	else if (Attribute == USoulAttributeSet::GetTenacityAttribute())
		Output = "Tenacity";

	else if (Attribute == USoulAttributeSet::GetAttackPowerAttribute())
		Output = "Attack Power";

	else if (Attribute == USoulAttributeSet::GetDefensePowerAttribute())
		Output = "Defense";

	else if (Attribute == USoulAttributeSet::GetAttackSpeedAttribute())
		Output = "Attack Speed";

	else if (Attribute == USoulAttributeSet::GetCriticalStrikeAttribute())
		Output = "Critical Strike";

	else if (Attribute == USoulAttributeSet::GetCriticalMultiAttribute())
		Output = "Critical Multiplier";
}
