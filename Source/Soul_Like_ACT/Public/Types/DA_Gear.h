// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Gear.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	TwoHandedMace,
	OneHandedSword,
};

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_Gear : public UDataAsset
{
	GENERATED_BODY()

public:

	float BaseDamage;

	EWeaponType WeaponType;

	class USkeletalMesh *WeaponMesh;
};
