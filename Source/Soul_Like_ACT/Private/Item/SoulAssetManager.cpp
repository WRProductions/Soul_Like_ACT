// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulAssetManager.h"
#include "Item/ItemBasic.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	USoulAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	USoulAssetManager::JewelItemType = TEXT("Jewel");
const FPrimaryAssetType	USoulAssetManager::WeaponItemType = TEXT("Weapon");
const FPrimaryAssetType	USoulAssetManager::ArmourItemType = TEXT("Armour");

void USoulAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}

USoulAssetManager* USoulAssetManager::Get()
{
	USoulAssetManager* This = Cast<USoulAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be SoulAssetManager!"));
		return NewObject<USoulAssetManager>(); // never calls this
	}
}

USoulItem* USoulAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning /*= true*/)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	USoulItem* LoadedItem = Cast<USoulItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
