// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetableActor.h"
#include "StatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATargetableActor::ATargetableActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	TargetIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockIconWidget"));
	TargetIcon->SetupAttachment(RootComponent);
	TargetIcon->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIcon->SetDrawSize(FVector2D{ 35.f,35.f });
}


void ATargetableActor::ToggleLockIcon(bool LockOn)
{
	if (!TargetIcon->GetWidgetClass()) return;

	if (!TargetIcon->bIsActive && LockOn)
	{
		TargetIcon->Activate(1);
	}
	else if(TargetIcon->bIsActive && !LockOn)
	{
		TargetIcon->Activate(0);
	}
}

// Called when the game starts or when spawned
void ATargetableActor::BeginPlay()
{
	Super::BeginPlay();

	TargetIcon->Activate(0);
	
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

