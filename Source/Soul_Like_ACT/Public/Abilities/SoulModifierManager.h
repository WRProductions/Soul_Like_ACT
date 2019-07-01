// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SoulItemTypes.h"
#include "Components/ActorComponent.h"
#include "SoulModifierManager.generated.h"

struct FGameplayAbilitySpecHandle;
struct FGameplayAbilitySpec;
class USoulActiveAbility;
class UGameplayAbility;
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
	// Called when the game starts
	virtual void BeginPlay() override;

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
	bool UpdateModifierToPlayer(const FSoulItemData& InputItemData, bool bIsAdded = true);

	UFUNCTION(BlueprintCallable)
	class USoulAbilitySystemComponent* GetOwnerGameplayAbilityComponent();

	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle& InSpecHandle);

	static FGameplayAbilitySpecHandle GetActiveAbilitySpecHandleFromCharacter(ASoulCharacterBase *InCharacter, TSubclassOf<USoulActiveAbility> InActiveAbility);

	friend class ASoulCharacterBase;
	friend class UInventoryManager;
	friend class UMobActionManager;
};
