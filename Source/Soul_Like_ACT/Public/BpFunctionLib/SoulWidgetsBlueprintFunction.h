// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoulWidgetsBlueprintFunction.generated.h"

class UGameplayAbilitySpec;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulWidgetsBlueprintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		static void ToString_Modifier(const USoulGameplayAbilitySpec* InputGA, FString& ModifierName, FString& ModifierLevel);
};
