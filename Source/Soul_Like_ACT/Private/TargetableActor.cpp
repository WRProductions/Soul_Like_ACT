// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetableActor.h"
#include "StatusComponent.h"

// Sets default values
ATargetableActor::ATargetableActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
}

// Called when the game starts or when spawned
void ATargetableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetableActor::BroadCastOnHealthChanged()
{
	if (OnActorHealthChanged.IsBound())
		OnActorHealthChanged.Broadcast(StatusComponent->Health, StatusComponent->MaxHealth);
}

// Called every frame
void ATargetableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATargetableActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

