// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoulPlayerController.generated.h"

class USoulUIManager;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API ASoulPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoulPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USoulUIManager* UIManager;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetupUI();
	
	UFUNCTION(BlueprintCallable)
	USoulUIManager* GetUIManager() const { return UIManager; }

	UFUNCTION(BlueprintCallable)
	USoulUIManager* GetUIManagerFromController(AController* InController);
};
