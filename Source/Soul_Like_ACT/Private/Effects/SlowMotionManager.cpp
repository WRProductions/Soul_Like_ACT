// Fill out your copyright notice in the Description page of Project Settings.

#include "Effects/SlowMotionManager.h"
#include "TimerManager.h"

// Sets default values
ASlowMotionManager::ASlowMotionManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = 0;

}

void ASlowMotionManager::CountDownSlowMo()
{

}

void ASlowMotionManager::InitSlowMotion(AActor *Attacker, AActor *Receiver, float HitStrength /*= 1.f*/)
{
}

