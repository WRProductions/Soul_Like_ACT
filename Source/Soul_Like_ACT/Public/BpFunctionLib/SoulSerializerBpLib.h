// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulSerializerBpLib.generated.h"

class USoulGameplayAbility;
class USoulItem;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulSerializerBpLib: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		static void ToString_Modifier(const USoulItem* ItemRef, TArray<FString>& ModifierNames, TArray<FString>& ModifierLevels, bool& Successful);

	static void ToString_Modifier(const TPair<TSubclassOf<USoulGameplayAbility>, int32>& InputAbilityInfo, FString& ModifierName, FString& ModifierLevel);
};
