// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulFSMComponent.h"

USoulFSMComponent::USoulFSMComponent()
{
	AddState(0, FName("Idle"));
	AddState(1, FName("Attack"));
	AddState(2, FName("Run"));
}
