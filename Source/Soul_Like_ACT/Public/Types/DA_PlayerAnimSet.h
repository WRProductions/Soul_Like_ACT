// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerAnimSet.generated.h"

class UDA_ComboMontage;

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
		UDA_ComboMontage* Dodge_F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Dodge_B;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Dodge_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Dodge_R;

	//Block
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Block;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Block_Guard;

	//Parry
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* Parry;

	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* ComboSet1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage* ComboSet2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage *DashAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDA_ComboMontage *ParryAttack;
};
