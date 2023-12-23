// Written by Jacob Drake - 2023

#pragma once

#include "ProjectScale/Actors/PSChasingEnemy.h"
#include "PSScaryFish.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSScaryFish : public APSChasingEnemy
{
	GENERATED_BODY()

	APSScaryFish();

protected:
	/* Enemy Override */
	virtual void UpdateDropChances() override;

	/* Chasing Enemy Override */
	virtual void StartLaunch() override;

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	int32 LaunchSoundRepetitions{ 4 };
	UPROPERTY(EditAnywhere, Category = "Sound")
	float LaunchSoundDelay{ 0.3f };

	FTimerHandle TimerHandleLaunchSound;

	void PlayLaunchSound();
};
