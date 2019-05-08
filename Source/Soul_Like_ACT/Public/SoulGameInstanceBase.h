// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoulSaveGame.h"
#include "SoulGameInstanceBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingFinished);

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
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
 	TArray<FSoulSaveItemData> DefaultInventory;

	//By Default, It contains all accessible Item Types
	const TArray<FPrimaryAssetType> AllItemTypes{USoulAssetManager::ArmourItemType, USoulAssetManager::WeaponItemType,USoulAssetManager::PotionItemType,USoulAssetManager::JewelItemType};

	UFUNCTION(BlueprintCallable)
		static void GetSoulPlayer(UObject* WorldContextObject, ASoulPlayerController*& MyController, ASoul_Like_ACTCharacter*& MyChar, UInventoryManager*& MyInentory, bool &Successful);

	UFUNCTION(BlueprintCallable)
	USoulSaveGame* GetSaveSlot();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetAllAccessibleItemID(TArray<FPrimaryAssetId>& OutpId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetItemIDWithType(const FPrimaryAssetType ItemType, TArray<FPrimaryAssetId> & OutpId);

	/* Adds the default inventory to the inventory array */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void AddDefaultInventory();

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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void AsyncLoadingSaveGame();

	UFUNCTION(BlueprintNativeEvent)
		void OnStartGameClicked();

	UPROPERTY(BlueprintAssignable)
		FOnLoadingFinished OnSaveGameLoadingFinished;

protected:
	/** Rather it will attempt to actually save to disk */
	/** Sets rather save/load is enabled. If disabled it will always count as a new character */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Save)
		bool bForceReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Levels)
	FName GameplayLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Levels)
	FName LoginLevelName;

	/** The slot name used for saving */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Save)
	FString SaveSlot;

	/** The platform-specific user index */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Save)
	TSubclassOf<USaveGame> DefaultSaveGameBPClass;

	/** The current save game object */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Save)
	USoulSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly))
	void MakeSoulItemData(UObject* InItemBase, TArray<UObject*> InJewels, FSoulItemData &OutItemData, int32 InItemCount = 1, int32 InItemLevel = 1);

	void Broadcast_OnSaveGameLoadFinshed();
};