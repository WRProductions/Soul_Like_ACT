// Fill out your copyright notice in the Description page of Project Settings.

#include "MobController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Mob/Mob_TargetingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Mob/MobBasic.h"

AMobController::AMobController()
{
	//Perception
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	sightConfig->SightRadius = 1500.f;
	sightConfig->LoseSightRadius = 2000.f;
	sightConfig->PeripheralVisionAngleDegrees = 360.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	AIPerceptionComponent->ConfigureSense(*sightConfig);

	//BB and BT
	BlockBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBaordComponent"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AMobController::BeginPlay()
{
	Super::BeginPlay();

	//FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus
}

void AMobController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedMob = Cast<AMobBasic>(InPawn);

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMobController::AISenseUpdateMessage);

	//set up bb and bt
	check(BlockBoardData);
	check(BehaviorTreeAsset);
}

void AMobController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedMob = nullptr;
}

void AMobController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobController::GainRageOnHit(AActor* SourceActor, AActor* TargetActor, const FHitResult HitResult)
{

}

void AMobController::AISenseUpdateMessage(AActor* Actor, FAIStimulus Stimulus)
{

	if (!PossessedMob)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveDynamic(this, &AMobController::AISenseUpdateMessage);
		return;
	}

	if (Cast<ASoulCharacterBase>(Actor)->Faction == EActorFaction::Player)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlockBoardComp->SetValueAsObject("PlayerPawn", Actor);
			UE_LOG(LogTemp, Warning, TEXT("%s is founded by %s"), *Actor->GetName(), *PossessedMob->GetName());
		}
		else
		{
			BlockBoardComp->SetValueAsObject("PlayerPawn", nullptr);
			UE_LOG(LogTemp, Warning, TEXT("%s is lost by %s"), *Actor->GetName(), *PossessedMob->GetName());
		}
	}
}
