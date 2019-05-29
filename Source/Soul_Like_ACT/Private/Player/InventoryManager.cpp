// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManager.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "SoulSaveGame.h"
#include "SoulGameInstanceBase.h"

UInventoryManager::UInventoryManager()
{
	PrimaryComponentTick.bCanEverTick = 0;
}

bool UInventoryManager::GetInventoryManager(UObject* WorldContext, UInventoryManager*& InventoryManager)
{
	UWorld *LocalWorld = WorldContext->GetWorld();
	if (LocalWorld)
	{
		APlayerController *LocalController = LocalWorld->GetFirstPlayerController();
		if (LocalController)
		{
			ACharacter *CurrCharacter = LocalController->GetCharacter();
			if (CurrCharacter)
			{
				ASoul_Like_ACTCharacter* MyChar = Cast<ASoul_Like_ACTCharacter>(CurrCharacter);
				if (MyChar)
				{
					InventoryManager = MyChar->GetInventoryManager();
					return true;
				}
			}
		}
	}
	return false;
}

bool UInventoryManager::AddInventoryItem(FSoulItemData InItemData)
{
	bool bChanged = false;

	if (!InItemData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}

	//Get slots with the similar item base
	TArray<FSoulItemSlot> OldSlots;
	bool bSuccessfulChecked = GetInventSlots(InItemData, OldSlots, true, false);
	
	//If we found a slot and not auto slot, update the old slot with new data
	if(bSuccessfulChecked)
	{
		for (auto & LocalSlot : OldSlots)
		{
			SetItemSlot(InItemData, LocalSlot);
		}

		while (InItemData.IsValid())
		{
			FSoulItemSlot RemainingSlot;

			bSuccessfulChecked = GetFirstInventSlot(InItemData, RemainingSlot, true, true);

			if (bSuccessfulChecked)
			{
				SetItemSlot(InItemData, RemainingSlot);
			}
		}

		LOG_FUNC_SUCCESS();
		return bSuccessfulChecked;
	}
	/**
	 * Fill the inventory slot if
	 * No same item bases in the inventory
	 */
	else
	{
		FSoulItemSlot LocalEmptySlot;

		bSuccessfulChecked = GetFirstInventSlot(InItemData, LocalEmptySlot, true, true);

		if (bSuccessfulChecked)
		{
			SetItemSlot(InItemData, LocalEmptySlot);
			LOG_FUNC_SUCCESS();
			return true;
		}

		LOG_FUNC_FAIL();
		return false;
	}

	LOG_FUNC_FAIL();
	return false;
}

bool UInventoryManager::RemoveInventoryItem(FSoulItemData RemovedItem)
{
	FSoulItemSlot OldSlot;
	
	if (GetFirstInventSlot(RemovedItem, OldSlot))
		return RemoveInventoryItemAtIndex(OldSlot, -RemovedItem.ItemCount);
	return false;
}

bool UInventoryManager::RemoveInventoryItemAtIndex(FSoulItemSlot InItemSlot, int32 ItemCount /*= 1*/)
{
	InventoryItems[InItemSlot].ItemCount -= ItemCount;
	
	if (InventoryItems[InItemSlot].ItemCount < 0)
	{
		InventoryItems[InItemSlot].ItemCount = 0;
	}
	
	return true;
}

bool UInventoryManager::GetFirstInventSlot(FSoulItemData InItemData
	, FSoulItemSlot & OutSlot
	, bool bSkipFullSlot /*= true*/
	, bool bGetEmptySlot /*= false*/) const
{
	for (auto& ItemTuple : InventoryItems)
	{
		if (bGetEmptySlot && !(ItemTuple.Value.IsValid()))
		{
			OutSlot = ItemTuple.Key;
			return true;
		}
		else if (ItemTuple.Value.HasSameItem(InItemData)
			&& (ItemTuple.Value.ItemCount < ItemTuple.Value.ItemBase->MaxCount || !bSkipFullSlot))
		{
			OutSlot = ItemTuple.Key;
			return true;
		}
	}

	return false;
}

bool UInventoryManager::GetInventSlots(FSoulItemData InItemData, TArray<FSoulItemSlot>& OutItemDatas, bool bSkipFullSlot /*= true*/, bool bGetEmptySlot /*= false*/) const
{
	for (auto& ItemTuple : InventoryItems)
	{
		if (bGetEmptySlot && !(ItemTuple.Value.IsValid()))
		{
			OutItemDatas.Add(ItemTuple.Key);
		}
		else if (ItemTuple.Value.HasSameItem(InItemData)
			&& (ItemTuple.Value.ItemCount < ItemTuple.Value.ItemBase->MaxCount || !bSkipFullSlot))
		{
			OutItemDatas.Add(ItemTuple.Key);
		}
	}

	if (OutItemDatas.Num() > 0) return true;
	return false;
}

bool UInventoryManager::GetInventoryItemData(FSoulItemSlot InItemSlot, FSoulItemData& ItemData) const
{
	ItemData = InventoryItems.FindChecked(InItemSlot);
	
	if (ItemData.IsValid()) return true;
	return false;
}

bool UInventoryManager::GetEquipItemData(FSoulEquipmentSlot InEquipSlot, FSoulItemData& ItemData) const
{
	const FSoulItemData* TempItemData = EquipedItems.Find(InEquipSlot);
	
	if (TempItemData->IsValid())
	{
		ItemData = *TempItemData;
		return true;
	}
	
	return false;
}

int32 UInventoryManager::GetSlottedItemCount(FSoulItemSlot InItemSlot) const
{
	FSoulItemData LocalData;
	
	return GetInventoryItemData(InItemSlot, LocalData) ? LocalData.ItemCount : 0;
}


const EGearType UInventoryManager::GetGearType(FSoulItemSlot InItemSlot)
{
	FSoulItemData LocalItemData;
	
	const bool bSuccessful = GetInventoryItemData(InItemSlot, LocalItemData);

	return bSuccessful ? LocalItemData.ItemBase->ItemSlotType : EGearType::Non_Gear;
}

bool UInventoryManager::AddEquipment(FSoulItemSlot InventorySlot)
{
	FSoulItemData InventData;

	if (!InventorySlot.IsValid()) return false;

	if (GetInventoryItemData(InventorySlot, InventData))
	{
		FSoulEquipmentSlot EquipSlot;
		
		if (GetEquipSlot(InventData.ItemBase->ItemSlotType, EquipSlot))
		{
			EquipToInventory(EquipedItems[EquipSlot], InventorySlot);
			
			InventoryToEquipment(InventData, EquipSlot);
			
			return true;
		}
		else
		{
			LOG_FUNC_ERROR("Cannot get EquipmentSlot");
			
			return false;
		}
	}
	
	LOG_FUNC_ERROR("Cannot get EquipmentSlot");

	return false;
}

void UInventoryManager::EquipGear(AWeaponActor *const Inp)
{
	if (CurrentWeapon)
	{
		FDetachmentTransformRules LocalDetRules{ EDetachmentRule::KeepWorld, 1 };
		CurrentWeapon->DetachFromActor(LocalDetRules);
		CurrentWeapon->Destroy();
	}

	CurrentWeapon = Inp;

	FAttachmentTransformRules LocalTransRules{ EAttachmentRule::SnapToTarget, 1 };
	if(Inp->GearInfo->WeaponType == EWeaponType::VE_1HSword)
		Inp->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), LocalTransRules, TEXT("2H_Mace"));
	else if (Inp->GearInfo->WeaponType == EWeaponType::VE_2HMace)
		Inp->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), LocalTransRules, TEXT("2H_Mace"));
	else if (Inp->GearInfo->WeaponType == EWeaponType::VE_Fist)
		Inp->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), LocalTransRules, TEXT("2H_Mace"));	
}

bool UInventoryManager::SaveInventory()
{
	return false;
}

bool UInventoryManager::LoadInventoryData(TArray<FSoulItemData> InInventoryItems
	, TMap<FSoulEquipmentSlot, FSoulItemData> InEquipedItems)
{
	/*InventoryItems = InInventoryItems;*/
	int i = 0;

	for (FSoulItemData TempItemData : InInventoryItems)
	{
		if (TempItemData.IsValid())
		{
			InventoryItems.Add(FSoulItemSlot(i), TempItemData);
		}
		else
		{
			InventoryItems.Add(FSoulItemSlot(i), FSoulItemData());
		}
		++i;
	}

	EquipedItems = EquipedItems;

	//TODO: update GAs

	Notify_OnInventoryLoadingFinished(true);
	
	return true;
}

bool UInventoryManager::InventoryToEquipment(FSoulItemData FromItem, FSoulEquipmentSlot ToSlot)
{
	if (FromItem.ItemBase->ItemSlotType != EGearType::Non_Gear)
	{
		FSoulEquipmentSlot MyEquipSlot(FromItem.ItemBase->ItemSlotType);
		if (MyEquipSlot == ToSlot)
		{
			//TODO: update GA


			EquipedItems[MyEquipSlot] = FromItem;

			NotifyEquipmentChanged(MyEquipSlot, FromItem);

			//TODO: update GA
			return true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("UInventoryManager -> Cannot Equip the item"));
	return false;
}

bool UInventoryManager::EquipToInventory(FSoulItemData FromItem, FSoulItemSlot ToSlot, bool bAutoSlot/*=true*/)
{
	if (ToSlot.IsValid() && !InventoryItems[ToSlot].IsValid())
	{
		InventoryItems[ToSlot] = FromItem;

		NotifySlottedItemChanged(ToSlot, FromItem);
		//TODO: remove GA

		return true;
	}
	else if(bAutoSlot)
	{
		FSoulItemSlot NewEmptySlot;
		
		GetFirstInventSlot(FromItem, NewEmptySlot, true, true);

		//TODO: remove GA

		InventoryItems[NewEmptySlot] = FromItem;
 		
		NotifySlottedItemChanged(NewEmptySlot, FromItem);

		return true;
	}
	else
	{
		return false;
	}
}

void UInventoryManager::SetItemSlot(UPARAM(ref) FSoulItemData& InItemData, FSoulItemSlot ItemSlot)
{
	if (InItemData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Old_ItemData-> Name:%s, Num:%i"), *(InItemData.ItemBase->ItemName.ToString()), InItemData.ItemCount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Old_ItemData->Invalid"));
	}

	FSoulItemData MyInventData;
	bool bNotEmptySlot = GetInventoryItemData(ItemSlot, MyInventData);

	if (!bNotEmptySlot)
	{
		InventoryItems[ItemSlot] = InItemData;
		InItemData.ItemCount = 0;
	}

	//Update ItemData if both have the same type
	else if (InItemData.HasSameItem(MyInventData))
	{
		InventoryItems[ItemSlot].UpdateItemData(InItemData);
	}

	//Switch ItemData
	else
	{
		InventoryItems[ItemSlot] = InItemData;
		InItemData = MyInventData;
	}

	if (InventoryItems[ItemSlot].IsValid())
	{		UE_LOG(LogTemp, Warning, TEXT("New_ItemData-> Name:%s, Num:%i"), *(InventoryItems[ItemSlot].ItemBase->ItemName.ToString()), InventoryItems[ItemSlot].ItemCount);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("New_ItemData->Invalid"));
	}
	NotifySlottedItemChanged(ItemSlot, InventoryItems[ItemSlot]);
}

void UInventoryManager::Notify_OnInventoryLoadingFinished(bool bFirstTimeInit)
{
	if (OnInventoryLoadingFinished.IsBound())
		OnInventoryLoadingFinished.Broadcast(this, bFirstTimeInit);
}

void UInventoryManager::NotifySlottedItemChanged(FSoulItemSlot ItemSlot, FSoulItemData ItemData)
{
	OnSlottedItemChanged.Broadcast(ItemSlot, ItemData);
}

void UInventoryManager::NotifyEquipmentChanged(FSoulEquipmentSlot EquipmentSlot, FSoulItemData ItemData)
{
	OnEquipmentChangedChanged.Broadcast(EquipmentSlot, ItemData);
}
