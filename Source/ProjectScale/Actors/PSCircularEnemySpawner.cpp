// Written by Jacob Drake - 2023

#include "PSCircularEnemySpawner.h"
#include "ProjectScale/Actors/PSEnemy.h"
#include "Curves/CurveFloat.h"

DEFINE_LOG_CATEGORY(CircularEnemySpawner);

APSCircularEnemySpawner::APSCircularEnemySpawner(): CurrentTime(0), CurrentNumberOfEnemiesInRow(1)
{
    PrimaryActorTick.bCanEverTick = true;
}

void APSCircularEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    CurrentTime = 0.0f;
    CurrentNumberOfEnemiesInRow = InitialNumberOfEnemiesInRow;

    CurrentTime = 0.0f;
    InitializeSpawnTimer();

    GetWorldTimerManager().SetTimer(IncrementTimerHandle, this, &APSCircularEnemySpawner::IncrementNumberOfEnemiesInRow, IncrementInterval, true);

    //DrawDebugCircle(
    //    GetWorld(),
    //    PlatformCenter,
    //    PlatformRadius,
    //    50,         
    //    FColor::Green,
    //    true,       
    //    -1,        
    //    0,          
    //    10.0f       
    //);
}

void APSCircularEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CurrentTime += DeltaTime;
}

void APSCircularEnemySpawner::StopSpawningEnemies()
{
    bShouldSpawnEnemies = false;
    GetWorldTimerManager().ClearTimer(SpawnTimer);
    GetWorldTimerManager().ClearTimer(IncrementTimerHandle);
}

void APSCircularEnemySpawner::SpawnEnemies(const int32 NumberOfEnemies) const
{
    if (!EnemyClass || NumberOfEnemies <= 0) return;

    for (int32 i = 0; i < NumberOfEnemies; ++i)
    {
        SpawnSingleEnemy();
    }
}

void APSCircularEnemySpawner::SpawnSingleEnemy() const
{
    if (!EnemyClass)
    {
        UE_LOG(CircularEnemySpawner, Error, TEXT("EnemyClass is not set."));
        return;
    }

    // random angle for spawn location
    const float Angle = FMath::RandRange(0.f, 360.f);

    //// platform edge location -- used for debugging
    //FVector PlatformEdgeLocation = PlatformCenter
    //    + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * PlatformRadius,
    //        FMath::Sin(FMath::DegreesToRadians(Angle)) * PlatformRadius,
    //        ZOffset);

    // the spawn location should be outside the platform by 'SpawnDistanceFromPlatform'
    const FVector SpawnLocation = PlatformCenter
        + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * (PlatformRadius + SpawnDistanceFromPlatform),
            FMath::Sin(FMath::DegreesToRadians(Angle)) * (PlatformRadius + SpawnDistanceFromPlatform),
            ZOffset);

    //DrawDebugLine(
    //    GetWorld(),
    //    PlatformEdgeLocation,
    //    SpawnLocation,
    //    FColor::Red,
    //    false,
    //    10.0f,
    //    0,
    //    2.0f
    //);
    
    if (APSEnemy* NewEnemy = GetWorld()->SpawnActor<APSEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator))
    {
        // calculate a random point within the platform for the enemy to move towards
        const float RandomAngle = FMath::RandRange(0.f, 360.f);
        const float RandomRadius = FMath::RandRange(0.f, PlatformRadius); // adjust radius within the platform
        const FVector TargetPoint = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomRadius,
            FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomRadius,
            ZOffset);

        // Calculate the direction vector from the enemy to the target point
        const FVector Direction = (TargetPoint - SpawnLocation).GetSafeNormal();
        NewEnemy->InitializeDirection(Direction);
        UE_LOG(CircularEnemySpawner, Display, TEXT("Enemy successfully spawned."));
    }
    else
    {
        UE_LOG(CircularEnemySpawner, Warning, TEXT("Failed to spawn enemy at location: %s"), *SpawnLocation.ToString());
    }
}

void APSCircularEnemySpawner::InitializeSpawnTimer()
{
    const float InitialSpawnDelay = GetSpawnFrequency();
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, InitialSpawnDelay, false);
}

void APSCircularEnemySpawner::HandleEnemySpawn()
{
    if (!bShouldSpawnEnemies) return;

    if (bIsRowSpawner)
    {
        SpawnEnemyRow(CurrentNumberOfEnemiesInRow, RowSpacing);
    }
    else
    {
        SpawnSingleEnemy();
    }

    // update timer for next spawn based on the curve
    const float NextSpawnDelay = GetSpawnFrequency();
    UE_LOG(CircularEnemySpawner, Log, TEXT("Spawning enemies. Next spawn in %f seconds."), NextSpawnDelay);

    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, NextSpawnDelay, false);

    CurrentTime += NextSpawnDelay;
}

float APSCircularEnemySpawner::GetSpawnFrequency() const
{
    if (SpawnCurve)
    {
        const float CurveValue = SpawnCurve->GetFloatValue(CurrentTime);

        const float Frequency = 1.0f / FMath::Max(BaseSpawnRate * CurveValue, 0.01f);

        const FString EnemyClassName = EnemyClass ? EnemyClass->GetName() : TEXT("None");
        UE_LOG(CircularEnemySpawner, Log, TEXT("Calculating Spawn Frequency for Enemy Class: %s. Current Time: %f, Curve Value: %f, Calculated Frequency: %f"), *EnemyClassName, CurrentTime, CurveValue, Frequency);

        return Frequency;
    }
    return 1.0f / BaseSpawnRate; // default frequency if no curve is set
}


void APSCircularEnemySpawner::IncrementNumberOfEnemiesInRow()
{
    if (CurrentNumberOfEnemiesInRow < MaxNumberOfEnemiesInRow)
    {
        CurrentNumberOfEnemiesInRow++;
    }
    else
    {
        if (IncrementTimerHandle.IsValid())
        {
            GetWorldTimerManager().ClearTimer(IncrementTimerHandle);
        }
    }
}

void APSCircularEnemySpawner::SpawnEnemyRow(const int32 NumberOfEnemies, const float Spacing) const
{
    if (!EnemyClass || NumberOfEnemies <= 0) return;

    // math hard, thanks gpt

    // random angle for row start position
    const float StartAngle = FMath::RandRange(0.f, 360.f);
    const FVector StartPosition = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(StartAngle)) * (PlatformRadius + SpawnDistanceFromPlatform), FMath::Sin(FMath::DegreesToRadians(StartAngle)) * PlatformRadius, ZOffset);

    // tangent direction on XY plane
    const FVector TangentDirection = FVector(-FMath::Sin(FMath::DegreesToRadians(StartAngle)), FMath::Cos(FMath::DegreesToRadians(StartAngle)), 0.0f);

    // determine a line of target points inside the circle parallel to the row
    const FVector BaseTargetPoint = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(StartAngle + 90.f)) * (PlatformRadius * 0.5f), FMath::Sin(FMath::DegreesToRadians(StartAngle + 90.f)) * (PlatformRadius * 0.5f), ZOffset);

    for (int32 i = 0; i < NumberOfEnemies; ++i)
    {
        FVector SpawnLocation = StartPosition + TangentDirection * (i * Spacing);
        FVector TargetPoint = BaseTargetPoint + TangentDirection * (i * Spacing);
        if (APSEnemy* NewEnemy = GetWorld()->SpawnActor<APSEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator))
        {
            FVector Direction = (TargetPoint - SpawnLocation).GetSafeNormal();
            NewEnemy->InitializeDirection(Direction);
        }
        else
        {
            UE_LOG(CircularEnemySpawner, Warning, TEXT("Failed to spawn enemy in row at location: %s"), *SpawnLocation.ToString());
        }
    }
}
