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
    void SpawnEnemies(const int32 NumberOfEnemies);

    UFUNCTION()
    void SpawnSingleEnemy();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    FVector PlatformCenter { 0, 0, 0 };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float PlatformRadius { 10.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float ZOffset { 0.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<APSEnemy> EnemyClass;


    // curve to control spawn frequency and number of enemies
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TObjectPtr<UCurveFloat> SpawnCurve;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float BaseSpawnRate { 0.1f };

    FTimerHandle SpawnTimer;

    float CurrentTime;

    void HandleEnemySpawn();
    void InitializeSpawnTimer();
    float GetSpawnFrequency();
};
