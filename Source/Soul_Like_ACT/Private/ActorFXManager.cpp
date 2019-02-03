// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorFXManager.h"
#include "Kismet/GameplayStatics.h"
#include "Types/DA_FXCollection.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UActorFXManager::UActorFXManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 0;
}


// Called when the game starts
void UActorFXManager::BeginPlay()
{
	Super::BeginPlay();

	check(FXCollection);
}

void UActorFXManager::SpawnParticleWithHitResult(const FHitResult &HitResult, UParticleSystem *ParticleClass)
{
	FRotator NormVec = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal) * -1;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleClass, HitResult.TraceEnd, NormVec, FVector::OneVector, true);
}

void UActorFXManager::SpawnSoundWithHitResult(const FHitResult &HitResult, USoundBase *SoundCue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
}	

bool UActorFXManager::PlayEffects(const FHitResult &HitResult, const EFXType InputType)
{
	USoundBase *TargetSFX;
	UParticleSystem *TargetVFX;

	switch (InputType)
	{
	case EFXType::VE_OnHit:
		TargetSFX = FXCollection->OnHitSound;
		TargetVFX = FXCollection->OnHitParticle;
		break;
	case EFXType::VE_OnBlock:
		TargetSFX = FXCollection->OnHitSound;
		TargetVFX = FXCollection->OnHitParticle;
		break;
	case EFXType::VE_OnParry:
		TargetSFX = FXCollection->OnHitSound;
		TargetVFX = FXCollection->OnHitParticle;
		break;
	default:
		return 0;
	}

	SpawnParticleWithHitResult(HitResult, TargetVFX);
	SpawnSoundWithHitResult(HitResult, TargetSFX);

	return 1;
}
