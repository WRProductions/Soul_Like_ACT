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
	bool bSuccessfulChecked = GetSlots(InItemData, OldSlots, true, false);
	
	//If we found a slot and not auto slot, update the old slot with new data
	if(bSuccessfulChecked)
	{
		for (auto & LocalSlot : OldSlots)
		{

			InventoryItems[LocalSlot].UpdateItemData(InItemData);
			NotifySlottedItemChanged(LocalSlot, InItemData);
			UE_LOG(LogTemp, Warning, TEXT("UInventoryManager::AddInventoryItem -> Old_Slot_[%i]"), LocalSlot.SlotNumber);
		}

		while (InItemData.IsValid())
		{
			FSoulItemSlot RemainingSlot;

			bSuccessfulChecked = GetFirstSlot(InItemData, RemainingSlot, true, true);

			if (bSuccessfulChecked)
			{
				InventoryItems[RemainingSlot].UpdateItemData(InItemData);
				NotifySlottedItemChanged(RemainingSlot, InItemData);
				UE_LOG(LogTemp, Warning, TEXT("UInventoryManager::AddInventoryItem -> Remining_Slot_[%i]"), RemainingSlot.SlotNumber);
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

		bSuccessfulChecked = GetFirstSlot(InItemData, LocalEmptySlot, true, true);

		if (bSuccessfulChecked)
		{
			InventoryItems[LocalEmptySlot] = InItemData;
			NotifySlottedItemChanged(LocalEmptySlot, InItemData);
			UE_LOG(LogTemp, Warning, TEXT("UInventoryManager::AddInventoryItem -> New_Slot_[%i]"), LocalEmptySlot.SlotNumber);
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
	
	if (GetFirstSlot(RemovedItem, OldSlot))
		return RemoveInventoryItemAtIndex(OldSlot, -RemovedItem.ItemCount);
	return false;
}

bool UInventoryManager::RemoveInventoryItemAtIndex(FSoulItemSlot InItemSlot, int32 ItemCount /*= 1*/)
{
	FSoulItemData EmptyItemData;
	InventoryItems[InItemSlot].ItemCount -= ItemCount;
	if (InventoryItems[InItemSlot].ItemCount < 0)
	{
		InventoryItems[InItemSlot].ItemCount = 0;
	}
	return EmptyItemData.IsValid();
}

bool UInventoryManager::GetFirstSlot(FSoulItemData InItemData
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

bool UInventoryManager::GetSlots(FSoulItemData InItemData, TArray<FSoulItemSlot>& OutItemDatas, bool bSkipFullSlot /*= true*/, bool bGetEmptySlot /*= false*/) const
{
	for (auto& ItemTuple : InventoryItems)
	{
		if (bGetEmptySlot && !(ItemTuple.Value.IsValid()))
		{
			OutItemDatas.Add(ItemTuple.Key);
		}
		else if (ItemTuple.Value.HasSameItem(InItemData)
			&& (ItemTuple.Value.ItemCount < ItemTuple.Value.ItemBase->MaxCount || !bSkipFullSlot))
			OutItemDatas.Add(ItemTuple.Key);
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

void UInventoryManager::AddEquipment(FSoulItemSlot InventorySlot)
{
	
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

void UInventoryManager::SetItemSlot(FSoulItemData& InItemData, FSoulItemSlot ItemSlot)
{
	FSoulItemData MyInventData;
	bool bWasEmptySlot = GetInventoryItemData(ItemSlot, MyInventData);

	if (bWasEmptySlot)
	{
		InventoryItems[ItemSlot] = FSoulItemData(InItemData);
		InItemData.ItemCount = 0;
	}

	//Update ItemData if both have the same type
	else if(InItemData.HasSameItem(MyInventData))
	{
		InventoryItems[ItemSlot].UpdateItemData(InItemData);
	}

	//Switch ItemData
	else
	{
		InventoryItems[ItemSlot] = InItemData;
		InItemData = MyInventData;
	}
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
