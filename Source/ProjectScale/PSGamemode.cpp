#include "PSGamemode.h"
#include "Kismet/GameplayStatics.h"
#include "PSHUD.h"

APSGamemode::APSGamemode()
{
    // TODO: load other gamemode defaults here instead of by level override
    // setting Gamemode settings via c++ example -- idk which I prefer tbh
    static ConstructorHelpers::FClassFinder<APSHUD> HUDClassFinder(TEXT("/Game/ProjectScale/Blueprints/HUD/BP_PSHUD.BP_PSHUD_C"));
    if (HUDClassFinder.Succeeded())
    {
        HUDClass = HUDClassFinder.Class;
        UE_LOG(LogTemp, Display, TEXT("Hud Loaded."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Hud not loaded."));
    }
}

void APSGamemode::StartPlay()
{
    //TODO: set player controller in constructor
    Super::StartPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    FInputModeGameOnly InputModeData;

    PC->SetInputMode(InputModeData);
    PC->bShowMouseCursor = false;
}
