// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture.h"
#include "SoulAssetManager.h"
#include "ItemBasic.generated.h"

class USoulGameplayAbility;

/** Base class for all items, do not blueprint directly */
UCLASS(Abstract, BlueprintType)
	class SOUL_LIKE_ACT_API USoulItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Constructor */
	USoulItem()
		: MaxCount(1)
		  , MaxLevel(1)
	{
	}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FPrimaryAssetType ItemType;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UTexture* ItemIcon;

	/** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxCount;

	/** Returns if the item is consumable (MaxCount <= 0)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
	bool IsConsumable() const;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;

	/** Ability to grant if this item is slotted */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	TMap<TSubclassOf<USoulGameplayAbility>, int32> GrantedAbility;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

UCLASS()
	class SOUL_LIKE_ACT_API USoulWeaponItem : public USoulItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	USoulWeaponItem()
	{
		ItemType = USoulAssetManager::WeaponItemType;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USoulPrimaryStatusGameplayAbility> PrimaryAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PrimaryAbilityLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AWeaponActor> WeaponActor;
};

UCLASS()
	class SOUL_LIKE_ACT_API USoulArmourItem : public USoulItem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USoulPrimaryStatusGameplayAbility> PrimaryAbility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 PrimaryAbilityLevel;

	/** Constructor */
	USoulArmourItem()
	{
		ItemType = USoulAssetManager::ArmourItemType;
	}
};

UCLASS()
	class SOUL_LIKE_ACT_API USoulPotionItem : public USoulItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	USoulPotionItem()
	{
		ItemType = USoulAssetManager::PotionItemType;
		MaxCount = 10; // Infinite
	}
};

UCLASS()
	class SOUL_LIKE_ACT_API USoulJewelItem : public USoulItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	USoulJewelItem()
	{
		ItemType = USoulAssetManager::JewelItemType;
	}
};
