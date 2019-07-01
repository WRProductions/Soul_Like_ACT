// Fill out your copyright notice in the Description page of Project Settings.

#include "MobActionManager.h"
#include "Mob/MobBasic.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulAbilitySysBPLib.h"

// Sets default values for this component's properties
UMobActionManager::UMobActionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 0;
}

bool UMobActionManager::TryUseActiveAbility(TSubclassOf<USoulActiveAbility> InActiveAbility, const float InMultiplier, const int32 InMontageIndex)
{
	FGameplayAbilitySpecHandle GASpecHandle = USoulModifierManager::GetActiveAbilitySpecHandleFromCharacter(OwnerRef, InActiveAbility);

	if (GASpecHandle.IsValid())
	{
		if (OwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(InActiveAbility, true))
			return true;
	}

	return false;
}

bool UMobActionManager::TryUseCombo(TSubclassOf<USoulActiveAbility> InActiveAbility, const float InMultiplier, bool bForceNew)
{
	if (!CurrActiveAbility->IsValidLowLevel() || CurrActiveAbility != InActiveAbility || bForceNew)
	{
		CurrComboStage = 0;
		CurrActiveAbility = InActiveAbility;

		TryUseActiveAbility(CurrActiveAbility, InMultiplier, CurrComboStage);

		return false;
	}
	else
	{
		UAnimInstance* OwnerAnimInstance = OwnerRef->GetMesh()->GetAnimInstance();
		
		UAnimMontage* GA_Montage;
		USoulAbilitySysBPLib::GetMontageFromActiveAbility(InActiveAbility, GA_Montage);

		//Reset
		if (GA_Montage->IsValidSectionIndex(CurrComboStage))
		{
			++CurrComboStage;
		}
		else
		{
			CurrComboStage = 0;
		}

		OwnerAnimInstance->Montage_JumpToSection(GA_Montage->GetSectionName(CurrComboStage), GA_Montage);
		
		return true;
	}
}
