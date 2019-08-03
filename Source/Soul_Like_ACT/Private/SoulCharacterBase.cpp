// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulCharacterBase.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Abilities/SoulModifierManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "ActorFxManager.h"

// Sets default values
ASoulCharacterBase::ASoulCharacterBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<USoulAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<USoulAttributeSet>(TEXT("AttributeSet"));

	FXManager = CreateDefaultSubobject<UActorFXManager>(TEXT("FXManager"));

	ModifierManager = CreateDefaultSubobject<USoulModifierManager>(TEXT("ModifierManager"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	TargetIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockIconWidget"));
	TargetIcon->SetupAttachment(RootComponent);
	TargetIcon->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIcon->SetDrawSize(FVector2D{ 35.f,35.f });
}


void ASoulCharacterBase::ToggleLockIcon()
{
	if (!IsTargetable() || TargetIcon->IsVisible())
	{
		TargetIcon->SetVisibility(false);
	}
	else
	{
		TargetIcon->SetVisibility(true);
	}
}

void ASoulCharacterBase::TriggerSlowMotion_WithDelay(float Delay)
{
	if (GetWorldTimerManager().GetTimerRemaining(Handler_SlowMotionDelay) <= 0.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Handler_SlowMotionDelay.IsValid()");
		GetWorldTimerManager().SetTimer(Handler_SlowMotionDelay, this, &ASoulCharacterBase::TriggerSlowMotion, 1.f, 0, Delay);
	}
}

void ASoulCharacterBase::HandleMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (OnMoveSpeedChanged.IsBound())
		OnMoveSpeedChanged.Broadcast(TArray<float>{GetMoveSpeed(), -1.f});
}

void ASoulCharacterBase::GetWeapon_Implementation(AWeaponActor*& OutWeaponActor)
{

}

void ASoulCharacterBase::AddStartupGameplayAbilities()
{
	ModifierManager->AddStartupGameplayAbilities();
}

void ASoulCharacterBase::HandleDamage(float DamageAmount, bool IsCriticaled, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, IsCriticaled, bIsStun, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
	
	if (GetIsHealthZero())
	{
		HandleOnDead();
	}
}

void ASoulCharacterBase::HandleDotDamage(float DamageAmount, bool IsCriticaled, bool bIsStun, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnDotDamaged(DamageAmount, IsCriticaled, bIsStun, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);

	if (GetIsHealthZero())
	{
		HandleOnDead();
	}
}

void ASoulCharacterBase::HandlePostureDamage(float PostureDamageAmount, bool IsCriticaled, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ASoulCharacterBase* InstigatorCharacter, AActor* DamageCauser)
{
	OnPostureDamaged(PostureDamageAmount, IsCriticaled, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::ResetPerilousStatus()
{
	LOG_FUNC_FAIL();
}

void ASoulCharacterBase::HandleOnDead()
{
	bIsDead = true;
}

void ASoulCharacterBase::MakeStepDecelAndSound_Notify(ASoulCharacterBase *CharacterRef)
{
	CharacterRef->MakeStepDecelAndSound();
}

void ASoulCharacterBase::MakeStepDecelAndSound_Implementation()
{
	return;
}

UAbilitySystemComponent* ASoulCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USoulModifierManager* ASoulCharacterBase::GetModifierManager() const
{
	return ModifierManager;
}

const bool ASoulCharacterBase::IsInRivalFaction(ASoulCharacterBase *DamageDealer, ASoulCharacterBase *DamageReceiver)
{
	if (DamageDealer->Faction == EActorFaction::Player && DamageReceiver->Faction == EActorFaction::Enemy)
		return 1;
	
	else if (DamageDealer->Faction == EActorFaction::Enemy && DamageReceiver->Faction == EActorFaction::Player)
		return 1;

	return 0;
}

void ASoulCharacterBase::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	LOG_FUNC_SUCCESS();
}

void ASoulCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

void ASoulCharacterBase::BindOnAttributesChanged()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetPostureAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandlePostureChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetLeechAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleLeechChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetAttackSpeedAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleAttackSpeedChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMoveSpeedAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleMoveSpeedChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMeleeRangeAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleMeleeRangeChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetPostureStrengthAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandlePostureStrengthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetDefensePowerAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleDefensePowerChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetAttackPowerAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleAttackPowerChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetPostureCrumbleAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandlePostureCrumbleChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetCriticalStrikeAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleCriticalStrikeChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetCriticalMultiAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleCriticalMultiChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandleHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(USoulAttributeSet::GetMaxPostureAttribute())
		.AddUObject(this, &ASoulCharacterBase::HandlePostureChanged);
}

// Called every frame
void ASoulCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASoulCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}