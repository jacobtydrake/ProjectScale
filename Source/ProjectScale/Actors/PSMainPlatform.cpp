// Written by Jacob Drake - 2023


#include "PSMainPlatform.h"
#include "Components/SphereComponent.h"


APSMainPlatform::APSMainPlatform()
{
	PrimaryActorTick.bCanEverTick = true;


    // Create and initialize the sphere collision component
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    SphereCollision->SetupAttachment(RootComponent); // Assuming you have a RootComponent

    SphereCollision->SetSphereRadius(PlatformRadius);
}

void APSMainPlatform::BeginPlay()
{
	Super::BeginPlay();	

    SphereCollision->SetSphereRadius(PlatformRadius);
}

void APSMainPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Draw a debug sphere
    if (SphereCollision)
    {
        DrawDebugSphere(
            GetWorld(),
            SphereCollision->GetComponentLocation(),
            SphereCollision->GetScaledSphereRadius(),
            32,  // Sphere segments (the more segments, the smoother the sphere)
            FColor::Green,  // Sphere color
            false,  // Persistent lines
            -1,  // Lifetime (negative means one frame)
            0,  // Depth priority
            1.0  // Thickness of lines
        );
    }
}

