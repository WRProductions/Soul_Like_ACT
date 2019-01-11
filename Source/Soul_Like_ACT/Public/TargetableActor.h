// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Targetable.h"
#include "TargetableActor.generated.h"

UENUM(BlueprintType)
enum class EOnHitRefelction: uint8
{
	Immune,
	Parry,
	Block,
	OnHit,
	Vulnerable,
};

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

public:
	// Sets default values for this pawn's properties
	ATargetableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStatusComponent *StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent *TargetIcon;
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

	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetable() const override { return Faction != EActorFaction::Untargetable; }

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLockIcon(bool LockOn) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class UStatusComponent *GetStatusComponent() const { return StatusComponent; }

//Static
public:
	static const bool IsInRivalFaction(ATargetableActor *DamageDealer, ATargetableActor *DamageReceiver);

	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = Outp))
	virtual void Exec_TryGetHit(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser, EOnHitRefelction &Outp);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageTaken(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHitEffects(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnStunEffects(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnParryEffects(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};


