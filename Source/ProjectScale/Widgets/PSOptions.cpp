// Written by Jacob Drake - 2023

#include "PSOptions.h"
#include "Components/ComboBoxString.h"
#include "ProjectScale/Widgets/PSGenericButton.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

void UPSOptions::NativeConstruct()
{
	Super::NativeConstruct();

	if (WindowModeOptions)
	{
		WindowModeOptions->OnSelectionChanged.AddDynamic(this, &UPSOptions::OnWindowModeChanged);
	}
	if (ApplyButton)
	{
		ApplyButton->OnPSButtonClicked.AddDynamic(this, &UPSOptions::OnApplySettings);
	}

	if (ResolutionOptions)
	{
		TArray<FIntPoint> Resolutions;
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		FIntPoint CurrentResolution = UserSettings ? UserSettings->GetScreenResolution() : FIntPoint(0, 0);
		FString CurrentResString = FString::Printf(TEXT("%dx%d"), CurrentResolution.X, CurrentResolution.Y);

		UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
		for (const FIntPoint& Resolution : Resolutions)
		{
			FString ResString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
			ResolutionOptions->AddOption(ResString);

			// Set the current resolution as the selected option
			if (ResString == CurrentResString)
			{
				ResolutionOptions->SetSelectedOption(ResString);
			}
		}
	}
}

void UPSOptions::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// don't need to do anything here
}

void UPSOptions::OnVsyncChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	bVSyncEnabled = SelectedItem == "Enabled";
}

void UPSOptions::OnApplySettings()
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		// window mode
		if (WindowModeOptions->GetSelectedOption() == "Fullscreen")
		{
			UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else if (WindowModeOptions->GetSelectedOption() == "Borderless")
		{
			UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		}
		else if (WindowModeOptions->GetSelectedOption() == "Windowed")
		{
			UserSettings->SetFullscreenMode(EWindowMode::Windowed);
		}

		// vsync
		UserSettings->SetVSyncEnabled(bVSyncEnabled);

		// resolution
		if (ResolutionOptions)
		{
			FString SelectedOption = ResolutionOptions->GetSelectedOption();
			TArray<FString> ResArray;
			SelectedOption.ParseIntoArray(ResArray, TEXT("x"), true);

			if (ResArray.Num() == 2)
			{
				int32 ResX = FCString::Atoi(*ResArray[0]);
				int32 ResY = FCString::Atoi(*ResArray[1]);
				UserSettings->SetScreenResolution(FIntPoint(ResX, ResY));

				UE_LOG(LogTemp, Warning, TEXT("New Resolution X: %d Y: %d"), ResX, ResY);
			}
		}

		UserSettings->ApplySettings(false);
		UserSettings->SaveSettings();
	}
}


