
#include "PSDamageComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UPSDamageComponent::UPSDamageComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Create the box component and attach it to the component's owner
    DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
    DamageCollision->SetBoxExtent(DamageCollisionSize);
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetRelativeLocation(DamageCollisionRelativeLocation);
    DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &UPSDamageComponent::OnDamageCollisionOverlap);
}

TObjectPtr<UBoxComponent> UPSDamageComponent::GetDamageCollisionBox()
{
    return DamageCollision;
}

void UPSDamageComponent::ActivateDamageCollision()
{
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageCollision->SetGenerateOverlapEvents(true);

    bIsDamageCollisionActive = true;
}

void UPSDamageComponent::DeactivateDamageCollision()
{
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetGenerateOverlapEvents(false);

    bIsDamageCollisionActive = false;
}

void UPSDamageComponent::ToggleRelativePositioning(const bool bShouldActivate)
{
    bIsRelativePositioningActive = true;
}

void UPSDamageComponent::BeginPlay()
{
	Super::BeginPlay();

    if (AActor* OwnerActor = GetOwner())
    {
        if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(OwnerActor->GetRootComponent()))
        {
            DamageCollision->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            DamageCollision->SetRelativeLocation(DamageCollisionRelativeLocation);
        }
    }
}

void UPSDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsRelativePositioningActive && GetOwner())
    {
        AActor* Owner = GetOwner();
        FVector MovementDirection = Owner->GetVelocity().GetSafeNormal();
        FVector NewRelativeLocation = FVector::ZeroVector;

        if (MovementDirection == FVector::ZeroVector)
        {
            DeactivateDamageCollision();
            return;
        }

        float HorizontalOffset = 35.f;
        float VerticalOffset = 35.f;
        float MovementThreshold = 0.5f;

        // Adjust the relative location based on movement direction with threshold
        if (FMath::Abs(MovementDirection.X) > MovementThreshold)
        {
            NewRelativeLocation.X = MovementDirection.X > 0 ? HorizontalOffset : -HorizontalOffset;
        }

        if (FMath::Abs(MovementDirection.Y) > MovementThreshold)
        {
            NewRelativeLocation.Y = MovementDirection.Y > 0 ? VerticalOffset : -VerticalOffset;
        }

        // Update the damage collision's location
        DamageCollision->SetRelativeLocation(NewRelativeLocation);

    }

    // Debug visualization
    if (DamageCollision->IsCollisionEnabled())
    {
        DrawDebugBox
        (
            GetWorld(),
            DamageCollision->GetComponentLocation(),
            DamageCollision->GetScaledBoxExtent(),
            FQuat(DamageCollision->GetComponentRotation()),
            FColor::Red,
            false,
            -1.f, // Duration
            0,
            2
        );
    }
}

void UPSDamageComponent::OnDamageCollisionOverlap
(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, 
    bool bFromSweep, 
    const FHitResult& SweepResult
)
{
    AActor* OwnerActor = GetOwner();

    // Check if the OtherActor has the required tag
    if (OtherActor->ActorHasTag(TeamTag))
    {
        return;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Tags are different."))
    }
    if (OtherActor->Implements<UPSCombatInterface>())
    {
        IPSCombatInterface::Execute_TakeDamage(OtherActor, DamageAmount);
    }
}

