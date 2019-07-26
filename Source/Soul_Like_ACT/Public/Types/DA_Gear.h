// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Interfaces/GeneralEnums.h"
#include "DA_Gear.generated.h"

class USoulActiveAbility;

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
		EWeaponType WeaponType = EWeaponType::Katana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BladeStartLength = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BladeTail = 120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* OnHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* BladeCollisionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* OnHitFX;
};

UCLASS()
class SOUL_LIKE_ACT_API UDA_AbilitiesPreset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_MidToHeavy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_HeavyToMid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_Unsheathe;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_Sheathe;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_MidToBatto;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_BattoToMid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_SheathedToBatto;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Abilities)
	TSubclassOf<USoulActiveAbility> GA_BattoToSheathed;
};