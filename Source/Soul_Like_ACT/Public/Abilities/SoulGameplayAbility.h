// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility.h"
#include "Abilities/SoulAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "SoulGameplayAbility.generated.h"

/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS()
	class SOUL_LIKE_ACT_API USoulGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	USoulGameplayAbility()
	{
	}

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FSoulGameplayEffectContainer> EffectContainerMap;


	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag,
	                                                                 const FGameplayEventData& EventData,
	                                                                 int32 OverrideGameplayLevel = -1);
protected:
	/** Make gameplay effect container spec to be applied later, using the passed in container */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSoulGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(
		const FSoulGameplayEffectContainer& Container, const FGameplayEventData& EventData,
		int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FSoulGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag,
	                                                                 const FGameplayEventData& EventData,
	                                                                 int32 OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(
		const FSoulGameplayEffectContainerSpec& ContainerSpec);
};


/**
 * Subclass of ability blueprint type with game-specific data
 * This class uses GameplayEffectContainers to allow easier execution of gameplay effects based on a triggering tag
 * Most games will need to implement a subclass to support their game-specific code
 */
UCLASS()
	class SOUL_LIKE_ACT_API USoulModifierGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:


public:
	USoulModifierGameplayAbility()
		: DisplayName(FText::FromString("Invalid Modifier"))
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Default)
	TSubclassOf<UGameplayEffect> EffectCollection;
};

UCLASS()
	class USoulPrimaryStatusGameplayAbility : public USoulModifierGameplayAbility
{
	GENERATED_BODY()

public:
	USoulPrimaryStatusGameplayAbility()
	{
	}
};
