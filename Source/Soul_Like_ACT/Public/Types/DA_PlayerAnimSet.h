// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerAnimSet.generated.h"

class UDA_AttackMontage;
class UDA_UtilityMontage;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_PlayerAnimSet : public UDataAsset
{
	GENERATED_BODY()

public:

	//Dodge
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Dodge_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Dodge_B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Dodge_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Dodge_R;

	//Block
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Block;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Block_Guard;

	//Parry
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_UtilityMontage* Parry;

	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_AttackMontage* ComboSet1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_AttackMontage *DashAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_AttackMontage *ParryAttack;
};
