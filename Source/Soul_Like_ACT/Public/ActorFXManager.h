// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoulDataAssets.h"
#include "ActorFXManager.generated.h"

UENUM(BlueprintType)
enum class EFXType : uint8
{
	OnHit,
	OnParry_Normal,
	OnParry_Perfect,
	OnParry_Failed,
	PowerShot,
	Immune,
};

class ASoulCharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UActorFXManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FX)
	UDA_FXPresets* FXPresets;
	
	template<class T>
	T* GetArrayRandom(TArray<T>& InArray);

public:	
	// Sets default values for this component's properties
	UActorFXManager();

	UFUNCTION(BlueprintCallable)
	void SpawnParticleWithHitResult(const FHitResult& HitResult, UParticleSystem* ParticleClass);

	UFUNCTION(BlueprintCallable)
	void PlaySoundWithHitResult(const FHitResult& HitResult, USoundBase* SoundCue);

	UFUNCTION(BlueprintCallable)
	bool PlayEffects(const FHitResult &HitResult, const EFXType InputType);
};

template<class T>
T* UActorFXManager::GetArrayRandom(TArray<T>& InArray)
{
	uint8 ArraySize = InArray.Num();

	if (ArraySize > 0)
		return &InArray[FMath::RandRange(0, ArraySize - 1)];
	else
		return nullptr;
}
