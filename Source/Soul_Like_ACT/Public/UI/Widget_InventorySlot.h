// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SoulItemTypes.h"
#include "Widget_InventorySlot.generated.h"
/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Default)
	FSoulItemData CurrentItemData;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
	class UBorder* ContentBorder;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
	class UImage* item_icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
	TSubclassOf<UUserWidget> BackWidget;

	UFUNCTION(BlueprintImplementableEvent)
	void SetupSlot(FSoulItemData InItemData);
};
