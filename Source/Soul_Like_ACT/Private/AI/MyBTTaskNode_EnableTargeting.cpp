// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTTaskNode_EnableTargeting.h"
#include "Mob/MobBasic.h"
#include "Mob/Mob_TargetingComponent.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTaskNode_EnableTargeting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor *TargetPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));

	AMobBasic *LocalOwner = Cast<AMobBasic>(Cast<AController>(OwnerComp.GetOwner())->GetPawn());
	
	if (!LocalOwner)
	{
		LOG_FUNC_FAIL();
		return EBTNodeResult::Failed;
	}

	if (!TargetPawn)
	{
		LOG_FUNC_FAIL();
		return EBTNodeResult::Failed;
	}

	LocalOwner->SetFocus(TargetPawn);
	
	//TODO 
	LocalOwner->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	
	return EBTNodeResult::Succeeded;
}
