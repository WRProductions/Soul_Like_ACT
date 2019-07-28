// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EKatanaStanceType : uint8
{
	Idle,
	Battao,
	MidStance,
	HeavyStance,
	Guard,
	Invalid	
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Katana,
	Odachi,
	SwordShield,
};