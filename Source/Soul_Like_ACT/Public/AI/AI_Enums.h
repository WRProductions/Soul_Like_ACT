// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_Enums.generated.h"

UENUM(BlueprintType)
enum class EAgreesiveActions : uint8
{
	Attack,
	DashBack,
	RunToTarget,
	Strafe,
};

UENUM(BlueprintType)
enum class EDefensiveActions : uint8
{
	Strafe,
	RunToTarget,
};