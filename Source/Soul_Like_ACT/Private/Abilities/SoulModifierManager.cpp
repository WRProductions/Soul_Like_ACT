// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulModifierManager.h"
#include "SoulCharacterBase.h"
#include "Types/SoulItemTypes.h"
#include "Animation/AnimInstance.h"

USoulModifierManager::USoulModifierManager()
{
	PrimaryComponentTick.bCanEverTick = false;
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

bool USoulModifierManager::FindActiveAbilitySpecHandle(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, FGameplayAbilitySpecHandle& OutGASpecHandle)
{
	USoulAbilitySystemComponent* AbilityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());

	if (!AbilityComponent)
	{
		LOG_FUNC_ERROR("Can't access ModifierManager");
		return false;
	}

	for (FGameplayAbilitySpecHandle& GASpecHandle : GrantedActiveAbilities)
	{
		FGameplayAbilitySpec* TempSpec = AbilityComponent->FindAbilitySpecFromHandle(GASpecHandle);
		if (TempSpec->Ability->GetClass() == ActiveAbilityClass)
		{
			OutGASpecHandle = GASpecHandle;
			return true;
		}
	}

	LOG_FUNC_ERROR("Cannot find GASpecHandle");
	return false;
}

USoulModifierManager* USoulModifierManager::GetModifierManager(AActor* SourceActor)
{
	ASoulCharacterBase* SoulChar = Cast<ASoulCharacterBase>(SourceActor);
	
	if (!SoulChar)
	{
		return nullptr;
	}
	return SoulChar->GetModifierManager();
}

void USoulModifierManager::AddStartupGameplayAbilities()
{
	USoulAbilitySystemComponent* AbilitySysComp = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
	check(AbilitySysComp);

	AbilitySysComp->InitAbilityActorInfo(GetOwner(), GetOwner());

	if (!(GetOwner()->Role == ROLE_Authority))
	{
		return;
	}
	for (TPair<TSubclassOf<USoulGameplayAbility>, int32>& TempActiveAbility : DefaultActiveAbilities)
	{
		if (TempActiveAbility.Key)
		{
			auto GrantedAbilitySpecHandle = AbilitySysComp->GiveAbility(
				FGameplayAbilitySpec
				(
					TempActiveAbility.Key
					, TempActiveAbility.Value
					, INDEX_NONE
					, GetOwner()
				)
			);

			GrantedActiveAbilities.Add(GrantedAbilitySpecHandle);
		}
	}

	for (TPair<TSubclassOf<USoulModifierGameplayAbility>, int32>& TempModifier : DefaultModifiers)
	{
		if (TempModifier.Key)
		{
			auto LocalGrantedMod = AbilitySysComp->GiveAbility(
				FGameplayAbilitySpec
				(
					TempModifier.Key
					, TempModifier.Value
					, INDEX_NONE
					, GetOwner()
				)
			);

			GrantedModifierAbilities.Add(LocalGrantedMod);

			AbilitySysComp->TryActivateAbility(LocalGrantedMod, true);
		}
	}

	bAbilitiesInitialized = true;
}

bool USoulModifierManager::UpdateModifier(const FSoulItemData& InputItemData, bool bIsAdded /*= true*/)
{
	USoulAbilitySystemComponent* AbilityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());

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

			auto* CurrGASpec = AbilityComponent->FindAbilitySpecFromHandle(SlottedAbilities[i]);

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
				AbilityComponent->ClearAbility(SlottedAbilities[i]);

				//remove the spec handle from the SlottedAbilities if the GA is completely removed
				if (CurrGALevel <= 0)
				{
					SlottedAbilities.RemoveAt(i);
				}
				//Refresh the GA
				else
				{
					//Assign a new spec handle to it
					FGameplayAbilitySpecHandle LocalGrantedMod = AbilityComponent->GiveAbility(
						FGameplayAbilitySpec(ItemModifier.Key, CurrGALevel, INDEX_NONE, GetOwner()));

					AbilityComponent->TryActivateAbility(LocalGrantedMod, true);

					SlottedAbilities.Add(LocalGrantedMod);
				}
				break;
			}
		}

		if (!bIsModNew)
		{
			FGameplayAbilitySpecHandle NewGrantedMod = AbilityComponent->GiveAbility(
				FGameplayAbilitySpec(ItemModifier.Key, ItemModifier.Value, INDEX_NONE, GetOwner()));

			AbilityComponent->TryActivateAbility(NewGrantedMod, true);

			SlottedAbilities.Add(NewGrantedMod);

			LOG_FUNC_NORMAL("New Mod: " + (ItemModifier.Key.GetDefaultObject()->GetName()));
		}
	}
	return true;
}