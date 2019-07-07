// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTT_DisableTargeting.h"
#include "Mob/MobBasic.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_DisableTargeting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMobBasic *LocalOwner = Cast<AMobBasic>(Cast<AController>(OwnerComp.GetOwner())->GetPawn());
	LocalOwner->SetFocus(nullptr);
	return EBTNodeResult::Succeeded;
}