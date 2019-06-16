// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulGameplayAbility.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulTargetType.h"
#include "SoulCharacterBase.h"


FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FSoulGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FSoulGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ASoulCharacterBase* OwningCharacter = Cast<ASoulCharacterBase>(OwningActor);
	USoulAbilitySystemComponent* OwningASC = USoulAbilitySystemComponent::
		GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const USoulTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default ont he ability system component
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{

			FGameplayEffectSpecHandle newGEHandle = MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel);
			
			newGEHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Physical"), true), EventData.EventMagnitude);
			newGEHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Type.Posture"), true), EventData.EventMagnitude);
			
			ReturnSpec.TargetGameplayEffectSpecs.
			           Add(newGEHandle);
		}
	}
	return ReturnSpec;
}

FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpec(
	FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FSoulGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FSoulGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainerSpec(
	const FSoulGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

float USoulGameplayAbility::GetAttackSpeed() const
{
	ASoulCharacterBase *OwnerCharacter = Cast<ASoulCharacterBase>(GetOwningActorFromActorInfo());
	if (OwnerCharacter)
		return (OwnerCharacter->GetAttackSpeed() / 100.f + 1.f);
	return 1.f;
}

TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainer(
	FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FSoulGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}

void USoulModifierGameplayAbility::ApplyEffectsToSelf()
{
	for (auto& LocalGE : ModifierEffects)
	{
		FActiveGameplayEffectHandle NewActivatedGE = USoulAbilitySystemComponent::ApplyGE_ToSelf(GetOwningActorFromActorInfo(), LocalGE, GetAbilityLevel());
		
		if (NewActivatedGE.IsValid())
		{
			EffectCollection.Add(NewActivatedGE);
		}
	}
}

void USoulModifierGameplayAbility::RemoveEffectsFromSelf()
{
	for (FActiveGameplayEffectHandle& LocalActiveEffect : EffectCollection)
	{
		if (LocalActiveEffect.WasSuccessfullyApplied())
		{
			//ApplyGameplayEffectToOwner()
			LocalActiveEffect.GetOwningAbilitySystemComponent()->RemoveActiveGameplayEffect(LocalActiveEffect);
			LOG_FUNC_NORMAL("1");
		}
	}
}
