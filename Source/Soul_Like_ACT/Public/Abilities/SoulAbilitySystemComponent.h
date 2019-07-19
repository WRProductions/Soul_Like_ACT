// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "AbilitySystemComponent.h"
#include "SoulAbilitySystemComponent.generated.h"

class USoulGameplayAbility;
/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	// Constructors and overrides
	USoulAbilitySystemComponent() {}

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<USoulGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	UFUNCTION(BlueprintCallable)
	bool TryActivateAbility_Soul(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable)
	void TryActiveAbilityOnce(TSubclassOf<USoulGameplayAbility> InGameplayAbility, int32 Level);

	/** Version of function in AbilitySystemGlobals that returns correct type */
	UFUNCTION(BlueprintCallable)
	static USoulAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	UFUNCTION(BlueprintCallable)
	static FActiveGameplayEffectHandle ApplyGE_ToSelf(const AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffect, const int32 AbilityLevel);

	UFUNCTION(BlueprintCallable)
	static FActiveGameplayEffectHandle ApplyGE_ToTarget(const AActor* FromActor, const AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffect, const int32 AbilityLevel);

	UFUNCTION(BlueprintCallable, Category = Ability, Meta = (Tooltip = "This function can be used to create the event data through weapon on hit"))
	static void CreateEventData(const AActor* Target, const AActor* Source, const FHitResult& InpHitResult, const FGameplayTag EventTag, const float EventMagnitude, FGameplayEventData& OutpEventData);

	UFUNCTION(BlueprintCallable)
	static bool GetMontageFromActiveAbility(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, UAnimMontage*& AnimMontage);

	UFUNCTION(BlueprintCallable)
	static float GetMontageSectionLength(UAnimMontage* AnimMontage, FName SectionName);

	void BindOnGameplayAbilityEndFromActiveSpecHandle(
		const FGameplayAbilitySpecHandle& GameplaySpecHandle
		, const FOnGameplayAbilityEnded::FDelegate& OnGameplayAbilityEndedDelegate
	);

	//Use this to remove tag like Ability.Melee
	//So we can force to use Evade while attacking
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag(const FGameplayTag& GameplayTag)
	{
		SetTagMapCount(GameplayTag, 0);
	}
};