// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTS_DistanceToPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Mob/MobController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTService_DistanceToPlayer::UMyBTService_DistanceToPlayer()
{
	bCreateNodeInstance = 1;

	//bNotifyBecomeRelevant = 1;
}

void UMyBTService_DistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor *PlayeyPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (PlayeyPawn)
	{
		float DistanceToPlayer = FVector::Distance(Cast<AMobController>(OwnerComp.GetOwner())->GetPawn()->GetActorLocation(), PlayeyPawn->GetActorLocation());
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::SanitizeFloat(DistanceToPlayer));
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToPlayerKey.SelectedKeyName, DistanceToPlayer);
		return;
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToPlayerKey.SelectedKeyName, -1.f);
}
