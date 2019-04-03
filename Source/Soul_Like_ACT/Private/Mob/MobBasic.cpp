// Fill out your copyright notice in the Description page of Project Settings.

#include "MobBasic.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Mob/Mob_TargetingComponent.h"
#include "Mob/MobActionManager.h"
#include "Item/WeaponActor.h"
#include "UObject/ConstructorHelpers.h"
#include "ActorFXManager.h"
#include "Components/WidgetComponent.h"
#include "Types/DamageTypes.h"

// Sets default values
AMobBasic::AMobBasic()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UMob_TargetingComponent>(TEXT("TargetingComponent"));

	ActionManager = CreateDefaultSubobject<UMobActionManager>(TEXT("ActionManager"));

	Faction = EActorFaction::Enemy;

	TSubclassOf <class UUserWidget> IconWidget = ConstructorHelpers::FClassFinder<UUserWidget>(
		TEXT("/Game/UMG/Mob/WB_LockIcon")).Class;
	TargetIcon->SetWidgetClass(IconWidget);
	TargetIcon->SetVisibility(0);
}

// Called when the game starts or when spawned
void AMobBasic::BeginPlay()
{
	Super::BeginPlay();
}



void AMobBasic::OnDeath_Implementation(int32 CurrHealth, int32 MaxHealth)
{
	if (CurrHealth <= 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, GetName() + " is dead");

		//Remove Collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		AController *LocalController = GetController();
		if (LocalController)
			LocalController->UnPossess();

		GetTargetingComponent()->FacingTarget_End();

		Faction = EActorFaction::Untargetable;

		StopAnimMontage(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());

		ToggleLockIcon(0);
	}
}

// Called every frame
void AMobBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMobBasic::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMobBasic::SetTarget(AActor *PlayerPawn) const
{
	TargetingComponent->SetTarget(PlayerPawn);
}

bool AMobBasic::GetIsStun()
{
	//TODO
	//If Stuned, cannnot facing player
	return 0;
}

void AMobBasic::SetFocus(bool InputMode, AActor * Target)
{

	if (TargetingComponent->GetIsEnabled() && InputMode == 0)
	{
		TargetingComponent->FacingTarget_End();
		TargetingComponent->SetTarget(nullptr);
		return;
	}
	if (!TargetingComponent->GetIsEnabled() && InputMode == 1)
	{
		TargetingComponent->FacingTarget_Init();
		TargetingComponent->SetTarget(Target);
		return;
	}
}

bool AMobBasic::GetIsTargetingEnabled() const
{
	return TargetingComponent->GetIsEnabled();
}
