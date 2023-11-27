// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSCircularEnemySpawner.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(CircularEnemySpawner, Log, All);

class APSEnemy;
class UCurveFloat;

UCLASS()
class PROJECTSCALE_API APSCircularEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    APSCircularEnemySpawner();

protected:
    /* Overrides */
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:

    UFUNCTION()
    void StopSpawningEnemies();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    bool bIsRowSpawner = false;

    UFUNCTION()
    void SpawnEnemies(const int32 NumberOfEnemies);

    UFUNCTION()
    void SpawnSingleEnemy();

    UFUNCTION()
    void InitializeSpawnTimer();

    UFUNCTION()
    void SpawnEnemyRow(const int32 NumberOfEnemies, const float Spacing);

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector PlatformCenter { 0, 0, 0 };

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float PlatformRadius { 10.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnDistanceFromPlatform{ 100.0f };

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float ZOffset { 0.0f };

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<APSEnemy> EnemyClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float RowSpacing = 135.0f;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    int32 InitialNumberOfEnemiesInRow = 1;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    int32 MaxNumberOfEnemiesInRow = 10;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float IncrementInterval = 60.0f; // Seconds


    // curve to control spawn frequency and number of enemies
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TObjectPtr<UCurveFloat> SpawnCurve;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float BaseSpawnRate { .1f };



    FTimerHandle SpawnTimer;

    float CurrentTime;

    void HandleEnemySpawn();

    float GetSpawnFrequency();


    UFUNCTION()
    void IncrementNumberOfEnemiesInRow();



private:

    int32 CurrentNumberOfEnemiesInRow;
    FTimerHandle IncrementTimerHandle;
    FTimerHandle FrequencyCheckTimerHandle;

    float LastSpawnDelay{ -1.0f };

    bool bShouldSpawnEnemies{ true };
};
