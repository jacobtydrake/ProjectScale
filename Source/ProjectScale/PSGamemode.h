

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PSGamemode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSGamemode : public AGameMode
{
	GENERATED_BODY()

protected:
	virtual void StartPlay() override;
	
};