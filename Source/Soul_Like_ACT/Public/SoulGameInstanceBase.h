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
 		TMap<FPrimaryAssetId, FSoulItemData> DefaultInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<FPrimaryAssetType> AllItemTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
		TArray<USoulItem*> AllItems;

	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(BlueprintReadWrite, Category = Save)
		int32 SaveUserIndex;

	/** Load all the sotre items up front */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Inventory)
		void InitializeAllItems();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetItemsIDWithType(const TArray<FPrimaryAssetType> ItemTypes, TMap<FPrimaryAssetId, FSoulItemData> & OutpItems);

	/**
	 * Adds the default inventory to the inventory array
	 * @param InventoryArray Inventory to modify
	 * @param RemoveExtra If true, remove anything other than default inventory
	 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddDefaultInventory(USoulSaveGame* SaveGame, bool bRemoveExtra = false);

	/** Returns true if this is a valid inventory slot */
 	UFUNCTION(BlueprintCallable, Category = Inventory)
 		bool IsValidItemSlot(FSoulItemSlot ItemSlot) const;

	/** Returns the current save game, so it can be used to initialize state. Changes are not written until WriteSaveGame is called */
	UFUNCTION(BlueprintCallable, Category = Save)
		USoulSaveGame* GetCurrentSaveGame();

	/** Sets rather save/load is enabled. If disabled it will always count as a new character */
	UFUNCTION(BlueprintCallable, Category = Save)
		void SetSavingEnabled(bool bEnabled);

	/** Loads a save game. If it fails, it will create a new one for you. Returns true if it loaded, false if it created one */
	UFUNCTION(BlueprintCallable, Category = Save)
		bool LoadOrCreateSaveGame();

	/** Writes the current save game object to disk */
	UFUNCTION(BlueprintCallable, Category = Save)
		bool WriteSaveGame();

	/** Resets the current save game to it's default. This will erase player data! This won't save to disk until the next WriteSaveGame */
	UFUNCTION(BlueprintCallable, Category = Save)
		void ResetSaveGame();

protected:
	/** The current save game object */
	UPROPERTY()
		USoulSaveGame* CurrentSaveGame;

	/** Rather it will attempt to actually save to disk */
	UPROPERTY()
		bool bSavingEnabled;
};