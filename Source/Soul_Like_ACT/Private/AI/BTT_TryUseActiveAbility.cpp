// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TryUseActiveAbility.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulModifierManager.h"


void UBTT_TryUseActiveAbility::BindOnGameplayAbilityEnded(AActor* SourceActor, TSubclassOf<USoulActiveAbility> InActiveAbility)
{
	USoulAbilitySystemComponent* PawnAblityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(SourceActor, false);
	if (!PawnAblityComponent->IsValidLowLevel())
	{
		LOG_FUNC_ERROR("Actor has no GASysComponent");
		return;
	}
	FGameplayAbilitySpecHandle SpecHandle;
	
	if (!USoulModifierManager::GetModifierManager(SourceActor)->FindActiveAbilitySpecHandle(InActiveAbility, SpecHandle))
	{
		LOG_FUNC_ERROR("Cannot find the active ability spec handle");
		return;
	}

	FGameplayAbilitySpec* GASpec = PawnAblityComponent->FindAbilitySpecFromHandle(SpecHandle);
	UGameplayAbility* LatestGAInstance;

	if (ensure(GASpec) && GASpec->IsActive())
	{
		LatestGAInstance = GASpec->NonReplicatedInstances[GASpec->NonReplicatedInstances.Num() - 1];
		LatestGAInstance->OnGameplayAbilityEnded.AddUObject(this, &UBTT_TryUseActiveAbility::EndTaskTrigger);
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
