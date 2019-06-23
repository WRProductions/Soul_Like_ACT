// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SoulAbilitySysBPLib.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS(meta = (ScriptName = "AbilitySystemLibrary"))
class SOUL_LIKE_ACT_API USoulAbilitySysBPLib : public UAbilitySystemBlueprintLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Ability, Meta = (Tooltip = "This function can be used to create the event data through weapon on hit"))
	static void CreateEventData(const AActor *Target, const AActor *Source, const FHitResult &InpHitResult, const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData & OutpEventData);

	UFUNCTION(BlueprintCallable)
		static bool GetMontageFromActiveAbility(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, UAnimMontage*& AnimMontage);
	UFUNCTION(BlueprintCallable)
		static float GetMontageSectionLength(UAnimMontage* AnimMontage, FName SectionName);
};
