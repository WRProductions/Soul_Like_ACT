// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SoulItemTypes.h"
#include "Components/ActorComponent.h"
#include "SoulModifierManager.generated.h"

USTRUCT(BlueprintType)
struct FEquipGA_Handles
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = Default)
	TArray<FGameplayAbilitySpecHandle> GA_Handles;
};

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

	/** Defined in SoulPlayerBase */
	class ASoulCharacterBase* PlayerRef;

	UFUNCTION(BlueprintCallable)
	class USoulAbilitySystemComponent* GetSoulGAComponent();

protected:
	/** If true we have initialized our abilities */
	UPROPERTY()
	bool bAbilitiesInitialized;

	/** 
	* These GAs are created when Beginplay
	* DefaultActive are bound to any inputs, e.g. Action/Spells
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	TMap<TSubclassOf<USoulGameplayAbility>, uint8> DefaultActiveAbilities;
	/**
	* These GAs are created when Beginplay
	* DefaultModifers can only be triggered or modify attributes, thus permanent.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	TMap<TSubclassOf<USoulModifierGameplayAbility>, uint8> DefaultModifiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TArray<FGameplayAbilitySpecHandle> GrantedDefaultActiveGAs;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TArray<FGameplayAbilitySpecHandle> GrantedDefaultModifiers;

	/** Map of slot to ability granted by that slot. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Default)
	TMap<FSoulEquipmentSlot, FEquipGA_Handles> SlottedAbilities;

	void AddStartupGameplayAbilities();

	bool AddOrRemoveGAOnEquipSlot(const FSoulEquipmentSlot& EquipSlot, bool RemoveGA);



	friend ASoulCharacterBase;
	friend class UInventoryManager;
};
