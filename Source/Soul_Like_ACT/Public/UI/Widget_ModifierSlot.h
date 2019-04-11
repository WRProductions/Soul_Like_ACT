// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ModifierSlot.generated.h"

class UTextBlock;

/**
 * The 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_ModifierSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Name = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Level = nullptr;

// 	UFUNCTION(BlueprintCallable)
// 	void OnSlottedItemChanged(const struct FSoulItemSlot &ItemSlot, const class USoulItem*& Item);

public:
	UFUNCTION(BlueprintCallable)
	void Setup(const FString &inp_Name, const FString &inp_Level);
};
