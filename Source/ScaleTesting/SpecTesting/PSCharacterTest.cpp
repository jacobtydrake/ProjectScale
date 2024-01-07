#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "ProjectScale/Actors/PSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ScaleTesting/ScaleTesting.h"
#include "Tests/AutomationCommon.h"
#include "EngineUtils.h"
#include "PaperFlipbookComponent.h"

BEGIN_DEFINE_SPEC(FPSCharacterTest, "ScaleTesting.PSCharacterTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

TObjectPtr<UWorld> World;
TObjectPtr<APSCharacter> PSCharacter;

END_DEFINE_SPEC(FPSCharacterTest)

void FPSCharacterTest::Define()
{
    BeforeEach([this]()
    {
        AutomationOpenMap("/Game/ProjectScale/Levels/AutoTestLevel.AutoTestLevel");

        World = GEditor->GetEditorWorldContext().World();
        if (!TestNotNull("World should not be null", World.Get())) return;

        UClass* PSCharacterClass = StaticLoadClass(APSCharacter::StaticClass(), nullptr, TEXT("/Game/ProjectScale/Blueprints/Actors/BP_PSCharacter.BP_PSCharacter_C"));
        if (!TestNotNull("PSCharacterClass should not be null", PSCharacterClass)) return;



        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        const FVector SpawnLocation(0.0f, 0.0f, 100.0f);

        PSCharacter = World->SpawnActor<APSCharacter>(PSCharacterClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
        if (!TestNotNull("PSCharacter should not be null", PSCharacter.Get())) return;

        PSCharacter->SetFlags(RF_Transient);
    });


    It("Character should deal damage", [this]()
        {
            if (!TestNotNull("PSCharacter should not be null", PSCharacter->GetDamageComponent())) return;
            // do other stuff
        });

    It("Character should be animated", [this]()
    {
        // Retrieve the flipbook component from the character
        UPaperFlipbookComponent* FlipbookComponent = PSCharacter->FindComponentByClass<UPaperFlipbookComponent>();
        if (!TestNotNull("Character should have a flipbook component", FlipbookComponent)) return;

        UPaperFlipbook* CurrentFlipbook = FlipbookComponent->GetFlipbook();
        TestNotNull("Flipbook should be playing an animation", CurrentFlipbook);

        bool bIsPlaying = FlipbookComponent->IsPlaying();
        TestTrue("Flipbook should be playing", bIsPlaying);
    });

    AfterEach([this]()
    {
        if (PSCharacter)
        {
            World->DestroyActor(PSCharacter.Get());
        }
        PSCharacter = nullptr;
        World = nullptr;
    });
}