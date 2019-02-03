// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Gear.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	VE_1HSword	UMETA(DisplayName = "1HSword"),
	VE_2HMace	UMETA(DisplayName = "2HMace"),
	VE_Fist		UMETA(DisplayName = "Fist"),
};

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_Gear : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::VE_1HSword;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMesh *WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BladeStartLength = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BladeTail = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase *SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem *BladeCollisionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem *OnHitFX;
};
