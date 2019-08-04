// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SoulAttributeSet.generated.h"

UENUM(BlueprintType)
enum class EParryResult : uint8
{
	Unguard,
	Normal,
	Perfect,
	Failed
};

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** This holds all of the attributes used by abilities, it instantiates a copy of this on every character */
UCLASS()
class SOUL_LIKE_ACT_API USoulAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	USoulAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Health)

	/** MaxHealth is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxHealth)

	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData Posture;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Posture)
		
	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData MaxPosture;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxPosture)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData PostureStrength;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureStrength)

	UPROPERTY(BlueprintReadOnly, Category = "Defense")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData MeleeRange;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MeleeRange)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Leech;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData PostureCrumble;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureCrumble)

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData CriticalStrike;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData CriticalMulti;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti)

	UPROPERTY(BlueprintReadOnly, Category = "Output", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "Output", meta = (HideFromLevelInfos))
	FGameplayAttributeData PostureDamage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, PostureDamage)


protected:
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	void GetHitEventFromTagContainer(const FGameplayTagContainer& InTagContainer, EParryResult &OutParryResult, bool &IsCritical, bool &IsStun);
};
