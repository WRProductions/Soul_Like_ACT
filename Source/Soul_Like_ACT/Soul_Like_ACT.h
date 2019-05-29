// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"

#define LOG_FUNC_SUCCESS() UE_LOG(LogTemp, Warning, TEXT("%s successful"), *FString(__FUNCTION__))
#define LOG_FUNC_FAIL() UE_LOG(LogTemp, Warning, TEXT("%s failed"), *FString(__FUNCTION__))
#define LOG_FUNC_ERROR(ErrorMessage) UE_LOG(LogTemp, Error, TEXT("%s ERROR -> %s"), *FString(__FUNCTION__), *FString(ErrorMessage))