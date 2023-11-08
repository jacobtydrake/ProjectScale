
#include "PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h "

DEFINE_LOG_CATEGORY(PSCharacter);

APSCharacter::APSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;
	//SpringArmComp->SetRelativeRotation(FRotator(-90.f, -90.f, 0.f));
	SpringArmComp->bUsePawnControlRotation = false;

	// Create the follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// TODO: allow to be set in editor via variable reference
	ComboWindowDuration = FirstAttackAnimationLength + 0.35f;

}

void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APSCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APSCharacter::StopMove);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APSCharacter::Attack);
	}
}

void APSCharacter::StartMove(const FInputActionValue& Value)
{
	bIsHoldingMove = true;
	Move(Value);
}

void APSCharacter::StopMove(const FInputActionValue& Value)
{
	bIsHoldingMove = false;
	MovementVector = FVector2D(0.0f, 0.0f);
}

void APSCharacter::Move(const FInputActionValue& Value)
{
	if (!bIsMovementAllowed) return;


	bIsHoldingMove = true;
	MovementVector = Value.Get<FVector2D>();

	const FVector CameraForward = CameraComp->GetForwardVector();
	const FVector CameraRight = CameraComp->GetRightVector();

	// project horizontally -- maybe
	const FVector Forward = FVector(CameraForward.X, CameraForward.Y, 0.f).GetSafeNormal();
	const FVector Right = FVector(CameraRight.X, CameraRight.Y, 0.f).GetSafeNormal();

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);

	if (MovementVector.X > 0)
	{
		LastMoveDirection = ELastMoveDirection::Right;
		LastHorizontalMoveDirection = ELastHorizontalMoveDirection::Right;
	}
	else if (MovementVector.X < 0)
	{
		LastMoveDirection = ELastMoveDirection::Left;
		LastHorizontalMoveDirection = ELastHorizontalMoveDirection::Left;
	}
	else if (MovementVector.Y > 0)
	{
		LastMoveDirection = ELastMoveDirection::Up;
	}
	else if (MovementVector.Y < 0)
	{
		LastMoveDirection = ELastMoveDirection::Down;
	}
}

void APSCharacter::Attack()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastAttackTime < InputBufferTime)
	{
		UE_LOG(PSCharacter, Display, TEXT("Attack within input buffer time"));
		return;
	}
	else if (CurrentTime - LastAttackEndTime < AttackCooldown)
	{
		UE_LOG(PSCharacter, Display, TEXT("Attack is on cooldown"));
		return;
	}
	else if (bIsComboAttackQueued || bIsComboAttackExecuting)
	{
		UE_LOG(PSCharacter, Display, TEXT("Early return"));
		return;
	}

	if (!bIsAttacking)
	{
		UE_LOG(PSCharacter, Display, TEXT("Attack executed"));

		bIsAttacking = true;
		LastAttackTime = CurrentTime;

		bIsMovementAllowed = false;

		ApplyAttackBoost(AttackThrustPower);

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSCharacter::OnFirstAttackAnimationEnd, FirstAttackAnimationLength, false);
		GetWorldTimerManager().SetTimer(ComboWindowTimerHandle, this, &APSCharacter::StopAttackAnim, ComboWindowDuration, false);
	}
	else if (CurrentTime - LastAttackTime < ComboWindowDuration)
	{
		if (!bIsComboAttackQueued && !bIsComboAttackExecuting && CurrentTime - LastAttackTime > FirstAttackAnimationLength)
		{
			UE_LOG(PSCharacter, Display, TEXT("Combo Attack executed immediately"));

			bIsComboAttackExecuting = true;

			OnComboAttackRequested(LastMoveDirection);

			GetWorldTimerManager().ClearTimer(AttackTimerHandle);
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSCharacter::StopAttackAnim, SecondAttackAnimationLength, false);

			GetWorldTimerManager().ClearTimer(ComboWindowTimerHandle);
		}
		else if (!bIsComboAttackQueued)
		{
			UE_LOG(PSCharacter, Display, TEXT("Combo Attack queued"));
			bIsComboAttackQueued = true;
		}
	}
	else
	{
		UE_LOG(PSCharacter, Display, TEXT("Attack is on cooldown"));
	}
}

void APSCharacter::OnFirstAttackAnimationEnd()
{
	if (bIsComboAttackQueued)
	{
		UE_LOG(PSCharacter, Display, TEXT("Combo Attack executed"));

		OnComboAttackRequested(LastMoveDirection);
		bIsComboAttackQueued = false;
		bIsComboAttackExecuting = true;

		ApplyAttackBoost(ComboAttackThrustPower);

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSCharacter::StopAttackAnim, SecondAttackAnimationLength, false);
		GetWorldTimerManager().ClearTimer(ComboWindowTimerHandle);
	}
}

void APSCharacter::StopAttackAnim()
{
	bIsComboAttackExecuting = false;
	bIsAttacking = false;
	bIsComboAttackQueued = false;
	LastAttackEndTime = GetWorld()->GetTimeSeconds();

	// Re-enable movement
	bIsMovementAllowed = true;
}

void APSCharacter::ApplyAttackBoost(const float ThrustPower)
{
	FVector BoostDirection;

	Jump();

	switch (LastMoveDirection)
	{
	case ELastMoveDirection::Up:
		BoostDirection = FVector(0, -1, 0);
		break;
	case ELastMoveDirection::Down:
		BoostDirection = FVector(0, 1, 0);
		break;
	case ELastMoveDirection::Left:
		BoostDirection = FVector(-1, 0, 0);
		break;
	case ELastMoveDirection::Right:
		BoostDirection = FVector(1, 0, 0);
		break;
	default:
		return;
	}

	// apply thrust in direction of last movement
	LaunchCharacter(BoostDirection * ThrustPower, true, true);
}
