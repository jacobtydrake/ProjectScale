
#include "PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APSCharacter::APSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APSCharacter::Move);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APSCharacter::Attack);
	}
}

void APSCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FVector CameraForward = CameraComp->GetForwardVector();
	const FVector CameraRight = CameraComp->GetRightVector();

	// project horizontally -- maybe
	const FVector Forward = FVector(CameraForward.X, CameraForward.Y, 0.f).GetSafeNormal();
	const FVector Right = FVector(CameraRight.X, CameraRight.Y, 0.f).GetSafeNormal();

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);

	// update sprite direction
	if (MovementVector.X > 0)
	{
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else if (MovementVector.X < 0)
	{
		// Face Left
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
}

void APSCharacter::Attack()
{
}

