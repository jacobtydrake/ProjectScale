// Written by Jacob Drake - 2023

#pragma once

#include "GameFramework/Actor.h"
#include "PSScaleWithPhysics.generated.h"

class USceneComponent;
class USphereComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;

enum class EPickupItemType : uint8;


UCLASS()
class PROJECTSCALE_API APSScaleWithPhysics : public AActor
{
	GENERATED_BODY()
	
public:	
	APSScaleWithPhysics();

	void SetSelectedFlipBook(EPickupItemType SelectedScale);

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> PhysicsSphere;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> PickupFlipbookComp;
	/* Available Flipbooks */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> BlueScaleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> OrangeScaleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> PurpleScaleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> BlackScaleFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> GoldScaleFlipbook;
};
