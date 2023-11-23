// Written by Jacob Drake - 2023

#include "PSHUD.h"
#include "ProjectScale/Widgets/PSHUDWidget.h"
#include "ProjectScale/Widgets/PSPauseScreen.h"

APSHUD::APSHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> HudWidgetObj(TEXT("/Game/ProjectScale/Blueprints/Widgets/WBP_PSHUDWidget.WBP_PSHUDWidget_C"));
    if (HudWidgetObj.Succeeded())
    {
        HUDWidgetClass = HudWidgetObj.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("APSHUD() failed to load WBP_PSHUDWidget. Check file path."))
    }
    static ConstructorHelpers::FClassFinder<UUserWidget> PauseWidgetObj(TEXT("/Game/ProjectScale/Blueprints/Widgets/Menus/WBP_PSPauseScreen.WBP_PSPauseScreen_C"));

    if (PauseWidgetObj.Succeeded())
    {
        PauseWidgetClass = PauseWidgetObj.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("APSHUD() failed to load WBP_PSPauseScreen. Check file path."))
    }
}

void APSHUD::BeginPlay()
{
    Super::BeginPlay();
    InitializeMainLevelUI();
}

void APSHUD::InitializeMainLevelUI()
{
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UPSHUDWidget>(GetWorld(), HUDWidgetClass);

        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PSHUDWidget is invalid"));
    }

    if (PauseWidgetClass)
    {
        PauseWidgetInstance = CreateWidget<UPSPauseScreen>(GetWorld(), PauseWidgetClass);
        PauseWidgetInstance->AddToViewport();
        PauseWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PSPauseScreen is invalid"));
    }
}

void APSHUD::TogglePauseScreen()
{
    bIsPauseScreenVisible = !bIsPauseScreenVisible;

    if (bIsPauseScreenVisible)
    {
        PauseWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        PauseWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void APSHUD::UpdateHealthWidget(int32 NewHealth)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->UpdateHealth(NewHealth);
    }
}


