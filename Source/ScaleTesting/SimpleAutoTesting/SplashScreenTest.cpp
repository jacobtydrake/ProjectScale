
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Blueprint/UserWidget.h"
#include "Tests/AutomationCommon.h"
#include "ScaleTesting/ScaleTesting.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSplashScreenTest, "ScaleTesting.SplashScreenTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FSplashScreenTest::RunTest(const FString& Parameters)
{
    const FString LevelName = TEXT("/Game/ProjectScale/Levels/MainMenu.MainMenu");
    const FName WidgetToCheck = "WBP_PSSplashScreen";

    AutomationOpenMap(LevelName);
    AddCommand(new FWaitLatentCommand(1.0f));

    for (TObjectIterator<UUserWidget> Widget; Widget; ++Widget)
    {
        if (Widget->GetName().StartsWith("WBP_PSSplashScreen") && Widget->IsInViewport() && Widget->IsVisible())
        {
            return true;
        }
    }

    UE_LOG(LogScaleTesting, Log, TEXT("%s is not Visible"), *WidgetToCheck.ToString());
    return false;
}