// Written by Jacob Drake - 2023

#include "PSCharacterWidgetComponent.h"
#include "ProjectScale/Widgets/PSCharacterWidget.h"

UPSCharacterWidgetComponent::UPSCharacterWidgetComponent()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> PSCharacterWidgetObj(TEXT("/Game/ProjectScale/Blueprints/Widgets/WBP_PSCharacterWidget.WBP_PSCharacterWidget_C"));
    if (PSCharacterWidgetObj.Succeeded())
    {
        PSCharacterWidgetClass = PSCharacterWidgetObj.Class;
        SetWidgetClass(PSCharacterWidgetClass);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UPSCharacterWidgetComponent() failed to load WBP_PSPickupWidget. Check file path."))
    }

    SetDrawAtDesiredSize(true);
}

void UPSCharacterWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeWidget();
}

void UPSCharacterWidgetComponent::InitializeWidget()
{
    if (PSCharacterWidgetClass)
    {
        WidgetInst = CreateWidget<UPSCharacterWidget>(GetWorld(), PSCharacterWidgetClass);

        if (WidgetInst)
        {
            SetWidget(WidgetInst);
            WidgetInst->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UPSCharacterWidgetComponent failed to initialize widget."))
        }

    }
}
