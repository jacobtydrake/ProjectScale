// Written by Jacob Drake - 2023


#include "PSDestructionZone.h"
#include "Components/BoxComponent.h"

APSDestructionZone::APSDestructionZone()
{
	PrimaryActorTick.bCanEverTick = true;

    // Create and attach the trigger volume
    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    // Configure the trigger volume
    TriggerVolume->SetCollisionProfileName(TEXT("Trigger"));
    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APSDestructionZone::OnOverlapBegin);
}


void APSDestructionZone::BeginPlay()
{
	Super::BeginPlay();
}

void APSDestructionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APSDestructionZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this))
    {
        // Destroy the actor
        OtherActor->Destroy();
    }
}

