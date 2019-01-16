// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Targetable.h"
#include "TargetableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStatusComponent *StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent *TargetIcon;

	FTimerHandle Handle_SlowMotion, Handler_SlowMotionDelay;

	void WaitForDilationReset()
	{ 
		CustomTimeDilation = 1.f; 
	}

	void TriggerSlowMotion()
	{
		CustomTimeDilation = 0.01f;
		GetWorldTimerManager().SetTimer(Handle_SlowMotion, this, &ATargetableActor::WaitForDilationReset, 1.f, 0, 0.15f);
	}

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActorFaction Faction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gears)
		class AWeaponActor *Weapon;

	UPROPERTY(BlueprintAssignable)
		FOnDead OnDead_Delegate;

//Static
public:
	static const bool IsInRivalFaction(ATargetableActor *DamageDealer, ATargetableActor *DamageReceiver);

	UFUNCTION(BlueprintCallable)
		virtual bool IsTargetable() const override { return Faction != EActorFaction::Untargetable; }

	UFUNCTION(BlueprintCallable)
		virtual void ToggleLockIcon(bool LockOn) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
		class UStatusComponent *GetStatusComponent() const { return StatusComponent; }
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool OnDamageTaken(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHitEffects(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnStunEffects(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnParryEffects(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser);

	//Called by WeaponActor and OnHit
	UFUNCTION(BlueprintCallable)
		void TriggerSlowMotion_WithDelay();


};


