// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/GeneralEnums.h"
#include "Types/DA_Gear.h"
#include "ActionSysManager.generated.h"

class ASoul_Like_ACTCharacter;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ActiveAbilities)
	UDA_AbilitiesPreset* ActiveAbilitiesPreset;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void GetActiveAbilitiesWithTags(struct FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities);

	bool bCanJumpSection;
	bool bWillJumpSection;
	uint8 CurrentComboSectionIndex;
	UAnimMontage *MontageToPlay;

	EKatanaStanceType CurrentStance = EKatanaStanceType::Idle;

public:
	bool CanUseAnyGA() const;
	bool IsUsingMelee() const;
	bool IsUsingSkills() const;
	bool IsUsingParry() const;

	bool bIsFree() const { return CanUseAnyGA() && !IsUsingParry() && !IsUsingMelee(); }

	UFUNCTION(BlueprintCallable)
	void GetStance(EKatanaStanceType &OutStance) const { OutStance = CurrentStance;}

	UFUNCTION(BlueprintCallable)
	bool SetNewStance(EKatanaStanceType InStance);
	UFUNCTION(BlueprintCallable)
	bool CanChangeStance(EKatanaStanceType InStance);
	
	UFUNCTION(BlueprintCallable)
	bool DoMeleeAttack();
	UFUNCTION(BlueprintCallable)
	bool DoSpecialMeleeAttack();
	UFUNCTION(BlueprintCallable)
	bool DoDodge();
	UFUNCTION(BlueprintCallable)
	bool DoChangeStance(EKatanaStanceType InStance);

	UFUNCTION(BlueprintCallable)
	bool DoParry_Start();
	UFUNCTION(BlueprintCallable)
	bool DoParry_End();


	//************************************
	// Method:    SetJumpSection
	// Qualifier: 
	// Parameter: FName InpComboScetionName
	// Parameter: UAnimMontage * InpMontage nullptr by default, it the jump section will calculated according to the montage index
	//************************************
	UFUNCTION(BlueprintCallable)
	bool SetJumpSection(uint8 ComboSectionIndex, const UAnimMontage *InpMontage);
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