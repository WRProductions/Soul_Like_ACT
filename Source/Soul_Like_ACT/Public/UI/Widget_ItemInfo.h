// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulSaveGame.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ItemInfo.generated.h"

class UTextBlock;
class UImage;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_ItemInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//Edit in BP
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* ModifierBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemType;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemMainStat;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemStat;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemStatPlus;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemLevel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemName;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemDescription;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ItemIcon;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ItemQuantity;

	/**
	 * Temp Variables
	 */
	TArray<FString> ModifierNames;
	TArray<FString> ModifierLevels;


public:
	/**
	 * The function is called when to read the slot information
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MakeItemInfo(const FSoulItemData& ItemData);
};
