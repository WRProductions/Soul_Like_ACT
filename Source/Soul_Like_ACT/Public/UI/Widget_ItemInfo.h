// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_ItemInfo.generated.h"

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
	class UVerticalBox* ModifierBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemType;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemMainStat;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemStat;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemLevel;
	

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
	void MakeItemInfo(USoulItem *ItemToRead, const FSoulItemData& ItemData);
};
