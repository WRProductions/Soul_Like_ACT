// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulAttributeSet.h"
#include "GameplayEffect.h"
#include "SoulCharacterBase.h"
#include "GameplayEffectExtension.h"


USoulAttributeSet::USoulAttributeSet()
	: Health(1.f)
	, MaxHealth(1.f)
	, Stamina(1.f)
	, MaxStamina(1.f)
	, AttackPower(0.0f)
	, AttackSpeed(0.0f)
	, Leech(0.0f)
	, DefensePower(0.0f)
	, Tenacity(0.0f)
	, MoveSpeed(400.0f)
	, CriticalStrike(5.0f)
	, CriticalMulti(50.f)
	, Damage(0.0f)
{
}


void USoulAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}


void USoulAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
// 	else if (Attribute == GetMaxStaminaAttribute())
// 	{
// 		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
// 	}
}


void USoulAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ASoulCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ASoulCharacterBase>(TargetActor);
	}
	if (Data.EvaluatedData.Attribute == GetIsCriticalDamageTakenAttribute())
	{
		return;
	}
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		ASoulCharacterBase* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ASoulCharacterBase>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		const bool bIsCriticalDamageTaken = GetIsCriticalDamageTaken() > 0.f ? true : false;
		SetIsCriticalDamageTaken(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, bIsCriticalDamageTaken, HitResult, SourceTags, SourceCharacter, SourceActor);

				// Call for all health changes
				TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			// Call for all health changes
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));

		if (TargetCharacter)
			TargetCharacter->HandleStaminaChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetLeechAttribute())
	{
		SetLeech(FMath::Clamp(GetLeech (), 0.0f, 100.0f));

		if (TargetCharacter)
			TargetCharacter->HandleLeechChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute())
	{
		SetLeech(FMath::Clamp(GetAttackSpeed(), 0.0f, 999.0f));

		if (TargetCharacter)
			TargetCharacter->HandleAttackSpeedChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, 2000.0f));
		if (TargetCharacter)
		{
			// Call for all movespeed changes
			TargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetTenacityAttribute())
	{
		SetTenacity(FMath::Clamp(GetTenacity(), 0.0f, 9999.0f));

		if (TargetCharacter)
			TargetCharacter->HandleTenacityChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetAttackPowerAttribute())
	{
		SetAttackPower(FMath::Clamp(GetAttackPower(), 0.0f, 9999.0f));
		if (TargetCharacter)
			TargetCharacter->HandleAttackPowerChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetDefensePowerAttribute())
	{
		SetDefensePower(FMath::Clamp(GetDefensePower(), 0.0f, 9999.0f));
		if (TargetCharacter)
			TargetCharacter->HandleDefensePowerChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetCriticalStrikeAttribute())
	{
	SetDefensePower(FMath::Clamp(GetCriticalStrike(), -999.f, 999.f));
	if (TargetCharacter)
		TargetCharacter->HandleCriticalStrikeChanged(DeltaValue, SourceTags);
	}
	else if (Data.EvaluatedData.Attribute == GetCriticalMultiAttribute())
	{
	SetDefensePower(FMath::Clamp(GetCriticalMulti(), -999.f, 999.f));
	if (TargetCharacter)
		TargetCharacter->HandleCriticalMultiChanged(DeltaValue, SourceTags);
	}
	if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		if (TargetCharacter)
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
	}	
	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		if (TargetCharacter)
			TargetCharacter->HandleStaminaChanged(DeltaValue, SourceTags);
	}
}