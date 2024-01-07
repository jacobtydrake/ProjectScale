// Written by Jacob Drake - 2023

#pragma once

#include "PaperCharacter.h"
#include "ProjectScale/Datatypes/Charactertypes.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "PSCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PSCharacter, Log, All);

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UPSDamageComponent;
class APSHUD;
class UPSCharacterWidgetComponent;
class UPSScoreController;
class UPSVacuumComponent;
class USoundBase;

enum class EPickupItemType : uint8;

UCLASS()
class PROJECTSCALE_API APSCharacter : public APaperCharacter, public IPSCombatInterface
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
	const bool GetIsDead() const { return bIsDead; }
	UFUNCTION(BlueprintCallable)
	const bool GetPlaySecondAttack() const { return bPlaySecondAttack; }

	UFUNCTION()
	void OnItemPickup(const EPickupItemType ItemType);

	void RevertSpriteColor() const;

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerIAnputComponent) override;

	/* PSCombatInterface Implementation */
	virtual void TakeDamage_Implementation(const float DamageAmount, const FVector& LaunchDirection) override;

	/* Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	/* Input Action Bindings */
	virtual void StopMove(const FInputActionValue& Value);
	virtual void Move(const FInputActionValue& Value);
	virtual void Attack();

	/* Triggers custom event in blueprint class that triggers an animation override. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
	void OnComboAttackRequested(ELastMoveDirection Direction);

	/* Damage Component Control */
	void ToggleDamageComp(const bool bShouldActiveCollision) const;

	/* Attack Animation events */
	UFUNCTION()
	void OnFirstAttackAnimationEnd();
	UFUNCTION()
	void StopAttackAnim();

	/* Used in attack animation. */
	UFUNCTION()
	void ApplyAttackBoost(const float ThrustPower);

	/* :( */
	UFUNCTION()
	void Die();

	/* Speed Buff */
	UFUNCTION()
	void ApplySpeedBuff();
	UFUNCTION()
	void RevertSpeedBuff();

	/* Attack Buff */
	UFUNCTION()
	void ApplyAttackBuff();
	UFUNCTION()
	void RevertAttackBuff();

	UFUNCTION()
	void PlayRandomFootstepSound();

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPSDamageComponent> DamageComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPSCharacterWidgetComponent> PSWidgetComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPSVacuumComponent> PSVacuumComp;

private:

	/* Stored properties for animation logic. */
	FVector2D MovementVector{ 0.0f, 0.0f };
	bool bIsHoldingMove{ false };
	ELastHorizontalMoveDirection LastHorizontalMoveDirection{ ELastHorizontalMoveDirection::Left };
	ELastMoveDirection LastMoveDirection{ ELastMoveDirection::Left };

	UPROPERTY()
	int32 CurrentHealth{ 0 };

	UPROPERTY(EditAnywhere)
	int32 MaxHealth{ 3 };

	/****** START ATTACK PROPERTIES *****/
	/* Attack Properties */
	UPROPERTY(EditAnywhere)
	float FirstAttackAnimationLength{ 0.333f };
	UPROPERTY(EditAnywhere)
	float SecondAttackAnimationLength{ 0.333f };

	/* Amount of time used to ignore repeated inputs. */
	UPROPERTY(EditAnywhere)
	float InputBufferTime{ 0.18f };

	/* Time + FirstAttackAnim to allow for second input for second attack */
	UPROPERTY(EditAnywhere)
	float ComboWindowDurationOffset{ 0.085f };

	/* Used to track the time between inputs for the InputBufferTime */
	float LastAttackTime{ 0.0f };

	/* Time required to pass before character can attack again. */
	UPROPERTY(EditAnywhere)
	float AttackCooldown{ 0.7f };

	/* Used to track the time between inputs for the AttackCooldown. */
	float LastAttackEndTime{ 0.0f };

	/* Attack Conditions */
	bool bIsAttacking{ false };
	bool bPlaySecondAttack{ false };
	bool bIsComboAttackExecuting{ false };
	bool bIsComboAttackQueued{ false };

	/* Attack Timers */
	FTimerHandle AttackTimerHandle;
	FTimerHandle ComboWindowTimerHandle;
	FTimerHandle InputBufferTimerHandle;
	/****** END ATTACK PROPERTIES *****/

	/* Boost Modifiers */
	UPROPERTY(EditAnywhere)
	float AttackThrustPower{ 2000.0f };
	UPROPERTY(EditAnywhere)
	float ComboAttackThrustPower{ 1400.0f };

	/* Boost Modifiers */
	UPROPERTY(EditAnywhere)
	float BoostedAttackThrustPower{ 2500.0f };
	UPROPERTY(EditAnywhere)
	float BoostedComboAttackThrustPower{ 2000.0f };

	/* flag for disabling movement while attacking or dying */
	bool bIsMovementAllowed{ true };

	FVector CachedInputVector{ FVector::ZeroVector };

	/* Controls rate at which player can take damage */
	UPROPERTY(EditAnywhere)
	float DamageCooldown{ 0.80f };
	float LastDamageTime{ 0.0f };

	/* Speed buff properties */
	UPROPERTY(EditAnywhere, Category = "Speed Buff")
	float SpeedBuffMultiplier{ 1.25f };
	UPROPERTY(EditAnywhere, Category = "Speed Buff")
	float SpeedBuffDuration{ 10.0f } ;
	/* Speed Buff Timer handle */
	FTimerHandle SpeedBuffTimerHandle;
	bool bIsSpeedBuffActive = false;

	/* Attack buff properties */
	bool bIsAttackBuffActive = false;
	UPROPERTY(EditAnywhere, Category = "Attack buff Buff")
	FVector OriginalAttackBounds{ 50.0f, 50.0f, 50.0f };
	UPROPERTY(EditAnywhere, Category = "Attack buff Buff")
	FVector IncreasedAttackBounds{ 75.0f, 75.0f, 50.0f };
	UPROPERTY(EditAnywhere, Category = "Attack buff Buff")
	float AttackBuffDuration{ 10.0f };
	UPROPERTY(EditAnywhere, Category = "Attack buff Buff")
	float ReducedAttackCooldown{ 0.2f };
	FTimerHandle AttackBuffTimerHandle;
	float OriginalAttackCooldown{0.7f};

	/* Cached */
	UPROPERTY()
	TObjectPtr<APSHUD> CachedHUD;
	UPROPERTY()
	TObjectPtr<UPSScoreController> CachedScoreController;

	/* womp womp */
	bool bIsDead{ false };

	/* Sound */
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> PunchSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HurtSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<TObjectPtr<USoundBase>> FootstepSounds;

	int32 LastFootstepSoundIndex = -1;
	FTimerHandle FootstepSoundTimerHandle;

#if WITH_EDITOR
public:
	UPSDamageComponent* GetDamageComponent() { return DamageComp; }
#endif
};
