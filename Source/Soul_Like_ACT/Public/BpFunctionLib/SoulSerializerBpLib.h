// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulGameplayAbility.h"
#include "SoulSerializerBpLib.generated.h"

struct FGameplayAttribute;
class USoulGameplayAbility;
class USoulItem;

/**
 * 
 */
UCLASS()
	class SOUL_LIKE_ACT_API USoulSerializerBpLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void GetModifiersFromItem(const USoulItem* ItemRef, TArray<FText>& ModifierNames,
	                              TArray<FText>& ModifierLevels, bool& Successful);

	UFUNCTION(BlueprintCallable)
	static void GetPrimaryStatusFromItem(const USoulItem* ItemRef, TArray<FText>& ModifierNames,
								TArray<FText>& ModifierLevels, bool& Successful);

	UFUNCTION(BlueprintCallable)
	static void AttributeToString(FGameplayAttribute Attribute, FString& Output);

	static void ModiferToText(const TPair<TSubclassOf<USoulModifierGameplayAbility>, int32>& InputAbilityInfo,
		FText& ModifierName, FText& ModifierLevel);

	template<typename T, typename Y>
	static FORCEINLINE void AddKeysToMap(TArray<T>& InKeys, TMap<T, Y>& InMap)
	{
		for (T InKey : InKeys)
		{
			InMap.FindOrAdd(InKey);
		}
	}

	template<typename T, typename Y, typename U>
	static FORCEINLINE void Map_CopyKeys(TMap<T, Y>& FromMap, TMap<T, U>& ToMap)
	{
		TArray<T> TempKeys;
		FromMap.GetKeys(TempKeys);

		AddKeysToMap(TempKeys, ToMap);
	}
};