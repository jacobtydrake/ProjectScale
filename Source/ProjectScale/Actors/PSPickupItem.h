// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PSPickupItem.generated.h"

class USphereComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class UPSPickupItemWidgetComponent;
class USceneComponent;

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	BlueScale UMETA(DisplayName = "Blue Scale"),
	OrangeScale UMETA(DisplayName = "Orange Scale"),
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
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USphereComponent> PickupCollisionComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPaperFlipbookComponent> PickupFlipbookComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPSPickupItemWidgetComponent> PSPickupItemWidgetComp;


public:	
	UFUNCTION()
	void InitializePickupItem(const TMap<EPickupItemType, float>& NewItemChances);

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
	TObjectPtr<UPaperFlipbook> BlueScaleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> OrangeScaleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> HealthFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> SpeedFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> ScreenWipeFlipbook;


private:

	UPROPERTY(EditAnywhere)
	float Lifespan = 25.0f; 
	UPROPERTY(EditAnywhere)
	float FlashTime = 8.0f;

	FTimerHandle FlashTimerHandle;
	FTimerHandle LifespanTimerHandle;

	void StartFlashing();
	void FlashEffect();

};