// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ComboMontage.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EAnimType : uint8
{
	Attack,
	DashAttack,
	SprintAttack,
	Block,
	Parry,
	Dodge,
};

UCLASS()
class SOUL_LIKE_ACT_API UDA_AnimMontage : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	EAnimType AnimType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UAnimMontage *Normal_Montage;
};


/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_AttackMontage : public UDA_AnimMontage
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UAnimMontage *Pre_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UAnimMontage *Heavy_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Light_Force = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Heavy_Force = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Light_DmgMulti = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	float Heavy_DmgMulti = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UDA_AttackMontage *Combo_DA;
};


UCLASS()
class SOUL_LIKE_ACT_API UDA_UtilityMontage : public UDA_AnimMontage
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
	UDA_UtilityMontage *Combo_DA;
};