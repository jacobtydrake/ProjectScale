// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "ProjectScale/Actors/PSEnemy.h"
#include "PSAttackingEnemy.generated.h"

class UPSDamageComponent;
class TimerManager;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSAttackingEnemy : public APSEnemy
{
	GENERATED_BODY()

	APSAttackingEnemy();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Die() override;
	virtual void UpdateDropChances() override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPSDamageComponent> DamageComp;

	UFUNCTION()
	void PerformAttack();

	UFUNCTION()
	void ActivateDamageCollision();

	UFUNCTION()
	void DeactivateDamageCollision();

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackAnimationLength{ 0.35f };

	float LastAttackTime{ 0.0f };

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackCooldown{ .80f };

	FTimerManager* TimerManager;

	FTimerHandle AttackLoopTimerHandle;
	FTimerHandle AttackLengthTimerHandle;
};
