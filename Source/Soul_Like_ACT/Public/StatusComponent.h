// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorHealthChanged, int32, ActorCurrentHealth, int32, ActorMaxHealth);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnActorHealthChangedNative, int32, int32);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int32 Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int32 Damage;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() { return ((float)Health / MaxHealth); }

	UFUNCTION(BlueprintCallable)
	void TakeDamage(int32 DamageTake) 
	{ 
		Health = FMath::Clamp(Health -= DamageTake, 0, MaxHealth); 
		if (OnActorHealthChanged.IsBound())
			OnActorHealthChanged.Broadcast(Health, MaxHealth);
	}

	UPROPERTY(BlueprintAssignable)
	FOnActorHealthChanged OnActorHealthChanged;
};