// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorFXManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UActorFXManager::UActorFXManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 0;
}

void UActorFXManager::SpawnParticleWithHitResult(const FHitResult &HitResult, UParticleSystem *ParticleClass)
{
	if (!HitResult.bBlockingHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld()
			, OnHitParticles[0]
			, GetOwner()->GetActorLocation()
			, FRotator::ZeroRotator
			, FVector::OneVector
			, true);
	}

	FRotator NormVec = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal) * -1;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleClass, HitResult.TraceEnd, NormVec, FVector::OneVector, true);
}

void UActorFXManager::SpawnSoundWithHitResult(const FHitResult &HitResult, USoundBase *SoundCue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
}	

bool UActorFXManager::PlayEffects(const FHitResult& HitResult, const EFXType InputType)
{
	SpawnParticleWithHitResult(HitResult, OnHitParticles[0]);
	SpawnSoundWithHitResult(HitResult, OnHitSounds[0]);

	return 1;
}
