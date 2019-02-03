// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimDABuffer.generated.h"

class UDA_ComboMontage;
class AWeaponActor;
class ATargetableActor;

UENUM(BlueprintType)
enum class EComboChoise : uint8
{
	LightAttack,
	HeavyAttack,
};

/*
	The class does not contain any public variables.
	It's used to unpack the DA_ComboMontage and set variables for the character and AnimManager.

*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UAnimDABuffer : public UActorComponent
{
	GENERATED_BODY()

	class ASoul_Like_ACTCharacter *OwnerRef;

	class UAnimManager *PlayerAnimManager;

	class AWeaponActor *PlayerWeapon;

public:	
	// Sets default values for this component's properties
	UAnimDABuffer();
		
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void CalculateDmg(AWeaponActor *PlayerWeapon);

	void CalculateForce(ATargetableActor *Target);

	void GetCombo(EComboChoise ComboType);

public:

	bool PlayComboMontage(EComboChoise );
};
