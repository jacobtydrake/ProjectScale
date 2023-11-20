// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSPickupItem.generated.h"

class USphereComponent;
class UPaperSpriteComponent;
class UPaperSprite;

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	Scale UMETA(DisplayName = "Scale"),
	Health UMETA(DisplayName = "Health"),
	Speed UMETA(DisplayName = "Speed"),
	ScreenWipe UMETA(DisplayName = "ScreenWipe")
};

UCLASS()
class PROJECTSCALE_API APSPickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APSPickupItem();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* PickupCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPaperSpriteComponent* PickupSprite;


public:	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickedUp();

	UFUNCTION()
	void SelectRandomItemType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupItemType ItemType;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TMap<EPickupItemType, float> ItemChances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPaperSprite* ScaleSpriteAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPaperSprite* HealthSpriteAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPaperSprite* SpeedSpriteAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	UPaperSprite* ScreenWipeSpriteAsset;


private:

};
