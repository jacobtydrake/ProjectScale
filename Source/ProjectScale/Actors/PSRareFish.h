// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "PSEnemy.h"
#include "PSRareFish.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSRareFish : public APSEnemy
{
	GENERATED_BODY()

	APSRareFish();

	/* Overrides */
	virtual void UpdateDropChances() override;

};
