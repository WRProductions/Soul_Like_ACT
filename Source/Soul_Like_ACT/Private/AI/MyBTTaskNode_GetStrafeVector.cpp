// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTaskNode_GetStrafeVector.h"
#include "Mob/MobBasic.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/NavigationSystemBase.h"


EBTNodeResult::Type UMyBTTaskNode_GetStrafeVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor *PlayerPawn = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("PlayerPawn"));
	AActor *SelfActor = Cast<AController>(OwnerComp.GetOwner())->GetPawn();


	FVector PlayerToMobVec = PlayerPawn->GetActorLocation() - SelfActor->GetActorLocation();
	float MaxScale = PlayerToMobVec.Size();
	FVector RightVecFromDistance = PlayerToMobVec.ToOrientationQuat().GetRightVector().GetSafeNormal();
	FVector ForwardVecFromDistance = PlayerToMobVec.GetSafeNormal();

	FVector StrafeVec = SelfActor->GetActorLocation() + ForwardVecFromDistance * FMath::RandBool() * FMath::RandRange(0.f, MaxScale - 0.3f)
		+ RightVecFromDistance * FMath::RandBool() * FMath::RandRange(0.f, MaxScale - 0.3f);


	//FNavLocation StrafeVecOnNavMesh;
	//FNavigationSystem::GetCurrent<UNavigationSystem>(GetWorld())->ProjectPointToNavigation(StrafeVec, StrafeVecOnNavMesh);
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("StrafeVector", StrafeVec);

	DrawDebugLine(GetWorld(), SelfActor->GetActorLocation(), StrafeVec, FColor::Blue, 0, 10.f, 0, 3.f);
	return EBTNodeResult::Succeeded;
}
