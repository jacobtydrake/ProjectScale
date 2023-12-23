// Written by Jacob Drake - 2023

#pragma once

#include "UObject/ObjectMacros.h"
#include "Curves/CurveFloat.h"
#include "PSEnemyType.generated.h"

USTRUCT(BlueprintType)
struct FPSEnemyType
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCurveFloat> SpawnCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseSpawnRate{ 0.0f };
};
