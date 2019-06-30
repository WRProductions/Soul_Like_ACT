// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyBTTaskNode_GetStrafeVector.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UMyBTTaskNode_GetStrafeVector : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
		float StrafeRadius_Min = 5.f;
	UPROPERTY(EditAnywhere, Category = Blackboard)
		float StrafeRadius_Max = 15.f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
