// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulAttributeSet.h"
#include "Interfaces/Targetable.h"
#include "SoulCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

//Exec only
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
class SOUL_LIKE_ACT_API ASoulCharacterBase : public ACharacter, public ITargetable, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASoulCharacterBase();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent *TargetIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UActorFXManager *FXManager;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly)
		class USoulAbilitySystemComponent *AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
		USoulAttributeSet* AttributeSet;

	/** If true we have initialized our abilities */
	UPROPERTY()
		bool bAbilitiesInitialized;

	/** Temp Gameplay Abilities */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AnimGA)
		TArray<TSubclassOf<UGameplayAbility>> AbilityArray;

	/** Passive gameplay effects applied on creation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	FTimerHandle Handle_SlowMotion, Handler_SlowMotionDelay;

	void WaitForDilationReset()
	{ 
		CustomTimeDilation = 1.f; 
	}

	void TriggerSlowMotion()
	{
		CustomTimeDilation = 0.01f;
		GetWorldTimerManager().SetTimer(Handle_SlowMotion, this, &ASoulCharacterBase::WaitForDilationReset, 1.f, 0, 0.15f);
	}

public:	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActorFaction Faction;

	UPROPERTY(BlueprintAssignable)
	FOnDead OnDead_Delegate;

//Static
public:
	static const bool IsInRivalFaction(ASoulCharacterBase *DamageDealer, ASoulCharacterBase *DamageReceiver);

	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetable() const override { return Faction != EActorFaction::Untargetable; }

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLockIcon(bool LockOn) override;

	//Called by WeaponActor and OnHit
	UFUNCTION(BlueprintCallable)
	void TriggerSlowMotion_WithDelay(float Delay);

	//Warning: Link this to AnyPointDamage node in BP
	virtual void Exec_TryGetHit(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser, const FHitResult &HitInfo, EOnHitRefelction &Outp);

	UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const { return AttributeSet->GetHealth(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const { return AttributeSet->GetMaxHealth(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetStamina() const { return AttributeSet->GetMaxHealth(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetMaxStamina() const { return AttributeSet->GetMaxHealth(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetLeech() const { return AttributeSet->GetMaxHealth(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetDefensePower() const { return AttributeSet->GetDefensePower(); }
	
	UFUNCTION(BlueprintCallable)
		virtual float GetAttackPower() const { return AttributeSet->GetAttackPower(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetTenacity() const { return AttributeSet->GetTenacity(); }

	UFUNCTION(BlueprintCallable)
		virtual float GetMoveSpeed() const { return AttributeSet->GetMoveSpeed(); }

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
		virtual int32 GetCharacterLevel() const { return 1; }


protected:
	/** Apply the startup GAs and GEs */
	void AddStartupGameplayAbilities();

	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnStaminaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnLeechChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnTenacityChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnDefensePowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAttackPowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
		void OnAttackSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleStaminaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleLeechChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleTenacityChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleDefensePowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleAttackPowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleAttackSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend USoulAttributeSet;

public:
	
	UFUNCTION(BlueprintCallable)
		static void TagContainerToString(const FGameplayTagContainer &Container, FString &Outp)
	{
		Outp = Container.ToString();
	}
};