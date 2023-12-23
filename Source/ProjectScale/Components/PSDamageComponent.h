// Written by Jacob Drake - 2023

#pragma once

#include "Components/ActorComponent.h"
#include "PSDamageComponent.generated.h"

class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSCALE_API UPSDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPSDamageComponent();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION()
	void SetTeamTag(const FName TeamTagName) { TeamTag = TeamTagName; }
	UFUNCTION()
	const bool GetIsDamageCollisionActive() const { return bIsDamageCollisionActive;  }
	UFUNCTION()
	void SetDamageCollisionSize(const FVector NewSize) const;

	TObjectPtr<UBoxComponent> GetDamageCollisionBox();
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ActivateDamageCollision();
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DeactivateDamageCollision();

	void ToggleRelativePositioning(const bool bShouldActivate);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DamageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	FName TeamTag;

	UFUNCTION()
	void OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Damage value
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Collision", meta = (AllowPrivateAccess = "true"))
	FVector DamageCollisionSize = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage Collision", meta = (AllowPrivateAccess = "true"))
	FVector DamageCollisionRelativeLocation = FVector::ZeroVector;

	bool bIsDamageCollisionActive{ false };

	bool bIsRelativePositioningActive{ false };
};
