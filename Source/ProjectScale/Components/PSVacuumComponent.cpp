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
}

void UPSVacuumComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	for (APSPickupItem* PickupItem : TrackedPickupItems)
	{
		if (PickupItem)
		{
			FVector DirectionToOwner = (GetOwner()->GetActorLocation() - PickupItem->GetActorLocation()).GetSafeNormal();
			float AttractionSpeed = 100.0f;
			FVector NewLocation = PickupItem->GetActorLocation() + DirectionToOwner * AttractionSpeed * GetWorld()->GetDeltaSeconds();
			PickupItem->SetActorLocation(NewLocation);
		}
	}
}

