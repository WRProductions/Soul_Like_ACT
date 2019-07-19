// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GeneralEnums.h"
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

	void GetActiveAbilitiesWithTags(struct FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities);

	bool bCanJumpSection;
	bool bWillJumpSection;
	FName JumpSectionName;
	UAnimMontage *JumpMontage;

	EKatanaStance KatanaStance;

public:
	bool bIsUsingMelee() const;
	bool bIsUsingSkills() const;
	bool bCanUseAnyGA() const;
	bool bIsUsingParry() const;

	bool bIsFree() const { return bCanUseAnyGA() && !bIsUsingParry() && !bIsUsingMelee(); }

	bool SetKatanaStance(EKatanaStance InKatanaStance);

	UFUNCTION(BlueprintCallable)
	bool DoMeleeAttack();
	UFUNCTION(BlueprintCallable)
	bool DoSpecialMeleeAttack();
	UFUNCTION(BlueprintCallable)
	bool DoDodge();

	UFUNCTION(BlueprintCallable)
		bool DoParry_Start();
	UFUNCTION(BlueprintCallable)
		bool DoParry_End();

	/**
	 * These 2 functions are called via Active Melee GameplayAbilities
	 */
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