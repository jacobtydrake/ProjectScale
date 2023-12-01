// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "PSEnemy.h"
#include "PSChasingEnemy.generated.h"

class APSCharacter;
class UPSDamageComponent;
class TimerManager;
class USoundBase;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSChasingEnemy : public APSEnemy
{
	GENERATED_BODY()

public:
	APSChasingEnemy();
	
	UFUNCTION(BlueprintCallable)
	const bool GetIsLaunching() const { return bIsLaunching; }
protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Die() override;
	virtual void UpdateDropChances() override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPSDamageComponent> DamageComp;

	/**/
	UFUNCTION()
	void UpdateChaseBehavior();

	/**/
	UFUNCTION()
	void PerformAttack();

	UFUNCTION()
	void EndAttack();

	UFUNCTION()
	virtual void StartLaunch();

	UFUNCTION()
	void HandleLaunch();

	UFUNCTION()
	void ToggleDamageCollision();

	/* TODO: cleanup header */
	UPROPERTY(EditAnywhere, Category = "Chase Behavior")
	bool bHasInfiniteDetectionRange{ false };

	UPROPERTY(EditAnywhere, Category = "Chase Behavior")
	float ChaseRange{ 800.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackRange{ 350.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackDamage{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackThrustPower{ 1050.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float TotalAttackDuration{ 1.3f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackHurtDuration{ 0.25f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float TotalLaunchDuration{ 0.3f };

	float AttackToLaunchDelay{ .35f };

	FVector LaunchVelocity{ 0, 0, 0 };

	/**/
	float LastAttackTime{ 0.0f };
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackCooldown{ 2.0f }; 

private:

	/* cached reference to PSCharacter */
	TObjectPtr<APSCharacter> PlayerCharacter;

	FTimerManager* TimerManager;

	FTimerHandle AttackTimerHandle;
	FTimerHandle LaunchDelayTimerHandle;
	FTimerHandle LaunchTimerHandle;
	FTimerHandle DamageCollisionTimerHandle;

	bool bIsLaunching{ false };

	float CurrentLaunchDuration{ 0.0f };

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;

};
