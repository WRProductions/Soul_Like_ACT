// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBTTaskNode_GetStrafeVector.h"
#include "Mob/MobBasic.h"
#include "GameFramework/Controller.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"


EBTNodeResult::Type UMyBTTaskNode_GetStrafeVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter *SelfActor = Cast<ACharacter>(Cast<AController>(OwnerComp.GetOwner())->GetPawn());

	FVector StrafeVec = SelfActor->GetActorLocation() 
		+ SelfActor->GetActorForwardVector() * FMath::RandBool() * FMath::RandRange(StrafeRadius_Min, StrafeRadius_Max)
		+ SelfActor->GetActorRightVector() * FMath::RandBool() * FMath::RandRange(StrafeRadius_Min, StrafeRadius_Max);


 	FNavLocation StrafeVecOnNavMesh;
 	FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->ProjectPointToNavigation(
 		StrafeVec, StrafeVecOnNavMesh);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("StrafeVector", StrafeVecOnNavMesh.Location);

	DrawDebugLine(GetWorld(), SelfActor->GetActorLocation(), StrafeVecOnNavMesh.Location, FColor::Blue, 0, 10.f, 0, 3.f);

	return EBTNodeResult::Succeeded;
}
