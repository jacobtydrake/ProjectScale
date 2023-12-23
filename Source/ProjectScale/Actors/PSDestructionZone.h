// Written by Jacob Drake - 2023

#pragma once

#include "GameFramework/Actor.h"
#include "PSDestructionZone.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTSCALE_API APSDestructionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	APSDestructionZone();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, Category = "Zone")
	TObjectPtr<UBoxComponent> TriggerVolume;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
