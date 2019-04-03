// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulDamageExecution.h"
#include "Abilities/SoulAttributeSet.h"
#include "AbilitySystemComponent.h"

struct SoulDamageStatics
{
	//For Target
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Tenacity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(IsCriticalDamageTaken);
	
	//For Source
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrike);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalMulti);

	SoulDamageStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, DefensePower, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Tenacity, Target, false);
		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, AttackPower, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, CriticalStrike, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, CriticalMulti, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(USoulAttributeSet, IsCriticalDamageTaken, Source, true);
	}
};

static const SoulDamageStatics& DamageStatics()
{
	static SoulDamageStatics DmgStatics;
	return DmgStatics;
}


USoulDamageExecution::USoulDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalMultiDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().IsCriticalDamageTakenDef);

}

void USoulDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = (Damage + AP) * (bCritical ? (1 + CriticalMulti ; 1) - DP
	// --------------------------------------

	float DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	float CriticalStrike = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeDef, EvaluationParameters, CriticalStrike);

	float CriticalMulti = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalMultiDef, EvaluationParameters, CriticalMulti);

	//TODO implement leech
	float Leech = 0.f;

	float IsCriticaled = 0.f;

	float DamageDone = Damage + AttackPower;

	if (DamageDone > 0.f)
	{
		//Critical
		if (FMath::RandRange(0.f, 100.f) - CriticalStrike >= 0.f)
		{
			++IsCriticaled;
			DamageDone *= (1 + CriticalMulti / 100.f);
		}
	
		DamageDone -= DefensePower;
		
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().IsCriticalDamageTakenProperty, EGameplayModOp::Override, IsCriticaled));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}

