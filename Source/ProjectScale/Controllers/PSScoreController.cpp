// Written by Jacob Drake - 2023


#include "PSScoreController.h"
#include "ProjectScale/Actors/PSPickupItem.h"
#include "ProjectScale/Actors/PSScaleWithPhysics.h"
#include "ProjectScale/PSHUD.h"

UPSScoreController::UPSScoreController()
{
    static ConstructorHelpers::FClassFinder<APSScaleWithPhysics> ScaleBPClass(TEXT("/Game/ProjectScale/Blueprints/Actors/BP_PSScaleWithPhysics.BP_PSScaleWithPhysics_C"));
    if (ScaleBPClass.Class)
    {
        ScaleWithPhysicsClass = ScaleBPClass.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UPSScoreController failed to load WBP_PSScaleWithPhysics. Check file path."))
    }

    InitializeScaleTypesToSpawn();
    CurrentScaleTypeIndex = 0;
}

void UPSScoreController::InitializeScaleTypesToSpawn()
{
    ScaleTypesToSpawn = {
        EPickupItemType::OrangeScale,
        EPickupItemType::BlueScale,
        EPickupItemType::PurpleScale,
        EPickupItemType::BlackScale,
        EPickupItemType::GoldScale
    };
}

void UPSScoreController::AddItemToScore(EPickupItemType ItemType)
{
    int32 ScoreToAdd = CalculateScoreForItem(ItemType);
    TotalScore += ScoreToAdd;
    ItemPickupCounts.FindOrAdd(ItemType)++;
}

int32 UPSScoreController::GetTotalScore() const
{
    return TotalScore;
}

TMap<EPickupItemType, int32> UPSScoreController::GetItemPickupCounts() const
{
    return ItemPickupCounts;
}

void UPSScoreController::StartSpawningScales(const FVector& NewSpawnLocation, const float NewSpawnDuration, const float NewDelayBetweenTypes, const FVector& NewSpawnOffsetRange)
{
    SpawnLocation = NewSpawnLocation;
    SpawnDuration = NewSpawnDuration;
    DelayBetweenTypes = NewDelayBetweenTypes;
    SpawnOffsetRange = NewSpawnOffsetRange;

    if (HasNextScaleType())
    {
        CurrentItemType = ScaleTypesToSpawn[CurrentScaleTypeIndex];
        StartSpawningCurrentType();
    }
    else
    {
        if (CachedHUD)
        {
            CachedHUD->UpdateScoreScreenBoxVisibility(CurrentScaleTypeIndex);
        }
    }
}

void UPSScoreController::SpawnNextScale()
{
    if (ScaleWithPhysicsClass)
    {
        FVector RandomOffset(
            FMath::RandRange(-SpawnOffsetRange.X, SpawnOffsetRange.X),
            FMath::RandRange(-SpawnOffsetRange.Y, SpawnOffsetRange.Y),
            0.0f
        );

        FVector RandomSpawnLocation = SpawnLocation + RandomOffset;

        APSScaleWithPhysics* NewScale = GetWorld()->SpawnActor<APSScaleWithPhysics>(ScaleWithPhysicsClass, RandomSpawnLocation, FRotator::ZeroRotator);
        if (NewScale)
        {
            NewScale->SetSelectedFlipBook(CurrentItemType);
        }

        ScalesSpawned++;

        if (ScalesSpawned >= CurrentItemCount || ScalesSpawned >= MaxScalesPerType)
        {
            GetWorld()->GetTimerManager().ClearTimer(ScaleSpawnTimerHandle);
            MoveToNextItemType();
        }
    }
}

void UPSScoreController::MoveToNextItemType()
{
    if (CachedHUD)
    {
        CachedHUD->UpdateScoreScreenBoxVisibility(CurrentScaleTypeIndex);
    }

    CurrentScaleTypeIndex++;

    if (HasNextScaleType())
    {
        CurrentItemType = ScaleTypesToSpawn[CurrentScaleTypeIndex];
        GetWorld()->GetTimerManager().SetTimer(ScaleSpawnTimerHandle, this, &UPSScoreController::StartSpawningCurrentType, DelayBetweenTypes, false);
    }
}

void UPSScoreController::InitializeCachedHUD(APSHUD* HUDtoCache)
{
    CachedHUD = HUDtoCache;

    if (CachedHUD == NULL)
    {
        UE_LOG(LogTemp, Error, TEXT("UPSScoreController failed to Cache PSHUD"));
    }
}

float UPSScoreController::GetSpawnRateForItemType(EPickupItemType ItemType) const
{
    int32 ItemCount = GetItemPickupCounts().FindRef(ItemType);
    if (ItemCount <= 0) return 0.0f;

    // Calculate spawn interval based on total duration and item count
    float SpawnInterval = SpawnDuration / static_cast<float>(ItemCount);
    return SpawnInterval;
}

void UPSScoreController::StartSpawningCurrentType()
{
    CurrentItemCount = GetItemPickupCounts().FindRef(CurrentItemType);

    // Skip to next type if no items to spawn
    if (CurrentItemCount <= 0)
    {
        MoveToNextItemType();
        return;
    }

    CurrentSpawnRate = GetSpawnRateForItemType(CurrentItemType);
    ScalesSpawned = 0;

    // Set timer for spawning scales
    GetWorld()->GetTimerManager().SetTimer(ScaleSpawnTimerHandle, this, &UPSScoreController::SpawnNextScale, CurrentSpawnRate, true);
}

int32 UPSScoreController::CalculateScoreForItem(EPickupItemType ItemType) const
{
    switch (ItemType)
    {
    case EPickupItemType::BlueScale:
        return 1;
    case EPickupItemType::OrangeScale:
        return 2; 
    case EPickupItemType::PurpleScale:
        return 3;
    case EPickupItemType::BlackScale:
        return 10;
    case EPickupItemType::GoldScale:
        return 20;
    default:
        return 0;
    }
}

bool UPSScoreController::HasNextScaleType() const
{
    return CurrentScaleTypeIndex < ScaleTypesToSpawn.Num();
}
