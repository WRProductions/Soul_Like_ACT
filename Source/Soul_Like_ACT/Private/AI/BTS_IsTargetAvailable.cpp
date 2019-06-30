// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTS_IsTargetAvailable.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "SoulCharacterBase.h"
#include "Mob/MobController.h"


UBTS_IsTargetAvailable::UBTS_IsTargetAvailable()
{
	bCreateNodeInstance = 1;
}


void UBTS_IsTargetAvailable::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	AMobController *Owner = Cast<AMobController>(OwnerComp.GetOwner());
	if (!Owner)
	{
		//OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("SelfActor"), nullptr);
		Owner->GetBTComp()->StopTree(EBTStopMode::Forced);
		return;
	}

	ASoulCharacterBase *Target = Cast<ASoulCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsTargetAliveKey.SelectedKeyName, (Target && !Target->GetIsDead()));
	return;
}
