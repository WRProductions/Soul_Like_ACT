// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTService_PredicateActions.h"
#include "AI/AI_Enums.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_PredicateActions::UMyBTService_PredicateActions()
{
	bCreateNodeInstance = 1;
	//bNotifyBecomeRelevant = true;
}


void UMyBTService_PredicateActions::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	uint8 ActionToTake = FMath::RandRange(0, 3);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), ActionToTake);
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::FromInt(ActionToTake));
}
