// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAbilitySystemComponent.h"
#include "SoulCharacterBase.h"
#include "Abilities/SoulGameplayAbility.h"
#include "AbilitySystemGlobals.h"


void USoulAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer & GameplayTagContainer, TArray<USoulGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<USoulGameplayAbility>(ActiveAbility));
		}
	}
}

int32 USoulAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	ASoulCharacterBase* OwningCharacter = Cast<ASoulCharacterBase>(OwnerActor);

	if (OwningCharacter)
	{
		//return OwningCharacter->GetCharacterLevel();
		return 1;
	}
	return 1;
}

USoulAbilitySystemComponent * USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor * Actor, bool LookForComponent)
{
	return Cast<USoulAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));

}

FActiveGameplayEffectHandle USoulAbilitySystemComponent::ApplyGE_ToSelf(const AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffect, const int32 AbilityLevel = 1/*=1*/)
{
	return ApplyGE_ToTarget(Actor, Actor, GameplayEffect, AbilityLevel);
}

FActiveGameplayEffectHandle USoulAbilitySystemComponent::ApplyGE_ToTarget(const AActor* FromActor, const AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffect, const int32 AbilityLevel)
{
	USoulAbilitySystemComponent* TempComp = GetAbilitySystemComponentFromActor(TargetActor);
	if (TempComp)
	{
		FGameplayEffectContextHandle EffectContext = TempComp->MakeEffectContext();

		EffectContext.AddSourceObject(FromActor);

		FGameplayEffectSpecHandle NewHandle = TempComp->MakeOutgoingSpec(GameplayEffect, AbilityLevel, EffectContext);

		if (NewHandle.IsValid())
		{
			return (TempComp->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TempComp));
		}
	}

	return FActiveGameplayEffectHandle();
}
