// Written by Jacob Drake - 2023


#include "PSScaryFish.h"
#include "ProjectScale/Actors/PSPickupItem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

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
	CustomDropChances.Add(EPickupItemType::Attack, 5.0f);
}

void APSScaryFish::StartLaunch()
{
	Super::StartLaunch();

	LaunchSoundRepetitions = 5;

	UE_LOG(LogTemp, Warning, TEXT("StartLaunch called. Resetting LaunchSoundRepetitions to %d"), LaunchSoundRepetitions);

	// Clear any existing timer and play the launch sound
	GetWorld()->GetTimerManager().ClearTimer(TimerHandleLaunchSound);
	PlayLaunchSound();
}

void APSScaryFish::PlayLaunchSound()
{
	if (LaunchSound && LaunchSoundRepetitions > 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
		LaunchSoundRepetitions--;
		if (LaunchSoundRepetitions > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandleLaunchSound, this, &APSScaryFish::PlayLaunchSound, LaunchSoundDelay, false);
		}
	}
}