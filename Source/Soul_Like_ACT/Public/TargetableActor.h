// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Targetable.h"
#include "TargetableActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorHealthChanged, int32, ActorCurrentHealth, int32, ActorMaxHealth);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActorHealthChangedNative, int32, int32);



UENUM(BlueprintType)
enum class EActorFaction : uint8
{
	Untargetable,
	Player,
	Enemy
};


UCLASS()
class SOUL_LIKE_ACT_API ATargetableActor : public ACharacter, public ITargetable
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStatusComponent *StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent *TargetIcon;

public:
	// Sets default values for this pawn's properties
	ATargetableActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsTargetable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActorFaction Faction;
	
	UPROPERTY(BlueprintAssignable)
	FOnActorHealthChanged OnActorHealthChanged;

	UFUNCTION(BlueprintCallable)
	void BroadCastOnHealthChanged();

	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetable() const override { return bIsTargetable; }

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLockIcon(bool LockOn) override;
};


