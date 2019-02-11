// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimDABuffer.generated.h"

class UDA_AttackMontage;
class UDA_UtilityMontage;
class AWeaponActor;
class UAnimManager;
class ASoul_Like_ACTCharacter;

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
class SOUL_LIKE_ACT_API UAnimDABuffer : public UObject
{
	GENERATED_BODY()

public:
	// Apply or Reset the combo DA to AnimManager
	static void ApplyComboDA(UAnimManager *AnimManagerRef, bool bResetCombo = 0);

	UFUNCTION(BlueprintCallable)
	static class UAnimMontage *GetAnimMontageFromAttackDA(UDA_AttackMontage *AttackDA, EComboChoise ELightOrHeavy);
	UFUNCTION(BlueprintCallable)
	static class UAnimMontage *GetPreMontageFromAttackDA(UDA_AttackMontage *AttackDA);
	UFUNCTION(BlueprintCallable)
	static class UAnimMontage *GetAnimMontageFromUtilityDA(UDA_UtilityMontage *UtilityDA);
};
