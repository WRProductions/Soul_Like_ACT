// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TryUseActiveAbility.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulModifierManager.h"


void UBTT_TryUseActiveAbility::BindOnGameplayAbilityEnded(AActor* SourceActor, TSubclassOf<USoulActiveAbility> ActiveAbility)
{
	USoulAbilitySystemComponent* PawnAblityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(SourceActor, false);
	if (!PawnAblityComponent->IsValidLowLevel())
	{
		LOG_FUNC_ERROR("Actor has no GASysComponent");
		return;
	}
	FGameplayAbilitySpecHandle SpecHandle;
	USoulModifierManager::GetModifierManager(SourceActor)->FindActiveAbilitySpecHandle(ActiveAbility, SpecHandle);
	FGameplayAbilitySpec* GASpec = PawnAblityComponent->FindAbilitySpecFromHandle(SpecHandle);
	UGameplayAbility* PrimaryAbility;

	if (GASpec->IsActive())
	{
		PrimaryAbility = GASpec->GetPrimaryInstance();
		PrimaryAbility->OnGameplayAbilityEnded.AddUObject(this, &UBTT_TryUseActiveAbility::EndTaskTrigger);
	}
	else
	{
		LOG_FUNC_ERROR("GameplayAbility is not instantiaed");
		FinishExecute(false);
	}
}

void UBTT_TryUseActiveAbility::EndTaskTrigger(class UGameplayAbility* GA)
{
	FinishExecute(true);
}
