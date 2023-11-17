// Written by Jacob Drake - 2023

#pragma once

#include "CoreMinimal.h"
#include "PSEnemy.h"
#include "PSChasingEnemy.generated.h"

class APSCharacter;
class UPSDamageComponent;

/**
 * 
 */
UCLASS()
class PROJECTSCALE_API APSChasingEnemy : public APSEnemy
{
	GENERATED_BODY()

public:
	APSChasingEnemy();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPSDamageComponent> DamageComp;

	/**/
	void UpdateChaseBehavior();

	/**/
	void PerformAttack();

	void EndAttack();

	UFUNCTION()
	void HandleLaunch();

	/* TODO: cleanup header */
	UPROPERTY(EditAnywhere, Category = "Chase Behavior")
	bool bHasInfiniteDetectionRange{ false };

	UPROPERTY(EditAnywhere, Category = "Chase Behavior")
	float ChaseRange{ 800.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackRange{ 250.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackDamage{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackThrustPower{ 1.0f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float TotalAttackDuration{ 0.5f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackHurtDuration{ 0.2f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float AttackAnimationLength{ 0.3f };

	UPROPERTY(EditAnywhere, Category = "Attack Behavior")
	float LaunchDuration{ 0.3f };

	FVector LaunchVelocity{ 0, 0, 0 };

	/**/
	float LastAttackTime{ 0.0f };
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackCooldown{ 2.0f }; 

	/* cached reference to PSCharacter */
	TObjectPtr<APSCharacter> PlayerCharacter;

	FTimerHandle AttackTimerHandle;
	FTimerHandle LaunchTimerHandle;

	bool bIsAttacking{ false };

	bool bIsLaunching{ false };
};
