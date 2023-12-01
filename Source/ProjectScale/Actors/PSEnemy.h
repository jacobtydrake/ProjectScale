

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "PSEnemy.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PSEnemy, Log, All);

class UCapsuleComponent;
class UPaperFlipbookComponent;
class UMaterialInstanceDynamic;
class APSPickupItem;
class USoundBase;

enum class EPickupItemType : uint8;

UCLASS()
class PROJECTSCALE_API APSEnemy : public AActor, public IPSCombatInterface
{
	GENERATED_BODY()

public:
	APSEnemy();
	APSEnemy(FVector NewDirection);

	/* Getters */
	UFUNCTION(BlueprintCallable)
	const bool GetIsAttacking() const { return bIsAttacking; }
	UFUNCTION(BlueprintCallable)
	const bool GetIsDead() const { return bIsDead; }
	UFUNCTION(BlueprintCallable)
	const FVector2D Get2DMovementDirection() const { return FVector2D(MovementDirection.X, -MovementDirection.Y); }
	UFUNCTION()
	const FVector GetMovementDirection() const { return MovementDirection; }
	UFUNCTION()
	virtual void OnCharacterDeath() { bIsCharacterDead = true; }


	UFUNCTION(BlueprintCallable)
	void InitializeDirection(const FVector& NewDirection);

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* PSCombatInterface Implementation */
	virtual void TakeDamage_Implementation(const float DamageAmount, const FVector& LaunchDirection);

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperFlipbookComponent> FlipbookComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> DeathEffect;

	// Variable to hold the pickup item Blueprint class
	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<APSPickupItem> PickupItemBlueprint;


	/**/
	UFUNCTION()
	virtual void Die();

	UFUNCTION()
	virtual void UpdateDropChances();

	UFUNCTION()
	void SpawnPickupItem();

	/**/
	void RevertSpriteColor();

	/**/
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	int32 MaxHealth{ 1 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int32 CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed{ 135 };

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MovementDirection{ 1,0,0 };

	UPROPERTY()
	bool bShouldMove{ true };

	UPROPERTY()
	bool bIsAttacking{ false };

	UPROPERTY()
	bool bIsDead{ false };

	UPROPERTY()
	bool bIsCharacterDead{ false };

	UPROPERTY(EditAnywhere)
	TMap<EPickupItemType, float> CustomDropChances;

	UPROPERTY()
	FVector DamagedLaunchDirection{ FVector::ZeroVector };

	UPROPERTY()
	FRotator DeadRotation{ FRotator::ZeroRotator };

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;
};
