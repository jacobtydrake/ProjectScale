
// Written by Jacob Drake - 2023

#pragma once

#include "UObject/Interface.h"
#include "PSCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPSCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTSCALE_API IPSCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(const float DamageAmount, const FVector& LaunchDirection);
};
