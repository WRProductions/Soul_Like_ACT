// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulModifierManager.h"
#include "SoulCharacterBase.h"
#include "GameFramework/Actor.h"
#include "Types/SoulItemTypes.h"
#include "Animation/AnimInstance.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulAbilitySystemComponent.h"

USoulModifierManager::USoulModifierManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USoulModifierManager::BeginPlay()
{
	Super::BeginPlay();
}

USoulModifierManager* USoulModifierManager::GetSoulModifierManger(class AActor* Owner)
{
	ASoulCharacterBase* OwnerChar = Cast<ASoulCharacterBase>(Owner);
	if (OwnerChar && OwnerChar->GetModifierManager())
	{
		return (OwnerChar->GetModifierManager());
	}

	return nullptr;
}

class USoulAbilitySystemComponent* USoulModifierManager::GetOwnerGameplayAbilityComponent()
{
	return USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
}

FGameplayAbilitySpec* USoulModifierManager::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle& InSpecHandle)
{
	return (GetOwnerGameplayAbilityComponent()->FindAbilitySpecFromHandle(InSpecHandle));
}

FGameplayAbilitySpecHandle USoulModifierManager::GetActiveAbilitySpecHandleFromCharacter(ASoulCharacterBase* InCharacter, TSubclassOf<USoulActiveAbility> InActiveAbility)
{
	USoulModifierManager* LocalModManager = InCharacter->GetModifierManager();
	USoulAbilitySystemComponent* OwnerAbilityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(InCharacter);

	if (!LocalModManager) return FGameplayAbilitySpecHandle();

	for (auto& LocalSpecHandle : LocalModManager->GrantedActiveAbilities)
	{
		FGameplayAbilitySpec *LocalGASpec = OwnerAbilityComponent->FindAbilitySpecFromHandle(LocalSpecHandle);
		if (LocalGASpec && LocalGASpec->Ability->StaticClass() == InActiveAbility)
		{
			return LocalSpecHandle;
		}
	}

	return FGameplayAbilitySpecHandle();
}

void USoulModifierManager::AddStartupGameplayAbilities()
{
	check(GetOwnerGameplayAbilityComponent());

	GetOwnerGameplayAbilityComponent()->InitAbilityActorInfo(GetOwner(), GetOwner());

	if (!(GetOwner()->Role == ROLE_Authority))
	{
		return;
	}
	for (TPair<TSubclassOf<USoulGameplayAbility>, int32>& TempActiveAbility : DefaultActiveAbilities)
	{
		if (TempActiveAbility.Key)
		{
			FGameplayAbilitySpecHandle LocalGrantedActiveGA = GetOwnerGameplayAbilityComponent()->GiveAbility(
				FGameplayAbilitySpec(
					TempActiveAbility.Key
					, TempActiveAbility.Value
					, INDEX_NONE
					, GetOwner())
			);

			GrantedActiveAbilities.Add(LocalGrantedActiveGA);
		}
	}

	for (TPair<TSubclassOf<USoulModifierGameplayAbility>, int32>& TempModifier : DefaultModifiers)
	{
		if (TempModifier.Key)
		{
			auto LocalGrantedMod = GetOwnerGameplayAbilityComponent()->GiveAbility(
				FGameplayAbilitySpec(
					TempModifier.Key
					, TempModifier.Value
					, INDEX_NONE
					, GetOwner())
			);

			GrantedModifierAbilities.Add(LocalGrantedMod);

			GetOwnerGameplayAbilityComponent()->TryActivateAbility(LocalGrantedMod, true);
		}
	}

	bAbilitiesInitialized = true;
}

bool USoulModifierManager::UpdateModifierToPlayer(const FSoulItemData& InputItemData, bool bIsAdded /*= true*/)
{
// 	if (PlayerRef->Role != ROLE_Authority)
// 	{
// 		return false;
// 	}
	if (!InputItemData.IsValid())
	{
		LOG_FUNC_ERROR("Invalid InputItemData");
		return false;
	}
	for (TPair<TSubclassOf<USoulModifierGameplayAbility>, int32>& ItemModifier : InputItemData.ItemBase->Modifiers)
	{
		bool bIsModNew = false;

		//check whether there is a same modifier already applied to the character
		for (int i = 0; i < SlottedAbilities.Num(); ++i)
		{

			auto* CurrGASpec = FindAbilitySpecFromHandle(SlottedAbilities[i]);

			UE_LOG(LogTemp, Warning, TEXT("%s vs %s"), *((ItemModifier.Key.GetDefaultObject())->GetName()), *CurrGASpec->Ability->GetName());

			//update the modifier if there is a same type
			if (SlottedAbilities[i].IsValid() && CurrGASpec->Ability->GetClass() == ItemModifier.Key)
			{
				bIsModNew = true;

				USoulModifierGameplayAbility* LocalModifier = Cast<USoulModifierGameplayAbility>(CurrGASpec->Ability);
				int32 LocalMaxLevel = LocalModifier->MaxLevel;
				int32 CurrGALevel = LocalModifier->GetAbilityLevel();

				//bIsAdded == true -> Add the modifier to the player
				if (bIsAdded)
				{
					if (CurrGALevel < LocalMaxLevel)
						CurrGALevel = FMath::Clamp(CurrGALevel + ItemModifier.Value, 1, LocalMaxLevel);
					else
						break;
				}
				//bIsAdded = false -> remove the modifier from the player
				else if (!bIsAdded) 
				{
					CurrGALevel = FMath::Clamp(CurrGALevel - ItemModifier.Value, 0, LocalMaxLevel);
				}
				else
				{
					break;
				}
				
				//Remove the old modifier
				//GetOwnerGameplayAbilityComponent()->CancelAbilityHandle(SlottedAbilities[i]);
				GetOwnerGameplayAbilityComponent()->ClearAbility(SlottedAbilities[i]);

				//remove the spec handle from the SlottedAbilities if the GA is completely removed
				if (CurrGALevel <= 0)
				{
					SlottedAbilities.RemoveAt(i);
				}
				//Refresh the GA
				else
				{
					//Assign a new spec handle to it
					FGameplayAbilitySpecHandle LocalGrantedMod = GetOwnerGameplayAbilityComponent()->GiveAbility(
						FGameplayAbilitySpec(ItemModifier.Key, CurrGALevel, INDEX_NONE, GetOwner()));

					GetOwnerGameplayAbilityComponent()->TryActivateAbility(LocalGrantedMod, true);

					SlottedAbilities.Add(LocalGrantedMod);
				}
				break;
			}
		}

		if (!bIsModNew)
		{
			FGameplayAbilitySpecHandle NewGrantedMod = GetOwnerGameplayAbilityComponent()->GiveAbility(
				FGameplayAbilitySpec(ItemModifier.Key, ItemModifier.Value, INDEX_NONE, GetOwner()));

			GetOwnerGameplayAbilityComponent()->TryActivateAbility(NewGrantedMod, true);

			SlottedAbilities.Add(NewGrantedMod);

			LOG_FUNC_NORMAL("New Mod: " + (ItemModifier.Key.GetDefaultObject()->GetName()));
		}
	}
	return true;
}