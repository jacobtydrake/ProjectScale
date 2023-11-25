// Written by Jacob Drake - 2023


#include "PSRareFish.h"
#include "ProjectScale/Actors/PSPickupItem.h"

APSRareFish::APSRareFish()
{
	MovementSpeed = 500.0f;
}

void APSRareFish::UpdateDropChances()
{
    CustomDropChances.Add(EPickupItemType::GoldScale, 95.0f);
    CustomDropChances.Add(EPickupItemType::ScreenWipe, 5.0f);
}
