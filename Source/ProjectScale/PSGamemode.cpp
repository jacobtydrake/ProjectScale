#include "PSGamemode.h"
#include "Kismet/GameplayStatics.h"


void APSGamemode::StartPlay()
{
    Super::StartPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    FInputModeGameOnly InputModeData;

    PC->SetInputMode(InputModeData);
    PC->bShowMouseCursor = false;  // Hide the mouse cursor.
}
