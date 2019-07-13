// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Abilities/SoulGameplayAbility.h"
#include "SoulModifierManager.generated.h"

class ASoulCharacterBase;
struct FSoulItemData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API USoulModifierManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoulModifierManager();

public:
	UFUNCTION(BlueprintCallable, Category = Player)
	static USoulModifierManager* GetSoulModifierManger(class AActor* Owner);

protected:
	/** If true we have initialized our abilities */
	UPROPERTY()
	bool bAbilitiesInitialized;

	/** 
	* These GAs are created when Beginplay
	* DefaultActive are bound to any inputs, e.g. Action/Spells
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	TMap<TSubclassOf<USoulGameplayAbility>,  int32> DefaultActiveAbilities;
	/**
	* These GAs are created when Beginplay
	* DefaultModifers can only be triggered or modify attributes, thus permanent.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	TMap<TSubclassOf<USoulModifierGameplayAbility>, int32> DefaultModifiers;

	/**
	 * GameAbilitySpecHandles that for future usages
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TArray<FGameplayAbilitySpecHandle> GrantedActiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TArray<FGameplayAbilitySpecHandle> GrantedModifierAbilities;

	/** Map of slot to ability granted by that slot. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TArray<FGameplayAbilitySpecHandle> SlottedAbilities;

	void AddStartupGameplayAbilities();

	/**
	 * Update the modifiers from the equipment to player. 
	 * bIsAdded is false represents the gear is unequipped
	 * Warning: The function shall only be called in UInventoryManager. Don't call it manually
	 */
	bool UpdateModifier(const FSoulItemData& InputItemData, bool bIsAdded = true);
	
public:
	UFUNCTION(BlueprintCallable)
	bool FindActiveAbilitySpecHandle(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, FGameplayAbilitySpecHandle& OutGASpecHandle);

	UFUNCTION(BlueprintCallable)
	static USoulModifierManager* GetModifierManager(AActor* SourceActor);

	friend class ASoulCharacterBase;
	friend class UInventoryManager;
	friend class UMobActionManager;
};
