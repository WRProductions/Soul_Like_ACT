// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ComboMontage.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EAnimType : uint8
{
	LightAttack,
	HeavyAttack,
	Block,
	Parry,
	Dodge,
};

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_ComboMontage : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage *AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAnimType AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Force = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasHeavyStrike;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_ComboMontage *Combo_Montage;
};
