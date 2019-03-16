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

void AMobBasic::Exec_TryGetHit(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser, const FHitResult &HitInfo, EOnHitRefelction &Outp)
{
	if (!IsTargetable()) return;

	if (UDamageType->GetClass() == UDamageType_MeleeHit::StaticClass())
	{
		if (Damage >= 100000.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Vulnerable"));
			Outp = EOnHitRefelction::Vulnerable;

			FXManager->PlayEffects(HitInfo, EFXType::VE_OnHit);

			return;
		}
		else if (ActionManager->bIsBlocking)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blocking"));
			Outp = EOnHitRefelction::Block;

			FXManager->PlayEffects(HitInfo, EFXType::VE_OnBlock);

			return;
		}
		/*
		else if (AnimManager->bIsParry)
		{
			UE_LOG(LogTemp, Warning, TEXT("Parry"));
			Outp = EOnHitRefelction::Parry;
			return;
		}
		
		else if (AnimManager->bIsDodging)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dodging"));
			Outp = EOnHitRefelction::Immune;
			return;
		}*/
	}
	else if (UDamageType->GetClass() == UDamageType_ParryRefelction::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Vulnerable"));
		Outp = EOnHitRefelction::Vulnerable;
		
		if (Weapon)
			Weapon->EndSwing();

		FXManager->PlayEffects(HitInfo, EFXType::VE_OnHit);

		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	Outp = EOnHitRefelction::OnHit;
	
	if (Weapon)
		Weapon->EndSwing();

	FXManager->PlayEffects(HitInfo, EFXType::VE_OnHit);

	return;
}
