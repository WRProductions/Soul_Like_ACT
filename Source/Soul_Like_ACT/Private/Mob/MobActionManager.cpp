// Fill out your copyright notice in the Description page of Project Settings.

#include "MobActionManager.h"
#include "Mob/MobBasic.h"
#include "Abilities/SoulModifierManager.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"

UMobActionManager::UMobActionManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UMobActionManager::TryUseActiveAbility(TSubclassOf<USoulActiveAbility> InActiveAbility, bool bUseCombo /*= false*/)
{
	FGameplayAbilitySpecHandle GASpecHandle;
	OwnerRef->GetModifierManager()->FindActiveAbilitySpecHandle(InActiveAbility, GASpecHandle);

	if (GASpecHandle.IsValid())
	{
		if (OwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(InActiveAbility, true))
		{
			if (bUseCombo)
			{
				EnableComboQuery();
			}
			else
			{
				ResetComboQuery();
			}
			return true;
		}
	}

	return false;
}

bool UMobActionManager::UseNextCombo(class UAnimInstance* AnimInstance)
{
	if (!GetIsComboInQuery())
	{
		LOG_FUNC_ERROR("Can't use combo.");
		
		ResetComboQuery();
		return false;
	}

	UAnimMontage* CurrMontage = AnimInstance->GetCurrentActiveMontage();
	
	if (!CurrMontage || CurrMontage->IsValidSectionIndex(CurrComboStage + 1))
	{
		ResetComboQuery();

		LOG_FUNC_ERROR("No montage is playing or No connected combo")

		return false;
	}
	else
	{
		++CurrComboStage;
		
		AnimInstance->Montage_JumpToSection(CurrMontage->GetSectionName(CurrComboStage), CurrMontage);
		
		return true;
	}
}

UMobActionManager* UMobActionManager::GetMobActionManager(AActor* SourceActor)
{
	AMobBasic* MobActor = Cast<AMobBasic>(SourceActor);
	if (MobActor->IsValidLowLevel())
	{
		return MobActor->ActionManager;
	}
	else
	{
		return nullptr;
	}
}
