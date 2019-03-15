// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionSysManager.generated.h"

class USoulGameplayAbility;
class UGameplayAbility;
class UAnimMontage;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_LIKE_ACT_API UActionSysManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionSysManager();

	class ASoul_Like_ACTCharacter *PlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool ActivateAbilitiesWithWeapon(bool bAllowRemoteActivation) const;

	void GetActiveAbilitiesWithTags(struct FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities);

	bool bCanJumpSection;
	bool bWillJumpSection;
	FName JumpSectionName;
	UAnimMontage *JumpMontage;

public:
	bool bBlockMovement;
	bool bIsUsingMelee() const;
	bool bIsUsingAbility() const;
	bool bCanUseAnyGA() const;

	bool DoMeleeAttack();
	bool DoDodge();

	UFUNCTION(BlueprintCallable)
	bool SetJumpSection(const FName InpComboScetionName, UAnimMontage *InpMontage);
	
	UFUNCTION(BlueprintCallable)
	bool JumpSectionForCombo();
	
	UFUNCTION(BlueprintCallable)
	bool TryEnableJumpSection () 
	{
		if(bCanJumpSection) 
			return bWillJumpSection = true;
		return false;
	}

	UFUNCTION(BlueprintCallable)
	static float PredictMovingDirection(ASoul_Like_ACTCharacter *CharacterRef);

	UFUNCTION(BlueprintCallable)
	static FName Get4WaysStepDirection_GA(float PredictableMovingDirection);
};