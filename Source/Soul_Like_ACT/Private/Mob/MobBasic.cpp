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

// Sets default values
AMobBasic::AMobBasic()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetingComponent = CreateDefaultSubobject<UMob_TargetingComponent>(TEXT("TargetingComponent"));
	TargetingComponent->OwnerRef = this;

	ActionManager = CreateDefaultSubobject<UMobActionManager>(TEXT("ActionManager"));
	ActionManager->OwnerRef = this;

	Faction = EActorFaction::Enemy;

	TSubclassOf <class UUserWidget> IconWidget = ConstructorHelpers::FClassFinder<UUserWidget>(
		TEXT("/Game/UMG/Mob/w_mob_LockIcon")).Class;
	TargetIcon->SetWidgetClass(IconWidget);
	TargetIcon->SetVisibility(0);
}

// Called when the game starts or when spawned
void AMobBasic::BeginPlay()
{
	Super::BeginPlay();
}



void AMobBasic::HandleOnDead()
{
	Super::HandleOnDead();

	MobOnDead();
}

void AMobBasic::MobOnDead_Implementation()
{
	//Remove Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	AController *LocalController = GetController();
		
	if (LocalController)
		LocalController->UnPossess();

	GetTargetingComponent()->FacingTarget_End();

	Faction = EActorFaction::Untargetable;

	StopAnimMontage(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
}

void AMobBasic::SetTarget(AActor *PlayerPawn) const
{
	TargetingComponent->SetTarget(PlayerPawn);
}

void AMobBasic::SetFocus(AActor * Target)
{

	if (!Target)
	{
		TargetingComponent->FacingTarget_End();
		return;
	}
	TargetingComponent->FacingTarget_Init(Target, EFacingPriority::Target);
	return;
}

bool AMobBasic::GetIsTargetingEnabled() const
{
	return TargetingComponent->GetIsFacingTarget();
}

void AMobBasic::GetWeapon_Implementation(AWeaponActor*& OutWeaponActor)
{
	OutWeaponActor = Weapon;
}

void AMobBasic::SetDisabled(bool bEnable)
{
	Super::SetDisabled(bEnable);

	Cast<AMobController>(GetController())->SetAIEnabled(bEnable);
}
