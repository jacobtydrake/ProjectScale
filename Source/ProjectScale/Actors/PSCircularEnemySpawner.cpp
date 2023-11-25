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
    FVector SpawnLocation = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(Angle)) * (PlatformRadius + SpawnDistanceFromPlatform), FMath::Sin(FMath::DegreesToRadians(Angle)) * PlatformRadius, ZOffset);

    APSEnemy* NewEnemy = GetWorld()->SpawnActor<APSEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
    if (NewEnemy)
    {
        // calculate a random point within the platform for the enemy to move towards
        float RandomAngle = FMath::RandRange(0.f, 360.f);
        float RandomRadius = FMath::RandRange(0.f, PlatformRadius); // adjust radius within the platform
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

void APSCircularEnemySpawner::InitializeSpawnTimer()
{
    float InitialSpawnDelay = GetSpawnFrequency();
    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, InitialSpawnDelay, false);
}

void APSCircularEnemySpawner::HandleEnemySpawn()
{
    if (bIsRowSpawner)
    {
        SpawnEnemyRow(CurrentNumberOfEnemiesInRow, RowSpacing);
    }
    else
    {
        SpawnSingleEnemy();
    }

    // update timer for next spawn based on the curve
    float NextSpawnDelay = GetSpawnFrequency();
    UE_LOG(CircularEnemySpawner, Log, TEXT("Spawning enemies. Next spawn in %f seconds."), NextSpawnDelay);

    GetWorldTimerManager().SetTimer(SpawnTimer, this, &APSCircularEnemySpawner::HandleEnemySpawn, NextSpawnDelay, false);

    CurrentTime += NextSpawnDelay;
}

float APSCircularEnemySpawner::GetSpawnFrequency()
{
    if (SpawnCurve)
    {
        float CurveValue = SpawnCurve->GetFloatValue(CurrentTime);

        float Frequency = 1.0f / FMath::Max(BaseSpawnRate * CurveValue, 0.01f);

        FString EnemyClassName = EnemyClass ? EnemyClass->GetName() : TEXT("None");
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

void APSCircularEnemySpawner::SpawnEnemyRow(const int32 NumberOfEnemies, const float Spacing)
{
    if (!EnemyClass || NumberOfEnemies <= 0) return;

    // math hard, thanks gpt

    // random angle for row start position
    float StartAngle = FMath::RandRange(0.f, 360.f);
    FVector StartPosition = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(StartAngle)) * (PlatformRadius + SpawnDistanceFromPlatform), FMath::Sin(FMath::DegreesToRadians(StartAngle)) * PlatformRadius, ZOffset);

    // tangent direction on XY plane
    FVector TangentDirection = FVector(-FMath::Sin(FMath::DegreesToRadians(StartAngle)), FMath::Cos(FMath::DegreesToRadians(StartAngle)), 0.0f);

    // determine a line of target points inside the circle parallel to the row
    FVector BaseTargetPoint = PlatformCenter + FVector(FMath::Cos(FMath::DegreesToRadians(StartAngle + 90.f)) * (PlatformRadius * 0.5f), FMath::Sin(FMath::DegreesToRadians(StartAngle + 90.f)) * (PlatformRadius * 0.5f), ZOffset);

    for (int32 i = 0; i < NumberOfEnemies; ++i)
    {
        FVector SpawnLocation = StartPosition + TangentDirection * (i * Spacing);
        FVector TargetPoint = BaseTargetPoint + TangentDirection * (i * Spacing);
        APSEnemy* NewEnemy = GetWorld()->SpawnActor<APSEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);

        if (NewEnemy)
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
