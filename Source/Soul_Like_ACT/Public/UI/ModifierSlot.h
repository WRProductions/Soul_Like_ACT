// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModifierSlot.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UModifierSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Name = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Value = nullptr;

// 	UFUNCTION(BlueprintCallable)
// 	void OnSlottedItemChanged(const struct FSoulItemSlot &ItemSlot, const class USoulItem*& Item);

	UFUNCTION(BlueprintCallable)
		void Setup(const TSubClassOf<USoulItem>& ItemInfo);
};
