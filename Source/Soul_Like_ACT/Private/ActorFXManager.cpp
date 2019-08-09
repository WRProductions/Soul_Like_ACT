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
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld()
		, ParticleClass
		, GetOwner()->GetActorLocation()
		, UKismetMathLibrary::MakeRotFromX(HitResult.Normal)
		, FVector::OneVector
		, true);
}

void UActorFXManager::PlaySoundWithHitResult(const FHitResult &HitResult, USoundBase *SoundCue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
}	

bool UActorFXManager::PlayEffects(const FHitResult& HitResult, const EFXType InputType)
{
	UParticleSystem* ParticalToUse;
	USoundBase* SoudToUse;
	
	switch (InputType)
	{
	case EFXType::OnHit:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnHitUnguard);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnHitUnguard);
		break;
	case EFXType::OnParry_Normal:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnParryNormal);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnParryNormal);
		break;
	case EFXType::OnParry_Perfect:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnParryPerfect);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnParryPerfect);
		break;
	case EFXType::OnParry_Failed:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnParryFailed);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnParryFailed);
		break;
	case EFXType::PowerShot:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnParryFailed);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnParryFailed);
		break;
	case EFXType::Immune:
		ParticalToUse = GetArrayRandom(FXPresets->PS_OnParryPerfect);
		SoudToUse = GetArrayRandom(FXPresets->Sound_OnParryPerfect);
		break;
	default:
		return false;
	}


	SpawnParticleWithHitResult(HitResult, ParticalToUse);
	PlaySoundWithHitResult(HitResult, SoudToUse);
	return true;
}
