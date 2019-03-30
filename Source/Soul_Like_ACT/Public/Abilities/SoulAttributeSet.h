// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SoulAttributeSet.generated.h"

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
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Stamina)
		
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MaxStamina)

	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, AttackSpeed)

	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Leech;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Leech)

	/** Base Damage is divided by DefensePower to get actual damage done, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, DefensePower)

	/** Base Damage is divided by DefensePower to get actual damage done, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Tenacity;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Tenacity)

	/** MoveSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, MoveSpeed)

	/** MoveSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData CriticalStrike;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalStrike)

	/** MoveSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed")
	FGameplayAttributeData CriticalMulti;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, CriticalMulti)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Output", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, Damage)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Output", meta = (HideFromLevelInfos))
	FGameplayAttributeData IsCriticalDamageTaken;
	ATTRIBUTE_ACCESSORS(USoulAttributeSet, IsCriticalDamageTaken)


protected:
	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

};
