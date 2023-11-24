// Written by Jacob Drake - 2023


#include "PSPickupItemWidgetComponent.h"
#include "ProjectScale/Widgets/PSPickupWidget.h"
#include "ProjectScale/Actors/PsPickupItem.h"

UPSPickupItemWidgetComponent::UPSPickupItemWidgetComponent()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> PSPickupWidgetObj(TEXT("/Game/ProjectScale/Blueprints/Widgets/WBP_PSPickupWidget.WBP_PSPickupWidget_C"));
    if (PSPickupWidgetObj.Succeeded())
    {
        PSPickupWidgetClass = PSPickupWidgetObj.Class;
        SetWidgetClass(PSPickupWidgetClass);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UPSPickupItemWidgetComponent() failed to load WBP_PSPickupWidget. Check file path."))
    }

    SetDrawAtDesiredSize(true);
}

void UPSPickupItemWidgetComponent::ActivePickupItemWidget(const EPickupItemType ItemType)
{
    switch (ItemType)
    {
    case EPickupItemType::OrangeScale:
        WidgetInst->SetText("+1");
        break;
    case EPickupItemType::BlueScale:
        WidgetInst->SetText("+2");
        break;
    case EPickupItemType::Speed:
        WidgetInst->SetText("+Speed");
        break;
    case EPickupItemType::Health:
        WidgetInst->SetText("+Health");
        break;
    case EPickupItemType::ScreenWipe:
        WidgetInst->SetText("ScreenWipe");
        break;
    }

    WidgetInst->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    WidgetInst->PlayPickupAnimation();

}

void UPSPickupItemWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializeWidget();
}

void UPSPickupItemWidgetComponent::InitializeWidget()
{
    if (PSPickupWidgetClass)
    {
        WidgetInst = CreateWidget<UPSPickupWidget>(GetWorld(), PSPickupWidgetClass);

        if (WidgetInst)
        {
            SetWidget(WidgetInst);
            WidgetInst->SetVisibility(ESlateVisibility::Collapsed);
            //WidgetInst->
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UPSPickupItemWidgetComponent failed to initialize widget: UPSPickupWidget."))
        }
    }
}
