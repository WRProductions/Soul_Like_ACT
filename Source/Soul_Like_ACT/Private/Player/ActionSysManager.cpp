// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ActionSysManager.h"
#include "Player/Soul_Like_ACTCharacter.h"
#include "Types/DA_PlayerAnimSet.h"
#include "Animation/AnimInstance.h"
#include "Types/DA_ComboMontage.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UActionSysManager::UActionSysManager()
{
	PrimaryComponentTick.bCanEverTick = 0;
}


void UActionSysManager::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerRef);
}

bool UActionSysManager::DoMeleeAttack()
{
	if (!bCanUseAnyGA())
		return 0;

	if (bIsUsingMelee())
		return JumpSectionForCombo();
	else
		return ActivateAbilitiesWithWeapon(true);
}

bool UActionSysManager::SetJumpSection(const FName InpComboScetionName, UAnimMontage *InpMontage)
{
	bCanJumpSection = 1;

	JumpSectionName = InpComboScetionName;
	JumpMontage = InpMontage;

	return true;
}

bool UActionSysManager::JumpSectionForCombo()
{
	if (!bCanJumpSection) return false;

	UAnimInstance *AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
	UAnimMontage *CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	
	if (!JumpMontage || JumpMontage == CurrentMontage)
	{
		FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentMontage);
		
		AnimInstance->Montage_SetNextSection(CurrentSectionName, JumpSectionName, CurrentMontage);
		
		return true;
	}
	else
	{
		//TODO: use other ability
		return true;
	}
	return false;
}

bool UActionSysManager::ActivateAbilitiesWithWeapon(bool bAllowRemoteActivation) const
{
	return Cast<USoulAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent())->TryActivateAbility(PlayerRef->TempGAHandle, true);
}

void UActionSysManager::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities)
{
	if (PlayerRef->AbilitySystemComponent)
	{
		PlayerRef->AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool UActionSysManager::bIsUsingMelee() const
{
	USoulAbilitySystemComponent *LocalComp = Cast<USoulAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent());

	if (!LocalComp) return 0;
	else
	{
		TArray<class USoulGameplayAbility*> LocalAbilities;

		LocalComp->GetActiveAbilitiesWithTags
		(
			FGameplayTagContainer
			{
				FGameplayTag::RequestGameplayTag(FName{"Ability.Melee"}, true)
			},
			LocalAbilities
		);

		return LocalAbilities.Num() > 0;
	}
}

bool UActionSysManager::bIsUsingAbility() const
{
	USoulAbilitySystemComponent *LocalComp = Cast<USoulAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent());
	
	if (!LocalComp) return 0;
	else
	{
		TArray<class USoulGameplayAbility*> LocalAbilities;
		
		LocalComp->GetActiveAbilitiesWithTags
		(
			FGameplayTagContainer
			{
				FGameplayTag::RequestGameplayTag(FName{"Ability.Skill"}, true)
			},
			LocalAbilities
		);

		return LocalAbilities.Num() > 0;
	}
}

bool UActionSysManager::bCanUseAnyGA() const
{
	return (PlayerRef->GetHealth() > 0.f &&
		!UGameplayStatics::IsGamePaused(GetWorld()) &&
		!bIsUsingAbility());
}


