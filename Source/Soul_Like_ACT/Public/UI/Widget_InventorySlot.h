// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Units)
	class UBorder* ContentBorder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
	UUserWidget* BackWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
	enum EGearTypes SlotType;
};
