// Written by Jacob Drake - 2023

#pragma once

#include "GameFramework/Actor.h"
#include "PSWooshEffect.generated.h"

class USceneComponent;
class UPaperFlipbookComponent;

UCLASS()
class PROJECTSCALE_API APSWooshEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APSWooshEffect();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RootSceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;
};
