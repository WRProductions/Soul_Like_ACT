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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = DataAsset)
	class UDA_FXCollection *FXCollection;

public:	
	// Sets default values for this component's properties
	UActorFXManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SpawnParticleWithHitResult(const FHitResult &HitResult, UParticleSystem *ParticleClass);
	
	void SpawnSoundWithHitResult(const FHitResult &HitResult, USoundBase *SoundCue);

public:

	UFUNCTION(BlueprintCallable)
	bool PlayEffects(const FHitResult &HitResult, const EFXType InputType);
};
