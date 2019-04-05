// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBasic.h"
#include "GameFramework/SaveGame.h"
#include "SoulSaveGame.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SOUL_LIKE_ACT_API USoulSaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	/** Constructor */
	USoulSaveGame()
	{
		// Set to current version, this will get overwritten during serialization when loading
		// SavedDataVersion = ERPGSaveGameVersion::LatestVersion;
	}

	/** Map of items to item data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		TMap<FPrimaryAssetId, FSoulItemData> InventoryData;

	/** Map of slotted items */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		TMap<FSoulItemSlot, FPrimaryAssetId> SlottedItems;

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
		FString UserId;

};
