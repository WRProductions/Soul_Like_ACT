// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TargetableActor.generated.h"

class UStatusComponent;

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
class SOUL_LIKE_ACT_API ATargetableActor : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStatusComponent *StatusComponent;
public:
	// Sets default values for this pawn's properties
	ATargetableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
};


