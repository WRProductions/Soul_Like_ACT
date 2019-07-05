// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_TryUseActiveAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SOUL_LIKE_ACT_API UBTT_TryUseActiveAbility : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSubclassOf<USoulActiveAbility> ActiveAbility;

	UFUNCTION(BlueprintCallable)
	void BindOnGameplayAbilityEnded(AActor *SourceActor, TSubclassOf<USoulActiveAbility> InActiveAbility);

	void EndTaskTrigger(class UGameplayAbility* GA);
};
