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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UVerticalBox* Modifier_Slot;
	

	/**
	 * Temp Variables
	 */
	TArray<FString> ModifierNames;
	TArray<FString> ModifierLevels;
	FString ItemLevel;

public:
	/**
	 * The function is called when to read the slot information
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MakeItemInfo(TSubclassOf<class USoulItem> ItemToRead, const FSoulItemData& ItemData);
};
