// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_FXCollection.generated.h"

class UParticleSystem;
class USoundBase;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UDA_FXCollection : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		USoundBase *OnHitSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		USoundBase *OnBlockSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
		USoundBase *OnParrySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		UParticleSystem *OnHitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		UParticleSystem *OnBlockParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Visual)
		UParticleSystem *OnParryParticle;


};
