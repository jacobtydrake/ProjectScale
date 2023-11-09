
#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "ProjectScale/Datatypes/Charactertypes.h"
#include "PSCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PSCharacter, Log, All);

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UPSDamageComponent;

UCLASS()
class PROJECTSCALE_API APSCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	APSCharacter();

	/* Getters */
	UFUNCTION(BlueprintCallable)
	const FVector2D GetMovementVector() const { return MovementVector; }
	UFUNCTION(BlueprintCallable)
	const bool GetIsHoldingMove() const { return bIsHoldingMove; }
	UFUNCTION(BlueprintCallable)
	const ELastHorizontalMoveDirection GetLastHorizontalMoveDirection() const { return LastHorizontalMoveDirection; }
	UFUNCTION(BlueprintCallable)
	const ELastMoveDirection GetLastMoveDirection() const {return LastMoveDirection;}
	UFUNCTION(BlueprintCallable)
	const bool GetIsAttacking() const { return bIsAttacking; }
	UFUNCTION(BlueprintCallable)
	const bool GetPlaySecondAttack() const { return bPlaySecondAttack; }

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerIAnputComponent) override;
	virtual void PostInitializeComponents() override;

	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	/* Input Action Bindings */
	virtual void StartMove(const FInputActionValue& Value);
	virtual void StopMove(const FInputActionValue& Value);
	virtual void Move(const FInputActionValue& Value);
	virtual void Attack();

	/* Triggers custom event in blueprint class that triggers an animation override. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnComboAttackRequested(ELastMoveDirection Direction);

	/* Damage Component Control */
	void ToggleDamageComp(const bool bShouldActiveCollision);

	/* Attack Animation events */
	UFUNCTION()
	void OnFirstAttackAnimationEnd();
	UFUNCTION()
	void StopAttackAnim();

	/* Used in attack animation. */
	UFUNCTION()
	void ApplyAttackBoost(const float ThrustPower);

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPSDamageComponent> DamageComp;

private:

	/* Stored properties for animation logic. */
	FVector2D MovementVector{ 0.0f, 0.0f };
	bool bIsHoldingMove{ false };
	ELastHorizontalMoveDirection LastHorizontalMoveDirection{ ELastHorizontalMoveDirection::Left };
	ELastMoveDirection LastMoveDirection{ ELastMoveDirection::Left };

	/****** START ATTACK PROPERTIES *****/
	/* Attack Properties */
	UPROPERTY(EditAnywhere)
	float FirstAttackAnimationLength{ 0.34f };
	UPROPERTY(EditAnywhere)
	float SecondAttackAnimationLength{ 0.34f };

	/* Amount of time used to ignore repeated inputs. */
	UPROPERTY(EditAnywhere)
	float InputBufferTime{ 0.2f };

	/* Used to track the time between inputs for the InputBufferTime */
	float LastAttackTime{ 0.0f };

	/* Time required to pass before character can attack again. */
	UPROPERTY(EditAnywhere)
	float AttackCooldown{ 0.33f };

	/* Used to tarck the time between inputs for the AttackCooldown. */
	float LastAttackEndTime{ 0.0f };

	/* Attack Conditions */
	bool bIsAttacking{ false };
	bool bPlaySecondAttack{ false };
	bool bIsComboAttackExecuting{ false };
	bool bIsComboAttackQueued{ false };

	/* Used to track */
	float ComboWindowDuration{ 0.0f };

	/* Timers */
	FTimerHandle AttackTimerHandle;
	FTimerHandle ComboWindowTimerHandle;
	FTimerHandle InputBufferTimerHandle;
	/****** END ATTACK PROPERTIES *****/

	/* Boost Modifiers */
	UPROPERTY(EditAnywhere)
	float AttackThrustPower = 2000.0f;
	float ComboAttackThrustPower = 1000.0f;

	/* flag for disabling movement while attacking */
	bool bIsMovementAllowed{ true };
};
