// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "Abilities/SoulAttributeSet.h"
#include "Interfaces/Targetable.h"
#include "SoulCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChanged, const TArray<float> &, values);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTrigger_ThreeParams, AActor*, SourceActor, AActor*, TargetActor, const FHitResult, HitResult);



UENUM(BlueprintType)
enum class EIsControllerValid : uint8
{
	IsValid,
	IsNotValid,
};

#define ATTRIBUTE_GETTER(PropertyName) \
	UFUNCTION(BlueprintCallable) \
	virtual float Get##PropertyName##() const \
	{ \
		return AttributeSet->Get##PropertyName##(); \
	}

#define ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(PropertyName) \
	ATTRIBUTE_GETTER(PropertyName) \
	virtual void Handle##PropertyName##Changed(const FOnAttributeChangeData& Data) \
	{ \
		if(On##PropertyName##Changed.IsBound()) \
			On##PropertyName##Changed.Broadcast(TArray<float>{Get##PropertyName##(), -1.f}); \
	}

#define ATTRIBUTE_GETTER_AND_HANDLECHANGED_TwoParams(PropertyName) \
	ATTRIBUTE_GETTER(##PropertyName##) \
	virtual void Handle##PropertyName##Changed(const FOnAttributeChangeData& Data) \
	{ \
		if(On##PropertyName##Changed.IsBound()) \
			On##PropertyName##Changed.Broadcast(TArray<float>{Get##PropertyName##(), GetMax##PropertyName##()}); \
	}

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
	//Notify UI on attributes changed via non-executions.
	//Only used on player pawn at the moment.
	UFUNCTION(BlueprintCallable)
	void BindOnAttributesChanged();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent *TargetIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UActorFXManager *FXManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USoulModifierManager* ModifierManager;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly)
	class USoulAbilitySystemComponent *AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
	USoulAttributeSet* AttributeSet;

	bool bIsDead, bIsDisabled;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Perilous)
	bool bIsPerilous;

	UAnimMontage* DeathMontage;

	FTimerHandle Handle_SlowMotion, Handler_SlowMotionDelay;

	void WaitForDilationReset()
	{ 
		CustomTimeDilation = 1.f; 
	}

	void TriggerSlowMotion()
	{
		CustomTimeDilation = 0.003f;
		GetWorldTimerManager().SetTimer(Handle_SlowMotion, this, &ASoulCharacterBase::WaitForDilationReset, 1.f, false, 0.2f);
	}

public:	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual USoulModifierManager* GetModifierManager() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	///////////////////////////////////Input Block///////////////////////////////////////////////
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Default)
	bool bMoveBlock;

	bool CachedMoveBlock ,bCached;
	
	//Remember to call ResetBlockMove after this function is called
	UFUNCTION(BlueprintCallable)
	void TemporaryBlockMove() { CachedMoveBlock = bMoveBlock; bMoveBlock = true;  bCached = true; }
	
	UFUNCTION(BlueprintCallable)
	void ResetBlockMove()
	{
		bMoveBlock = false;
		bCached = false;
	}
	
	UFUNCTION(BlueprintCallable)
	void ResetBlockMove_UseCache(bool& Succeed) 
	{
		if(bCached)
			bMoveBlock = CachedMoveBlock;
		Succeed = bCached;
		bCached = false; 
	}

	UFUNCTION(BlueprintCallable)
	bool IsDisabled() const { return bIsDisabled; }

	UFUNCTION(BlueprintCallable)
	virtual void SetDisabled(bool bEnable)
	{ 
		bIsDisabled = bEnable;

		if (bIsDisabled) 
		{
			TemporaryBlockMove();
		}
		else 
		{
			ResetBlockMove();
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EActorFaction Faction;

	static const bool IsInRivalFaction(ASoulCharacterBase *DamageDealer, ASoulCharacterBase *DamageReceiver);

	UFUNCTION(BlueprintCallable)
	virtual bool IsTargetable() const override 
	{ 
		return (Faction != EActorFaction::Untargetable
			&& !GetIsDead()); 
	}

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLockIcon() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const { return GetHealth() > 0.f; }

	//Called by WeaponActor and OnHit
	UFUNCTION(BlueprintCallable)
	void TriggerSlowMotion_WithDelay(float Delay);

	void Notify_OnMeleeAttack(AActor * TargetActor, const FHitResult HitResult)
	{
		if (OnMeleeAttack.IsBound())
			OnMeleeAttack.Broadcast(this, TargetActor, HitResult);
	}

	void Notify_OnMeleeKill(AActor* SourceActor, AActor* TargetActor, const FHitResult HitResult)
	{
		if (OnMeleeKill.IsBound())
			OnMeleeKill.Broadcast(SourceActor, TargetActor, HitResult);
	}

	void Notify_OnStun(AActor* SourceActor, const FHitResult HitResult)
	{
		if (OnStun.IsBound())
		{
			OnStun.Broadcast(SourceActor, this, HitResult);
		}
	}

	void OnParry(AActor* SourceActor, const FHitResult HitResult, bool bPerfectParry, bool bFailed)
	{
// 		if (bFailed)
// 		{
// 			if (OnParryFailed.IsBound())
// 				OnParryFailed.Broadcast(SourceActor, this, HitResult);
// 			return;
// 		}
// 		else if (bPerfectParry)
// 		{
// 			if (OnParrySucced.IsBound())
// 				OnParrySucced.Broadcast(SourceActor, this, HitResult);
// 		}
// 		else
// 		{
// 			if (OnParryNormal.IsBound())
// 				OnParryNormal.Broadcast(SourceActor, this, HitResult);
// 		}
	}

	ATTRIBUTE_GETTER_AND_HANDLECHANGED_TwoParams(Health);
	ATTRIBUTE_GETTER(MaxHealth);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_TwoParams(Posture);
	ATTRIBUTE_GETTER(MaxPosture);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(MeleeRange);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(Leech);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(PostureStrength);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(DefensePower);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(AttackPower);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(PostureCrumble);
	ATTRIBUTE_GETTER(MoveSpeed);
	virtual void HandleMoveSpeedChanged(const FOnAttributeChangeData& Data);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(AttackSpeed);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(CriticalStrike);
	ATTRIBUTE_GETTER_AND_HANDLECHANGED_OneParam(CriticalMulti);

	UFUNCTION(BlueprintCallable)
	float GetPosturePercent() const { return GetPosture() / GetMaxPosture(); }

	UFUNCTION(BlueprintCallable)
	float GetAreaMultiplier() const { return GetMeleeRange(); }

	/** Returns the character level that is passed to the ability system */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const { return 1; }
	
	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable)
	bool GetIsHealthZero() const { return GetHealth() <= 0.f; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetWeapon(AWeaponActor *&OutWeaponActor);

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, bool IsCriticaled, EParryResult ParryResult, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleDotDamage(float DamageAmount, bool IsCriticaled, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandlePostureDamage(float PostureDamageAmount, bool IsCriticaled, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);
protected:
	/** Apply the startup GAs and GEs */
	UFUNCTION(BlueprintCallable)
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
	void OnDamaged(float DamageAmount, bool IsCriticaled, EParryResult ParryResult, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDotDamaged(float DamageAmount, bool IsCriticaled, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);
	
	//Called when character takes posture damage
	UFUNCTION(BlueprintImplementableEvent)
	void OnPostureDamaged(float PostureDamage, bool IsCriticaled, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser);

#pragma region ATTRIBUTE_DELEGATES
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnPostureChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnMoveSpeedChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnMeleeRangeChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnPostureStrengthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnDefensePowerChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnLeechChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnPostureCrumbleChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnAttackPowerChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnAttackSpeedChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnCriticalStrikeChanged;
	UPROPERTY(BlueprintAssignable)
	FOnChanged OnCriticalMultiChanged;
#pragma endregion

#pragma region ON_TRIGGER_DELEGATES
	//On_XXX trigger
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnMeleeAttack;
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnMeleeKill;
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnStun;
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnParryFailed;
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnParrySucced;
	UPROPERTY(BlueprintAssignable)
	FTrigger_ThreeParams OnParryNormal;
#pragma endregion

	UFUNCTION(BlueprintNativeEvent)
	void MakeStepDecelAndSound();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetPerilousStatus();

	UFUNCTION(BlueprintCallable)
	virtual void HandleOnDead();

public:
	UFUNCTION(BlueprintCallable)
	static void TagContainerToString(const FGameplayTagContainer &Container, FString &Outp)
	{
		Outp = Container.ToString();
	}

	UFUNCTION(BlueprintCallable)
	static void MakeStepDecelAndSound_Notify(ASoulCharacterBase *CharacterRef);

	friend USoulAttributeSet;

	UFUNCTION(BlueprintCallable)
	float GetAP_TEST() const { return GetAttackPower(); }

	//Use this to remove tag like Ability.Melee
	//So we can force to use Evade while attacking
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag_DANGER(const FGameplayTag& GameplayTag)
	{
		AbilitySystemComponent->SetTagMapCount(GameplayTag, 0);
	}
};