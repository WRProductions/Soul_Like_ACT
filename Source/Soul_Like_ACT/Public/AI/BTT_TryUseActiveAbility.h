// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TryUseActiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UBTT_TryUseActiveAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION(BlueprintCallable)
	void BindOnGameplayAbilityEnded(UGameplayAbility *GameplayAbilityInstance);
};
