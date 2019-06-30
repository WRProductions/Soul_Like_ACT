// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_IsTargetAvailable.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UBTS_IsTargetAvailable : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_IsTargetAvailable();

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector IsTargetAliveKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
