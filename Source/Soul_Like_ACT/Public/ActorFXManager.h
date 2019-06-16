// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorFXManager.generated.h"

UENUM(BlueprintType)
enum class EFXType : uint8
{
	VE_OnHit	UMETA(DisplayName = "OnHit"),
	VE_OnParry	UMETA(DisplayName = "OnParry"),
	VE_OnBlock	UMETA(DisplayName = "OnBlock"),
};


class ASoulCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UActorFXManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorFXManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		TArray<USoundBase*> OnHitSounds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		TArray<USoundBase*> OnBlockSounds;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		TArray<USoundBase*> OnParrySounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		TArray<UParticleSystem*> OnHitParticles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		TArray<UParticleSystem*> OnBlockParticles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		TArray<UParticleSystem*> OnParryParticles;

	UFUNCTION(BlueprintCallable)
	void SpawnParticleWithHitResult(const FHitResult& HitResult, UParticleSystem* ParticleClass);

	UFUNCTION(BlueprintCallable)
	void SpawnSoundWithHitResult(const FHitResult& HitResult, USoundBase* SoundCue);

	UFUNCTION(BlueprintCallable)
	bool PlayEffects(const FHitResult &HitResult, const EFXType InputType);
};
