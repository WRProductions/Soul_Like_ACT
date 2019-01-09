// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/MyBTTaskNode_EnableTargeting.h"
#include "Mob/MobBasic.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMyBTTaskNode_EnableTargeting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor *TargetPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerPawn"));

	AMobBasic *LocalOwner = Cast<AMobBasic>(Cast<AController>(OwnerComp.GetOwner())->GetPawn());
	if (!LocalOwner || !TargetPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMyBTTaskNode_EnableTargeting failed cast %s"), *OwnerComp.GetOwner()->GetName());
		return EBTNodeResult::Failed;
	}

	
	LocalOwner->SetFocus(1, TargetPawn);
	LocalOwner->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	return EBTNodeResult::Succeeded;
}
