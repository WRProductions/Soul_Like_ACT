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

bool UActionSysManager::DoMeleeAttack()
{
	if (!bCanUseAnyGA() || bIsUsingParry())
		return 0;

 	if (bIsUsingMelee())
 		return TryEnableJumpSection();
	
	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee.Normal"}, true) },
		true);
}

bool UActionSysManager::DoSpecialMeleeAttack()
{
	if (!bCanUseAnyGA())
		return 0;

	if (bIsUsingMelee())
		return TryEnableJumpSection();

	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee.ParryBreak"}, true) },
		true);

}

bool UActionSysManager::DoDodge()
{
	if (!bCanUseAnyGA())
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

bool UActionSysManager::DoParry_Start()
{
	if (!bCanUseAnyGA())
	{
		TArray<USoulGameplayAbility*> tempGAs;
		PlayerRef->AbilitySystemComponent->GetActiveAbilitiesWithTags(
			FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Skill"}, true) },
			tempGAs);

		for (USoulGameplayAbility* localGA : tempGAs)
		{
			LOG_FUNC_ERROR(localGA->GetName());
		}

		return 0;
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

bool UActionSysManager::SetJumpSection(const FName InpComboScetionName, UAnimMontage *InpMontage)
{
	bWillJumpSection = 0;
	bCanJumpSection = 1;

	JumpSectionName = InpComboScetionName;
	JumpMontage = InpMontage;

	return true;
}

bool UActionSysManager::JumpSectionForCombo()
{
	bCanJumpSection = false;

	if (!bWillJumpSection) return false;

	bWillJumpSection = false;

	UAnimInstance *AnimInstance = PlayerRef->GetMesh()->GetAnimInstance();
	UAnimMontage *CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	//FName CurrentSection = AnimInstance->Montage_GetCurrentSection(CurrentMontage);
	
	if (!JumpMontage || JumpMontage == CurrentMontage)
	{
		FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentMontage);
		
		AnimInstance->Montage_JumpToSection(JumpSectionName, CurrentMontage);

		UE_LOG(LogTemp, Warning, TEXT("Current Montage: %s"),
			*(CurrentMontage->GetName()));
	}
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

bool UActionSysManager::bIsUsingMelee() const
{
	USoulAbilitySystemComponent *LocalComp = Cast<USoulAbilitySystemComponent>(PlayerRef->GetAbilitySystemComponent());

	if (!LocalComp) return 0;

	else
	{
		TArray<class USoulGameplayAbility*> LocalAbilities;

		LocalComp->GetActiveAbilitiesWithTags(
			FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee"}, true) },
			LocalAbilities);

		return LocalAbilities.Num() > 0;
	}
}

bool UActionSysManager::bIsUsingSkills() const
{
	UAbilitySystemComponent * MyAbilityComponent = PlayerRef->GetAbilitySystemComponent();
	
	return (MyAbilityComponent->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName{ "Ability.Skill" }, true)));
}

bool UActionSysManager::bCanUseAnyGA() const
{
	return (PlayerRef->GetHealth() > 0.f &&
		!UGameplayStatics::IsGamePaused(GetWorld()) &&
		!bIsUsingSkills());
}

bool UActionSysManager::bIsUsingParry() const
{
	UAbilitySystemComponent *MyAbilityComponent = USoulAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner(), false);

	return (MyAbilityComponent->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName{ "Ability.Skill.Parry" }, true)));
}
