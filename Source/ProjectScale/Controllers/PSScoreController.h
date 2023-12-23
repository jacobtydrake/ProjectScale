// Written by Jacob Drake - 2023

#pragma once

#include "PSScoreController.generated.h"

class APSScaleWithPhysics;
class APSHUD;
class USound;

enum class EPickupItemType : uint8;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API UPSScoreController : public UObject
{
	GENERATED_BODY()

	UPSScoreController();

public:
	UFUNCTION()
	void AddItemToScore(const EPickupItemType ItemType);

	UFUNCTION()
	int32 GetTotalScore() const;

	UFUNCTION()
	TMap<EPickupItemType, int32> GetItemPickupCounts() const;

	UFUNCTION()
	void StartSpawningScales(const FVector& SpawnLocation, const float SpawnDuration, const float DelayBetweenTypes, const FVector &NewSpawnOffsetRange);
	
	UFUNCTION()
	void InitializeCachedHUD(APSHUD* HUDtoCache);

	UFUNCTION()
	void InitializeSound(USoundBase* Sound);

private:

	void InitializeScaleTypesToSpawn();

	TSubclassOf<APSScaleWithPhysics> ScaleWithPhysicsClass;

	void SpawnNextScale();

	float GetSpawnRateForItemType(EPickupItemType ItemType) const;

	void StartSpawningCurrentType();
	 
	void MoveToNextItemType();

	int32 TotalScore = 0;

	TMap<EPickupItemType, int32> ItemPickupCounts;

	static int32 CalculateScoreForItem(EPickupItemType ItemType);

	TArray<EPickupItemType> ScaleTypesToSpawn;

	int32 CurrentScaleTypeIndex;


	bool HasNextScaleType() const;

	FVector SpawnLocation;
	float SpawnDuration{ 5.0f };
	float DelayBetweenTypes{ 3.0f };
	float CurrentSpawnRate;
	EPickupItemType CurrentItemType;
	int32 CurrentItemCount;
	int32 ScalesSpawned;
	FTimerHandle ScaleSpawnTimerHandle;
	FVector SpawnOffsetRange;
	int32 MaxScalesPerType{ 150 };

	/* cached reference to hud. */
	TObjectPtr<APSHUD> CachedHUD;

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> ScaleSpawnSound;
};
