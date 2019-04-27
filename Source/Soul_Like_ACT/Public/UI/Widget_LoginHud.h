// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_LoginHud.generated.h"

class UTextBlock;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_LoginHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* StartGame;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ResetButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Rank;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerID;

	//Call this on CONSTRUCT
	UFUNCTION(BlueprintCallable)
	void CacheSaveSlot();
	UFUNCTION(BlueprintCallable)
	void BindButtons();
};
