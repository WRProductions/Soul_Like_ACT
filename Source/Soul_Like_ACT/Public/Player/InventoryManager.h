// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

class AWeaponActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearEquiped, const AWeaponActor*, WeaponReference);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()


	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UInventoryManager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AWeaponActor *CurrentWeapon;

	UFUNCTION(BlueprintCallable)
		void EquipGear(AWeaponActor * const Inp);

	UPROPERTY(BlueprintAssignable)
		FOnGearEquiped OnGearEquiped;
};
