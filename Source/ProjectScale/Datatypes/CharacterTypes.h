// Written by Jacob Drake - 2023

#pragma once

#include "CharacterTypes.generated.h"

UENUM(BlueprintType)
    enum class ELastHorizontalMoveDirection : uint8
{
    Left UMETA(DisplayName = "Left"),
    Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
    enum class ELastMoveDirection : uint8
{
    None UMETA(DisplayName = "None"),
    Left UMETA(DisplayName = "Left"),
    Right UMETA(DisplayName = "Right"),
    Up UMETA(DisplayName = "Up"),
    Down UMETA(DisplayName = "Down")
};
