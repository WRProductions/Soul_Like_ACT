// Fill out your copyright notice in the Description page of Project Settings.

#include "MobController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Mob/MobBasic.h"

AMobController::AMobController()
{
	// Setup the perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	sightConfig->SightRadius = 1500.f;
	sightConfig->LoseSightRadius = 2500.f;
	sightConfig->PeripheralVisionAngleDegrees = 360.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMobController::AISenseUpdateMessage);
}

void AMobController::BeginPlay()
{
	Super::BeginPlay();

	//FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus

}



void AMobController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	PossessedMob = Cast<AMobBasic>(InPawn);

}

void AMobController::UnPossess()
{
	Super::UnPossess();

	PossessedMob = nullptr;
}

void AMobController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobController::AISenseUpdateMessage(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("%s is founded by %s"), *Actor->GetName(), *PossessedMob->GetName());
}
