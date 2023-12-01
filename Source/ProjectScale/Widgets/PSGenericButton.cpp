// Written by Jacob Drake - 2023


#include "PSGenericButton.h"
#include "Components/Button.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

void UPSGenericButton::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UPSGenericButton::OnButtonClicked);
    }
}

void UPSGenericButton::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (OnHoverSound)
    {
        UGameplayStatics::PlaySound2D(this, OnHoverSound);
    }
}

void UPSGenericButton::OnButtonClicked()
{
    if (OnClickSound)
    {
        UGameplayStatics::PlaySound2D(this, OnClickSound);
    }
}
