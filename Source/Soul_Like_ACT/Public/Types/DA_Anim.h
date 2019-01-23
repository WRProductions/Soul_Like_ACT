// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Anim.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EAnimType : uint8
{
	LightAttack,
	HeavyAttack,
	Defence,
	Parry,
	Dodge,
};

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_Anim : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAnimMontage *AnimMontage;

	EAnimType AnimType;

	float Force;

	float DamageMultiplier;

	UDA_Anim *ComboDA;
};
