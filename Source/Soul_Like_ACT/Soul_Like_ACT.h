// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#define LOG_FUNC_SUCCESS() UE_LOG(LogTemp, Warning, TEXT("%s successful"), *FString(__FUNCTION__))
#define LOG_FUNC_FAIL() UE_LOG(LogTemp, Warning, TEXT("%s failed"), *FString(__FUNCTION__))
#define LOG_FUNC_ERROR(ErrorMessage) UE_LOG(LogTemp, Error, TEXT("%s ERROR -> %s"), *FString(__FUNCTION__), *FString(ErrorMessage))
#define LOG_FUNC_NORMAL(Message) UE_LOG(LogTemp, Warning, TEXT("%s Successful -> %s"), *FString(__FUNCTION__), *FString(Message))