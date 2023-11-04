
#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PSCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class ELastMoveDirection : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};


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
	const ELastMoveDirection GetLastMoveDirection() const {return LastMoveDirection;}

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComp;

private:
	FVector2D MovementVector{ 0.0f, 0.0f };

	bool bIsHoldingMove{ false };

	ELastMoveDirection LastMoveDirection { ELastMoveDirection::Left };

};
