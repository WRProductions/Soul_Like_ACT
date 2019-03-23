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

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	bool ActivateAbilitiesWithWeapon(bool bAllowRemoteActivation) const;

	void GetActiveAbilitiesWithTags(struct FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities);

	bool bCanJumpSection;
	bool bWillJumpSection;
	FName JumpSectionName;
	UAnimMontage *JumpMontage;

	/**
	 * For Input only
	 */
	bool bIsLeftButtonPressed;
	float ChargingPoints;
	const float MaxChargingPoints = .35f;
	const float MaxPressedDuration = .5f;

public:
	bool bIsUsingMelee() const;
	bool bIsUsingAbility() const;
	bool bCanUseAnyGA() const;

	/**
	 * When pressed, charging starts
	 */
	UFUNCTION(BlueprintCallable)
	void OnLeftButtonPressed();
	
	/**
	 * When released, check charging points
	 * if charging points < 0.6, trigger normal melee attack
	 * else, trigger charge attack
	 */
	UFUNCTION(BlueprintCallable)
	void OnLeftButtonRelease();

	/**
	 * Dodge when SpaceBar pressed
	 */
	UFUNCTION(BlueprintCallable)
	void OnSpaceBarPressed();

	/**
	 * GA Trigger
	 */
	bool DoMeleeAttack();
	bool DoSpecialMeleeAttack();
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