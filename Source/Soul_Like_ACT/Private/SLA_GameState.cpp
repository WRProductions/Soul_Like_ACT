// Fill out your copyright notice in the Description page of Project Settings.

#include "SLA_GameState.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Player/Soul_Like_ACTCharacter.h"
void ASLA_GameState::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ASoul_Like_ACTCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	check(PlayerRef);
}
