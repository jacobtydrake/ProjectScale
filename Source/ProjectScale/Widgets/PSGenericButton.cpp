// Written by Jacob Drake - 2023


#include "PSGenericButton.h"
#include "Components/Button.h"

void UPSGenericButton::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button)
    {
        Button->OnClicked.AddDynamic(this, &UPSGenericButton::OnButtonClicked);
    }
}

void UPSGenericButton::OnButtonClicked()
{
}
