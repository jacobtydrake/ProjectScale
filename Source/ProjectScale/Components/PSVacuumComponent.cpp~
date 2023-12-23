// Written by Jacob Drake - 2023


#include "PSVacuumComponent.h"
#include "ProjectScale/Actors/PSPickupItem.h"

UPSVacuumComponent::UPSVacuumComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPSVacuumComponent::BeginPlay()
{
	Super::BeginPlay();

	SetSphereRadius(OriginalRadius);
	OnComponentBeginOverlap.AddDynamic(this, &UPSVacuumComponent::OnVacuumOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UPSVacuumComponent::OnVacuumEndOverlap);

    CurrentRadius = OriginalRadius;
}

void UPSVacuumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TrackedPickupItems.Num() > 0)
	{
		AttractPickupItems();
	}
}

void UPSVacuumComponent::SetVacuumRadius(const float NewRadius)
{
	SetSphereRadius(NewRadius);
}

void UPSVacuumComponent::OnVacuumOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APSPickupItem* PickupItem = Cast<APSPickupItem>(OtherActor))
	{
		TrackedPickupItems.Add(PickupItem);
	}
}

void UPSVacuumComponent::OnVacuumEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APSPickupItem* PickupItem = Cast<APSPickupItem>(OtherActor))
	{
		TrackedPickupItems.Remove(PickupItem);
	}
}

void UPSVacuumComponent::AttractPickupItems()
{
    for (int32 i = 0; i < TrackedPickupItems.Num(); ++i)
    {
        APSPickupItem* PickupItem = TrackedPickupItems[i];
        if (PickupItem)
        {
            FVector PlayerLocation = GetOwner()->GetActorLocation();
            FVector PickupLocation = PickupItem->GetActorLocation();
            float DistanceSquared = FVector::DistSquared(PlayerLocation, PickupLocation);

            if (DistanceSquared <= FMath::Square(CurrentRadius))
            {
                FVector DirectionToOwner = (PlayerLocation - PickupLocation).GetSafeNormal();
                TRange<float> InputRange(0.f, FMath::Square(CurrentRadius));
                TRange<float> OutputRange(MinSpeed, MaxSpeed);
                float AttractionSpeed = FMath::GetMappedRangeValueClamped(InputRange, OutputRange, DistanceSquared);

                FVector NewLocation = FMath::VInterpTo(PickupLocation, PlayerLocation, GetWorld()->GetDeltaSeconds(), AttractionSpeed);
                PickupItem->SetActorLocation(NewLocation);

                // need to check if iten was picked up during loop
                if (i < TrackedPickupItems.Num() && TrackedPickupItems[i] != PickupItem)
                {
                    --i;
                }
            }
        }
    }
}

