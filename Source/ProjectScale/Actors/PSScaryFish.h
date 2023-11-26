// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "ProjectScale/Actors/PSChasingEnemy.h"
#include "PSScaryFish.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSScaryFish : public APSChasingEnemy
{
	GENERATED_BODY()

	APSScaryFish();

protected:
	virtual void UpdateDropChances() override;
	
};
