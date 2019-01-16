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

	if (!TargetIcon->IsVisible() && LockOn)
	{
		TargetIcon->SetVisibility(1);
	}
	else if(TargetIcon->IsVisible() && !LockOn)
	{
		TargetIcon->SetVisibility(0);
	}
}

void ATargetableActor::TriggerSlowMotion_WithDelay()
{
	if (GetWorldTimerManager().GetTimerRemaining(Handler_SlowMotionDelay) <= 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Handler_SlowMotionDelay.IsValid()");
		GetWorldTimerManager().SetTimer(Handler_SlowMotionDelay, this, &ATargetableActor::TriggerSlowMotion, 1.f, 0, 0.1f);
	}
}

const bool ATargetableActor::IsInRivalFaction(ATargetableActor *DamageDealer, ATargetableActor *DamageReceiver)
{
	if (DamageDealer->Faction == EActorFaction::Player && DamageReceiver->Faction == EActorFaction::Enemy)
		return 1;
	
	else if (DamageDealer->Faction == EActorFaction::Enemy && DamageReceiver->Faction == EActorFaction::Player)
		return 1;

	return 0;
}


bool ATargetableActor::OnDamageTaken_Implementation(float Damage, class UDamageType const* DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	TriggerSlowMotion_WithDelay();

	StatusComponent->TakeDamage(FMath::TruncToInt(Damage));
	bool IsDead_Local = StatusComponent->GetHealthPercent() <= 0.f;
	
	if (IsDead_Local) OnDead_Delegate.Broadcast();
	
	return (IsDead_Local);
}

// Called when the game starts or when spawned
void ATargetableActor::BeginPlay()
{
	Super::BeginPlay();
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

