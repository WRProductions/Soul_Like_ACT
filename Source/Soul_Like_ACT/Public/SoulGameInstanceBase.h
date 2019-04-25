// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Types/SoulItemTypes.h"
#include "SoulGameInstanceBase.generated.h"

class USoulItem;
class USoulSaveGame;

/**
 * Base class for GameInstance, should be blueprinted
 * Most games will need to make a game-specific subclass of GameInstance
 * Once you make a blueprint subclass of your native subclass you will want to set it to be the default in project settings
 */
UCLASS()
class SOUL_LIKE_ACT_API USoulGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	// Constructor
	USoulGameInstanceBase();

	/** List of inventory items to add to new players */
 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
 		TArray<FPrimaryAssetId> DefaultInventory;

	//By Default, It contains all accessible Item Types
	const TArray<FPrimaryAssetType> AllItemTypes{USoulAssetManager::ArmourItemType, USoulAssetManager::WeaponItemType,USoulAssetManager::PotionItemType,USoulAssetManager::JewelItemType};

	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		int32 SaveUserIndex;

	/** Rather it will attempt to actually save to disk */
	/** Sets rather save/load is enabled. If disabled it will always count as a new character */
	UPROPERTY()
		bool bSavingEnabled;

	/** The current save game object */
	UPROPERTY()
		USoulSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetAllAccessibleItemID(TArray<FPrimaryAssetId>& OutpId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetItemIDWithType(const FPrimaryAssetType ItemType, TArray<FPrimaryAssetId> & OutpId);

	/**
	 * Adds the default inventory to the inventory array
	 * @param InventoryArray Inventory to modify
	 * @param RemoveExtra If true, remove anything other than default inventory
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddDefaultInventory(bool bRemoveExtra = false);

	/** Returns true if this is a valid inventory slot */
 	UFUNCTION(BlueprintCallable, Category = Inventory)
 		bool IsValidItemSlot(FSoulItemSlot ItemSlot) const;

	/** Loads a save game. If it fails, it will create a new one for you. Returns true if it loaded, false if it created one */
	UFUNCTION(BlueprintCallable, Category = Save)
		bool LoadOrCreateSaveGame();

	/** Writes the current save game object to disk */
	UFUNCTION(BlueprintCallable, Category = Save)
		bool WriteSaveGame();

	/** Resets the current save game to it's default. This will erase player data! This won't save to disk until the next WriteSaveGame */
	UFUNCTION(BlueprintCallable, Category = Save)
		void ResetSaveGame();

	/** Spawn Floating Damage Widget on screen */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = UI)
		void SpawnFloatingDamageTextWidget(const AActor* DamageReceiver, const float DamageInput);

protected:



	//FStreamableDelegate
	void OnAsyncLoadingFinished();

	TSharedPtr<FStreamableHandle> TempStreamableHandle;
};