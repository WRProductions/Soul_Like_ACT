// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModifierSlot.generated.h"

class UTextBlock;


UCLASS()
class SOUL_LIKE_ACT_API UModifierSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Name = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* GA_Level = nullptr;

// 	UFUNCTION(BlueprintCallable)
// 	void OnSlottedItemChanged(const struct FSoulItemSlot &ItemSlot, const class USoulItem*& Item);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn))
		FText AbilityName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn))
		FText AbilityMagnitude;

public:
	UFUNCTION(BlueprintCallable)
	void Setup(const FText&inp_Name, const FText &inp_Level);
};
