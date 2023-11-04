
#include "PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	}
	else if (MovementVector.X < 0)
	{
		LastMoveDirection = ELastMoveDirection::Left;
	}
}

void APSCharacter::Attack()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastAttackTime >= AttackCooldown)
	{
		UE_LOG(LogTemp, Display, TEXT("Attack executed"));

		LastAttackTime = CurrentTime;

		StartAttackAnim();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown"));
	}
}

void APSCharacter::StartAttackAnim()
{
	bIsAttacking = true;
	const float AttackDuration = 0.35f;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSCharacter::StopAttackAnim, AttackDuration, false);
}

void APSCharacter::StopAttackAnim()
{
	bIsAttacking = false;
}

