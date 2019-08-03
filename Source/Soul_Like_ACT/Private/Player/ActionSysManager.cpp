// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ActionSysManager.h"
#include "Player/Soul_Like_ACTCharacter.h"
#include "Animation/AnimInstance.h"
#include "Abilities/SoulModifierManager.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "TimerManager.h"
#include "SoulGameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UActionSysManager::UActionSysManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionSysManager::BeginPlay()
{
	Super::BeginPlay();

	check(PlayerRef);
}

void UActionSysManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UActionSysManager::SetNewStance(EKatanaStanceType InStance)
{
	PlayerRef->AbilitySystemComponent->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Stance.Katana.MidStance", true));
	PlayerRef->AbilitySystemComponent->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Stance.Katana.Idle", true));
	PlayerRef->AbilitySystemComponent->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Stance.Katana.Battao", true));
	PlayerRef->AbilitySystemComponent->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Stance.Katana.HeavyStance", true));
	PlayerRef->AbilitySystemComponent->RemoveGameplayTag(FGameplayTag::RequestGameplayTag("Stance.Katana.Guard", true));

	if (InStance == EKatanaStanceType::Idle)
	{
		PlayerRef->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("Stance.Katana.Idle", true), 1);
	}
	if (InStance == EKatanaStanceType::Battao)
	{
		PlayerRef->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("Stance.Katana.Battao", true), 1);
	}
	if (InStance == EKatanaStanceType::MidStance)
	{
		PlayerRef->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("Stance.Katana.MidStance", true), 1);
	}
	if (InStance == EKatanaStanceType::HeavyStance)
	{
		PlayerRef->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("Stance.Katana.HeavyStance", true), 1);
	}
	if (InStance == EKatanaStanceType::Guard)
	{
		PlayerRef->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("Stance.Katana.Guard", true), 1);
	}
	CurrentStance = InStance;

	return true;
}

bool UActionSysManager::CanChangeStance(EKatanaStanceType InStance)
{
	if (InStance == EKatanaStanceType::Idle)
	{
		return CurrentStance == EKatanaStanceType::Battao;
	}
	else if (InStance == EKatanaStanceType::Battao)
	{
		return CurrentStance == EKatanaStanceType::Idle || CurrentStance == EKatanaStanceType::MidStance;
	}
	else if (InStance == EKatanaStanceType::MidStance)
	{
		return (CurrentStance == EKatanaStanceType::Battao
			|| CurrentStance == EKatanaStanceType::HeavyStance
			|| CurrentStance == EKatanaStanceType::Idle);
	}
	else if (InStance == EKatanaStanceType::HeavyStance)
	{
		return CurrentStance == EKatanaStanceType::MidStance;
	}

	return false;
}

bool UActionSysManager::DoMeleeAttack()
{
	if (!CanUseAnyGA() || IsUsingParry())
		return 0;

 	if (IsUsingMelee())
 		return TryEnableJumpSection();
	
	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee.Normal"}, true) },
		true);
}

bool UActionSysManager::DoSpecialMeleeAttack()
{
	if (!CanUseAnyGA())
		return 0;

	if (IsUsingMelee())
		return TryEnableJumpSection();

	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee.ParryBreak"}, true) },
		true);

}

bool UActionSysManager::DoDodge()
{
	if (!CanUseAnyGA())
	{
		TArray<USoulGameplayAbility*> tempGAs;
		PlayerRef->AbilitySystemComponent->GetActiveAbilitiesWithTags(
			FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Skill"}, true) },
			tempGAs);

		for (USoulGameplayAbility * localGA : tempGAs)
		{
			LOG_FUNC_ERROR(localGA->GetName());
		}
		
		return 0;
	}

	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Skill.Evade"}, true) },
		true);
}

bool UActionSysManager::DoChangeStance(EKatanaStanceType InStance)
{
	//if(!CanChangeStance(InStance)) return false;

	if (!CanUseAnyGA() || IsUsingMelee() || IsUsingParry())
		return false;

	if (InStance == EKatanaStanceType::MidStance && CurrentStance == EKatanaStanceType::Idle)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_Unsheathe, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::Idle && CurrentStance == EKatanaStanceType::MidStance)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_Sheathe, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::HeavyStance && CurrentStance == EKatanaStanceType::MidStance)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_MidToHeavy, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::MidStance && CurrentStance == EKatanaStanceType::HeavyStance)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_HeavyToMid, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::Battao && CurrentStance == EKatanaStanceType::MidStance)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_MidToBatto, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::MidStance && CurrentStance == EKatanaStanceType::Battao)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_BattoToMid, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::Battao && CurrentStance == EKatanaStanceType::Idle)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_SheathedToBatto, 1, INDEX_NONE, GetOwner()));
	}
	else if (InStance == EKatanaStanceType::Idle && CurrentStance == EKatanaStanceType::Battao)
	{
		PlayerRef->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(
			FGameplayAbilitySpec(ActiveAbilitiesPreset->GA_BattoToSheathed, 1, INDEX_NONE, GetOwner()));
	}
	else
	{
		return false;
	}

	return true;
}

bool UActionSysManager::DoParry_Start()
{
	if (!CanUseAnyGA())
	{
		return false;
	}

	if (CurrentStance != EKatanaStanceType::Idle || CurrentStance != EKatanaStanceType::MidStance)
	{
		return false;
	}

	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Skill.Parry"}, true) },
		true);

}

bool UActionSysManager::DoParry_End()
{
	UAnimInstance* AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

	if (CurrentMontage)
	{
		AnimInstance->Montage_JumpToSection(FName("ParryEnd"), CurrentMontage);
		return true;
	}

	return false;
}

bool UActionSysManager::SetJumpSection(uint8 ComboSectionIndex, const UAnimMontage* InpMontage)
{
	bWillJumpSection = 0;
	bCanJumpSection = 1;

	if (InpMontage)
	{
		if (InpMontage->IsValidSectionIndex(ComboSectionIndex))
			CurrentComboSectionIndex = ComboSectionIndex;
		else
			CurrentComboSectionIndex = 0;

		MontageToPlay = const_cast<UAnimMontage*>(InpMontage);
		return true;
	}
	else
	{
		UAnimInstance* AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
			MontageToPlay = AnimInstance->GetCurrentActiveMontage();
		else
			return false;

		if (MontageToPlay && MontageToPlay->IsValidSectionIndex(CurrentComboSectionIndex + 1))
		{
			++CurrentComboSectionIndex;
			
			return true;
		}

		return false;
	}
}

bool UActionSysManager::JumpSectionForCombo()
{
	//FName CurrentSection = AnimInstance->Montage_GetCurrentSection(CurrentMontage);

	if (MontageToPlay && bCanJumpSection && bWillJumpSection)
	{
		UAnimInstance* AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
		
		if (AnimInstance)
			MontageToPlay = AnimInstance->GetCurrentActiveMontage();
		else
			return false;

		AnimInstance->Montage_JumpToSection(MontageToPlay->GetSectionName(CurrentComboSectionIndex), MontageToPlay);
//		UE_LOG(LogTemp, Warning, TEXT("Current Montage: %s"), *(CurrentMontage->GetName()));
	}

	bCanJumpSection = false;

	if (!bWillJumpSection) return false;

	bWillJumpSection = false;

	return true;
}

float UActionSysManager::PredictMovingDirection(ASoul_Like_ACTCharacter *CharacterRef)
{
	FVector PlayerVelocity = CharacterRef->PredictMovement();
	FRotator PlayerRotation = CharacterRef->GetActorRotation();
	if (!PlayerVelocity.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(PlayerRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedVel = PlayerVelocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedVel);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedVel);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}
	
	return 0.f;
}

FName UActionSysManager::Get4WaysStepDirection_GA(float PredictableMovingDirection)
{
	if (PredictableMovingDirection > -45.f && PredictableMovingDirection < 45.f)
		return "F";
	else if (PredictableMovingDirection <= -45.f && PredictableMovingDirection > -135.f)
		return "L";
	else if (PredictableMovingDirection < 135.f && PredictableMovingDirection >= 45.f)
		return "R";
	else
		return "B";
}

void UActionSysManager::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities)
{
	if (PlayerRef->AbilitySystemComponent)
		PlayerRef->AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
}

bool UActionSysManager::IsUsingMelee() const
{
	USoulAbilitySystemComponent *LocalComp = Cast<USoulAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent());

	if (!LocalComp) return 0;

	else
	{
		TArray<USoulGameplayAbility*> LocalAbilities;

		LocalComp->GetActiveAbilitiesWithTags(
			FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee"}, true) },
			LocalAbilities);

		return LocalAbilities.Num() > 0;
	}
}

bool UActionSysManager::IsUsingSkills() const
{
	UAbilitySystemComponent * MyAbilityComponent = PlayerRef->GetAbilitySystemComponent();
	
	return (MyAbilityComponent->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName{ "Ability.Skill" }, true)));
}

bool UActionSysManager::CanUseAnyGA() const
{
	return (
		PlayerRef->GetHealth() > 0.f &&
		!UGameplayStatics::IsGamePaused(GetWorld()) &&
		!IsUsingSkills()
	);
}

bool UActionSysManager::IsUsingParry() const
{
	UAbilitySystemComponent *MyAbilityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner(), false);

	return (MyAbilityComponent->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName{ "Ability.Skill.Parry" }, true)));
}
