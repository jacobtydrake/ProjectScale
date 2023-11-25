// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PSVacuumComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSCALE_API UPSVacuumComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	UPSVacuumComponent();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	void SetVacuumRadius(const float NewRadius);

private:
	TArray<class APSPickupItem*> TrackedPickupItems;

	UFUNCTION()
	void OnVacuumOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnVacuumEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AttractPickupItems();

	UPROPERTY(EditDefaultsOnly)
	float OriginalRadius{ 150.0f };

	UPROPERTY(EditAnywhere)
	float CurrentRadius;

	UPROPERTY(EditAnywhere)
	float MinSpeed{ 1.0f };

	UPROPERTY(EditAnywhere)
	float MaxSpeed{ 30.0f };

};
