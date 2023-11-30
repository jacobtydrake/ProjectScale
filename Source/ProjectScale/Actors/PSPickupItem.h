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
class USoundBase;

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	BlueScale UMETA(DisplayName = "Blue Scale"),
	OrangeScale UMETA(DisplayName = "Orange Scale"),
	PurpleScale UMETA(DisplayName = "Purple Scale"),
	BlackScale UMETA(DisplayName = "Black Scale"),
	GoldScale UMETA(DisplayName = "Gold Scale"),
	Health UMETA(DisplayName = "Health"),
	Speed UMETA(DisplayName = "Speed"),
	Attack UMETA(DisplayName = "Attack")
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

	UFUNCTION()
	void EnableCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	EPickupItemType ItemType;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TMap<EPickupItemType, float> ItemChances;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> HealthFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> SpeedFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	TObjectPtr<UPaperFlipbook> AttackFlipbook;

private:
	UPROPERTY(EditAnywhere)
	float Lifespan{ 25.0f };
	UPROPERTY(EditAnywhere)
	float FlashTime{ 8.0f };

	FTimerHandle FlashTimerHandle;

	void StartFlashing();
	void FlashEffect();

	/* item bob properties */
	float RunningTime{ 0.0f };
	float BobSpeed{ 2.5f };
	float BobAmplitude{ 12.0f };
	float InitialZ{ 105.f };

	/* move to platform if off, so player can pick up item */
	void MoveTowardsCircle(float DeltaTime);
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector CircleCenter;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CircleRadius = 1000.0f;  
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeedTowardsCircle = 100.0f;
	bool bIsInCircle{ false };

	TObjectPtr<USoundBase> SelectedSound;

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> OrangeScaleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> BlueScaleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> PurpleScaleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> BlackScaleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> GoldScaleSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HealthSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> SpeedSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;
};
