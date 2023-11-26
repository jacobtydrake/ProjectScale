// Written by Jacob Drake - 2023


#include "PSScaryFish.h"
#include "ProjectScale/Actors/PSPickupItem.h"

APSScaryFish::APSScaryFish()
{
	MaxHealth = 3;
	MovementSpeed = 90.0f;

	// attack params
	AttackRange = 850;
	TotalLaunchDuration = 1.f;
	AttackHurtDuration = 1.f;
	AttackToLaunchDelay = 0.5f;
	TotalAttackDuration = 2.5f;
	AttackCooldown = 4.0f;
}

void APSScaryFish::UpdateDropChances()
{
	CustomDropChances.Add(EPickupItemType::BlackScale, 90.0f);
	CustomDropChances.Add(EPickupItemType::Health, 5.0f);
	CustomDropChances.Add(EPickupItemType::ScreenWipe, 5.0f);
}
