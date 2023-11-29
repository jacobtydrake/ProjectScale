// Written by Jacob Drake - 2023

#include "PSWooshEffect.h"
#include "PaperFlipbookComponent.h"
#include "ProjectScale/Utils/PSGlobals.h"

APSWooshEffect::APSWooshEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WooshEffectFlipbook"));
	FlipbookComponent->SetupAttachment(RootComponent);

	FlipbookComponent->SetRelativeRotation(PSGlobals::SpriteRotation);
}

void APSWooshEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void APSWooshEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

