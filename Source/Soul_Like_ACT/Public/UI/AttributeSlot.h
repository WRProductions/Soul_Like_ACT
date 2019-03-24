// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "AttributeSlot.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(Abstract)
class SOUL_LIKE_ACT_API UAttributeSlot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *AttributeType = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock *AttributeValue = nullptr;

public:
	/** The Attribute we modify or the GE we modify modifies. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default, meta = (ExposeOnSpawn = true), meta = (FilterMetaTag = "HideFromModifiers"))
	FGameplayAttribute MyAttribute;

	UFUNCTION(BlueprintCallable)
	void SetAttributeType();

	UFUNCTION(BlueprintCallable)
	void OnAttributeChanged(TArray<float> values);
};
