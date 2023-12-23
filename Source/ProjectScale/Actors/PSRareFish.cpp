// Written by Jacob Drake - 2023

#include "PSRareFish.h"
#include "ProjectScale/Actors/PSPickupItem.h"

APSRareFish::APSRareFish()
{
	MovementSpeed = 600.0f;
}

void APSRareFish::UpdateDropChances()
{
    CustomDropChances.Add(EPickupItemType::GoldScale, 95.0f);
    CustomDropChances.Add(EPickupItemType::Health, 5.0f);
}
