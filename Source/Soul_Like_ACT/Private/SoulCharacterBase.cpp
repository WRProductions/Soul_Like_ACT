// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulCharacterBase.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/SoulGameplayAbility.h"
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

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.f);
	GetCapsuleComponent()->SetCapsuleRadius(60.f);

	TargetIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockIconWidget"));
	TargetIcon->SetupAttachment(RootComponent);
	TargetIcon->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIcon->SetDrawSize(FVector2D{ 35.f,35.f });
}


void ASoulCharacterBase::ToggleLockIcon(bool LockOn)
{

	if (!TargetIcon->IsVisible() && LockOn)
	{
		TargetIcon->SetVisibility(1);
	}
	else if(TargetIcon->IsVisible() && !LockOn)
	{
		TargetIcon->SetVisibility(0);
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

void ASoulCharacterBase::Exec_TryGetHit(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser, const FHitResult &HitInfo, EOnHitRefelction &Outp)
{
	return;
}

void ASoulCharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (Role == ROLE_Authority)
	{
		for (auto TempAbility : AbilityArray)
		{
			if (TempAbility)
				AbilitySystemComponent->GiveAbility(TempAbility);
		}

		// Now apply passives
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
	}

	bAbilitiesInitialized = true;
}

void ASoulCharacterBase::HandleDamage(float DamageAmount, const FHitResult & HitInfo, const FGameplayTagContainer & DamageTags, ASoulCharacterBase * InstigatorCharacter, AActor * DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ASoulCharacterBase::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
	if (bAbilitiesInitialized)
		OnHealthChanged.Broadcast(TArray<float>{GetHealth(), GetMaxHealth()});
}

void ASoulCharacterBase::HandleStaminaChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
	if (bAbilitiesInitialized)
		OnStaminaChanged.Broadcast(TArray<float>{GetStamina(), GetMaxStamina()});
}

void ASoulCharacterBase::HandleMoveSpeedChanged(float DeltaValue, const FGameplayTagContainer & EventTags)
{
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (bAbilitiesInitialized)
		OnMoveSpeedChanged.Broadcast(TArray<float>{GetMoveSpeed(), -1.f});
}

void ASoulCharacterBase::HandleLeechChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnLeechChanged.Broadcast(TArray<float>{GetLeech(), -1.f});
	}
}

void ASoulCharacterBase::HandleTenacityChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
		OnTenacityChanged.Broadcast(TArray<float>{GetTenacity(), -1.f});
}

void ASoulCharacterBase::HandleDefensePowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
		OnDefensePowerChanged.Broadcast(TArray<float>{GetDefensePower(), -1.f});
}

void ASoulCharacterBase::HandleAttackPowerChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
		OnAttackPowerChanged.Broadcast(TArray<float>{GetAttackPower(), -1.f});
}

void ASoulCharacterBase::HandleAttackSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
		OnAttackSpeedChanged.Broadcast(TArray<float>{GetAttackSpeed(), -1.f});
}

UAbilitySystemComponent* ASoulCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

	AddStartupGameplayAbilities();
}

void ASoulCharacterBase::UnPossessed()
{
	Super::UnPossessed();
}

// Called when the game starts or when spawned
void ASoulCharacterBase::BeginPlay()
{
	Super::BeginPlay();
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
