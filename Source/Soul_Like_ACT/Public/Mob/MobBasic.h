// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulCharacterBase.h"
#include "MobBasic.generated.h"

class UDA_FXCollection;

UCLASS()
class SOUL_LIKE_ACT_API AMobBasic : public ASoulCharacterBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMob_TargetingComponent *TargetingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gears)
		class AWeaponActor *Weapon;

public:
	// Sets default values for this pawn's properties
	AMobBasic();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMobActionManager *ActionManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeath(int32 CurrHealth, int32 MaxHealth);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetTarget(AActor *PlayerPawn) const;

	bool GetIsStun();

	UMob_TargetingComponent *GetTargetingComponent() const { return TargetingComponent; }

	void SetFocus(bool Enable, AActor * Target);

	bool GetIsTargetingEnabled() const;
};
