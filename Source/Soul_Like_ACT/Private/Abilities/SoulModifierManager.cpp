// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulModifierManager.h"
#include "SoulCharacterBase.h"
#include "Abilities/SoulAbilitySystemComponent.h"

// Sets default values for this component's properties
USoulModifierManager::USoulModifierManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USoulModifierManager::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerRef);
}

class USoulAbilitySystemComponent* USoulModifierManager::GetSoulGAComponent()
{
	return USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(PlayerRef);
}

void USoulModifierManager::AddStartupGameplayAbilities()
{
	check(PlayerRef->GetAbilitySystemComponent());

	PlayerRef->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerRef, PlayerRef);

	if (PlayerRef->Role == ROLE_Authority)
	{
		for (auto& TempActiveAbility : DefaultActiveAbilities)
		{
			if (TempActiveAbility.Key)
			{
				FGameplayAbilitySpecHandle LocalGrantedActiveGA = PlayerRef->GetAbilitySystemComponent()->GiveAbility(
					FGameplayAbilitySpec(
						TempActiveAbility.Key
						, TempActiveAbility.Value
						, INDEX_NONE
						, PlayerRef)
				);
				
				GrantedDefaultActiveGAs.Add(LocalGrantedActiveGA);
			}
		}
		for (auto& TempModifier : DefaultModifiers)
		{
			if (TempModifier.Key)
			{
				auto LocalGrantedMod = PlayerRef->GetAbilitySystemComponent()->GiveAbility(
					FGameplayAbilitySpec(
						TempModifier.Key
						, TempModifier.Value
						, INDEX_NONE
						, PlayerRef)
				);

				GrantedDefaultModifiers.Add(LocalGrantedMod);

				GetSoulGAComponent()->TryActivateAbility(LocalGrantedMod);

			}
		}
// Now apply passives
// 		for (auto& GameplayEffect : PassiveGameplayEffects)
// 		{
// 			if (!GameplayEffect.Key) continue;
// 
// 			FGameplayEffectContextHandle EffectContext = PlayerRef->GetAbilitySystemComponent()->MakeEffectContext();
// 			EffectContext.AddSourceObject(PlayerRef);
// 
// 			FGameplayEffectSpecHandle NewHandle = PlayerRef->GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect.Key, GameplayEffect.Value, EffectContext);
// 			if (NewHandle.IsValid())
// 			{
// 				FActiveGameplayEffectHandle ActiveGEHandle = PlayerRef->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), PlayerRef->GetAbilitySystemComponent());
// 			}
// 		}
	}

	bAbilitiesInitialized = true;
}

bool USoulModifierManager::AddOrRemoveGAOnEquipSlot(const FSoulEquipmentSlot& EquipSlot, bool RemoveGA)
{
	return false;
}
