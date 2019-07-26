// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAbilitySystemComponent.h"
#include "SoulCharacterBase.h"
#include "Abilities/SoulModifierManager.h"
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

bool USoulAbilitySystemComponent::TryActivateAbility_Soul(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation)
{
	return TryActivateAbility(AbilityToActivate, bAllowRemoteActivation);
}

void USoulAbilitySystemComponent::TryActiveAbilityOnce(TSubclassOf<USoulGameplayAbility> InGameplayAbility, int32 Level)
{
	FGameplayAbilitySpec GA_Spec(InGameplayAbility, Level, INDEX_NONE, GetOwner());
	GiveAbilityAndActivateOnce(GA_Spec);
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

void USoulAbilitySystemComponent::CreateEventData(const AActor* Target, const AActor* Source, const FHitResult& InpHitResult, const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData& OutpEventData)
{
	FGameplayEffectContextHandle TempContextHandle(new FGameplayEffectContext());
	TempContextHandle.AddHitResult(InpHitResult);

	FGameplayEventData TempEventData;
	TempEventData.Instigator = Source;
	TempEventData.Target = Target;
	//TODO damage magnitude
	TempEventData.EventMagnitude = EventMagnitude;
	TempEventData.EventTag = EventTag;
	TempEventData.ContextHandle = TempContextHandle;

	OutpEventData = TempEventData;
}

bool USoulAbilitySystemComponent::GetMontageFromActiveAbility(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, UAnimMontage*& AnimMontage)
{
	USoulActiveAbility* LocalAbility = Cast<USoulActiveAbility>(ActiveAbilityClass->GetDefaultObject());
	if (LocalAbility)
	{
		AnimMontage = LocalAbility->MontageToPlay;
		return (AnimMontage != nullptr);
	}

	AnimMontage = NULL;
	return false;
}

float USoulAbilitySystemComponent::GetMontageSectionLength(UAnimMontage* AnimMontage, FName SectionName)
{
	int32 SectionIndex = AnimMontage->GetSectionIndex(SectionName);

	if (SectionIndex == INDEX_NONE)
		return 0.f;

	return AnimMontage->GetSectionLength(SectionIndex);
}

void USoulAbilitySystemComponent::BindOnGameplayAbilityEndFromActiveSpecHandle(
	const FGameplayAbilitySpecHandle& GameplaySpecHandle
	, const FOnGameplayAbilityEnded::FDelegate& OnGameplayAbilityEndedDelegate)
{
	auto *LocalGASpec = FindAbilitySpecFromHandle(GameplaySpecHandle);
	
	
}
