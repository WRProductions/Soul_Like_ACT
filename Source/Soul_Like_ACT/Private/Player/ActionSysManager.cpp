// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/ActionSysManager.h"
#include "Player/Soul_Like_ACTCharacter.h"
#include "Types/DA_PlayerAnimSet.h"
#include "Animation/AnimInstance.h"
#include "Types/DA_ComboMontage.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "TimerManager.h"
#include "SoulGameplayAbility.h"
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
 		return TryEnableJumpSection();
	
	else
		return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
			FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Melee"}, true) },
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
			UE_LOG(LogTemp, Warning, TEXT("Current GA: %s"), *localGA->GetName());
		}
		
		return 0;
	}

	return PlayerRef->AbilitySystemComponent->TryActivateAbilitiesByTag(
		FGameplayTagContainer{ FGameplayTag::RequestGameplayTag(FName{"Ability.Skill.Evade"}, true) },
		true);
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
	else
	{
		//TODO: use other ability
		UE_LOG(LogTemp, Warning, TEXT("Current Montage: %s, Jump Montage: %s"),
			*(CurrentMontage->GetName()),
			*(JumpMontage->GetName()));
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

bool UActionSysManager::ActivateAbilitiesWithWeapon(bool bAllowRemoteActivation) const
{
	if (PlayerRef->AbilitySystemComponent)
		return PlayerRef->AbilitySystemComponent->TryActivateAbilityByClass(PlayerRef->AbilityArray[0], true);
	
	return false;
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

bool UActionSysManager::bIsUsingAbility() const
{
	UAbilitySystemComponent *localComp = PlayerRef->GetAbilitySystemComponent();
	
	if (!localComp) return 0;

	return (localComp->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName{ "Ability.Skill" }, true)));
}

bool UActionSysManager::bCanUseAnyGA() const
{
	return (PlayerRef->GetHealth() > 0.f &&
		!UGameplayStatics::IsGamePaused(GetWorld()) &&
		!bIsUsingAbility());
}