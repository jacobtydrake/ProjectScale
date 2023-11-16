// Written by Jacob Drake - 2023


#include "PSCircularEnemySpawner.h"
#include "ProjectScale/Actors/PSEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Curves/CurveFloat.h"

DEFINE_LOG_CATEGORY(CircularEnemySpawner);

APSCircularEnemySpawner::APSCircularEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APSCircularEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
    CurrentTime = 0.0f;
    InitializeSpawnTimer();
}

void APSCircularEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APSCircularEnemySpawner::SpawnEnemies(const int32 NumberOfEnemies)
{
    if (!EnemyClass || NumberOfEnemies <= 0) return;

    for (int32 i = 0; i < NumberOfEnemies; ++i)
    {
        SpawnSingleEnemy();
    }
}

void APSCircularEnemySpawner::SpawnSingleEnemy()
{
    if (!EnemyClass)
    {
        UE_LOG(CircularEnemySpawner, Error, TEXT("EnemyClass is not set."));
        return;
    }

    // random angle for spawn location
    float Angle = FMath::RandRange(0.f, 360.f);
    FVector SpawnLocation = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * PlatformRadius, FMath::Sin(FMath::DegreesToRadians(Angle)) * PlatformRadius, ZOffset);

    APSEnemy* NewEnemy = GetWorld()->SpawnActor<APSEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
    if (NewEnemy)
    {
        // calculate a random point within the platform for the enemy to move towards
        float RandomAngle = FMath::RandRange(0.f, 360.f);
        float RandomRadius = FMath::RandRange(0.f, PlatformRadius - DirectionTolerance); // Adjust radius within the platform
        FVector TargetPoint = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomRadius, FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomRadius, ZOffset);

        // calculate the direction vector from the enemy to the target point
        FVector Direction = (TargetPoint - SpawnLocation).GetSafeNormal();
        NewEnemy->InitializeDirection(Direction);
        UE_LOG(CircularEnemySpawner, Display, TEXT("Enemy successfully spawned."));
    }
    else
    {
        UE_LOG(CircularEnemySpawner, Warning, TEXT("Failed to spawn enemy at location: %s"), *SpawnLocation.ToString());
    }
}

void APSCircularEnemySpawner::HandleEnemySpawn()
{
    int32 NumEnemiesToSpawn = FMath::RoundToInt(SpawnCurve->GetFloatValue(CurrentTime));
    SpawnEnemies(NumEnemiesToSpawn);

    // update timer for next spawn
    float NextSpawnDelay = GetSpawnFrequency();
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, NextSpawnDelay, false);

    CurrentTime += NextSpawnDelay;
}

void APSCircularEnemySpawner::InitializeSpawnTimer()
{
    float InitialSpawnDelay = GetSpawnFrequency();
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, InitialSpawnDelay, false);
}

float APSCircularEnemySpawner::GetSpawnFrequency()
{
    if (SpawnCurve)
    {
        //ensure non-negative and non-zero frequency
        return FMath::Max(SpawnCurve->GetFloatValue(CurrentTime), 0.1f);
    }
    return 1.0f;
}
