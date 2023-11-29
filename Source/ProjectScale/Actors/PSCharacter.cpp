
#include "PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h "
#include "ProjectScale/Components/PSDamageComponent.h"
#include "Components/BoxComponent.h"
#include "ProjectScale/Controllers/PSPlayerController.h"
#include "PSPickupItem.h"
#include "ProjectScale/PSHUD.h"
#include "ProjectScale/Controllers/PSPlayerController.h"
#include "ProjectScale/Components/PSCharacterWidgetComponent.h"
#include "ProjectScale/Controllers/PSScoreController.h"
#include "ProjectScale/Components/PSVacuumComponent.h"
#include "ProjectScale/Utils/PSGlobals.h"
#include "ProjectScale/Actors/PSWooshEffect.h"

DEFINE_LOG_CATEGORY(PSCharacter);

APSCharacter::APSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Player"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 200.0f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bDoCollisionTest = false;

	// Create the follow camera - make static? maybe?
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create and attach the damage component
	DamageComp = CreateDefaultSubobject<UPSDamageComponent>(TEXT("DamageComponent"));
	DamageComp->SetTeamTag("Player");
	DamageComp->ToggleRelativePositioning(true);

	PSWidgetComp = CreateDefaultSubobject<UPSCharacterWidgetComponent>(TEXT("PSWidgetComponent"));
	PSWidgetComp->SetupAttachment(RootComponent);
	PSWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PSWidgetComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	const FRotator NewRotation = FRotator(30.0f, 90.0f, 0.0f); //TODO: make var
	PSWidgetComp->SetRelativeRotation(NewRotation);

	const FVector WidgetOffset = FVector(0.0f, 0.0f, 120.0);  //TODO: make var
	PSWidgetComp->SetRelativeLocation(WidgetOffset);

	PSWidgetComp->SetCastShadow(false);
	PSWidgetComp->SetVisibility(false);

	PSVacuumComp = CreateDefaultSubobject<UPSVacuumComponent>(TEXT("PSVacuumComponent"));
	PSVacuumComp->SetupAttachment(RootComponent);

	GetSprite()->SetRelativeRotation(PSGlobals::SpriteRotation);

	// ComboWindow must at least be the length of the first attack
	ComboWindowDurationOffset += FirstAttackAnimationLength;

	// set starting health to max health
	CurrentHealth = MaxHealth;
}

void APSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<APSPlayerController> PSPlayerController = Cast<APSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		CachedHUD = PSPlayerController->GetPSHUD();
		CachedScoreController = PSPlayerController->GetPSScoreController();
	}
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

void APSCharacter::TakeDamage_Implementation(const float DamageAmount, const FVector& LaunchDirection)
{
	if (bIsDead) return;

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastDamageTime >= DamageCooldown)
	{
		LastDamageTime = CurrentTime;

		CurrentHealth -= DamageAmount;

		FVector DirectionToLaunch = LaunchDirection;
		DirectionToLaunch.Z = 0.0f;

		LaunchCharacter(DirectionToLaunch * 500, true, true);

		GetSprite()->SetSpriteColor(FLinearColor::Red);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &APSCharacter::RevertSpriteColor, 0.1f, false);

		if (CachedHUD)
		{
			CachedHUD->UpdateHealthWidget(CurrentHealth);
		}

		if (CurrentHealth <= 0.0f)
		{
			Die();
		}
	}
	else
	{
		UE_LOG(PSCharacter, Display, TEXT("Damage on cooldown, no damage taken"));
	}
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

	const FVector CameraForward = CameraComp->GetForwardVector();
	const FVector CameraRight = CameraComp->GetRightVector();

	// project horizontally
	const FVector Forward = FVector(CameraForward.X, CameraForward.Y, 0.f).GetSafeNormal();
	const FVector Right = FVector(CameraRight.X, CameraRight.Y, 0.f).GetSafeNormal();

	AddMovementInput(Forward, MovementVector.Y);
	AddMovementInput(Right, MovementVector.X);
}

// flags are set to control animation states in ABP_BuffBaby state machine
// OnComboAttackRequested() triggers custom event in BP_PSCharacter with Animation Overrides
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
		UE_LOG(PSCharacter, Display, TEXT("Combo attack is already in progress"));
		return;
	}
	else if (bIsDead)
	{
		return;
	}

	if (!bIsAttacking)
	{
		UE_LOG(PSCharacter, Display, TEXT("First Attack executed"));
		ToggleDamageComp(false);
		bIsAttacking = true;
		LastAttackTime = CurrentTime;

		bIsMovementAllowed = false;

		ToggleDamageComp(true);

		const float BoostPower = bIsAttackBuffActive ? BoostedAttackThrustPower : AttackThrustPower;
		ApplyAttackBoost(AttackThrustPower);

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSCharacter::OnFirstAttackAnimationEnd, FirstAttackAnimationLength, false);
		GetWorldTimerManager().SetTimer(ComboWindowTimerHandle, this, &APSCharacter::StopAttackAnim, ComboWindowDurationOffset, false);
	}
	else if (CurrentTime - LastAttackTime < ComboWindowDurationOffset)
	{
		if (!bIsComboAttackQueued && !bIsComboAttackExecuting && CurrentTime - LastAttackTime > FirstAttackAnimationLength)
		{
			UE_LOG(PSCharacter, Display, TEXT("Combo Attack executed immediately"));

			bIsComboAttackExecuting = true;

			OnComboAttackRequested(LastMoveDirection);
			ToggleDamageComp(true);

			const float BoostPower = bIsAttackBuffActive ? BoostedComboAttackThrustPower : ComboAttackThrustPower;
			ApplyAttackBoost(BoostPower);

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

void APSCharacter::ToggleDamageComp(const bool bShouldActiveCollision)
{
	if (DamageComp)
	{
		if (bShouldActiveCollision)
		{
			DamageComp->ActivateDamageCollision();
		}
		else
		{
			DamageComp->DeactivateDamageCollision();
		}
	}
}

void APSCharacter::OnFirstAttackAnimationEnd()
{
	ToggleDamageComp(false);
	//WooshEffectComp->SetVisibility(false);

	if (bIsComboAttackQueued)
	{
		UE_LOG(PSCharacter, Display, TEXT("Combo Attack executed"));

		OnComboAttackRequested(LastMoveDirection);
		bIsComboAttackQueued = false;
		bIsComboAttackExecuting = true;

		ToggleDamageComp(true);

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

	ToggleDamageComp(false);
	//WooshEffectComp->SetVisibility(false);
	bIsMovementAllowed = true;
}

void APSCharacter::ApplyAttackBoost(const float ThrustPower)
{
	const FVector LastInputVector = GetMovementComponent()->GetLastInputVector();

	// if character was not moving, apply thrust where currently facing.
	if (!bIsHoldingMove && !bIsComboAttackExecuting) //TODO: move second condition to func param and clean up logic.
	{
		FVector BoostDirection;
		switch (LastHorizontalMoveDirection)
		{
		case ELastHorizontalMoveDirection::Left:
			BoostDirection = FVector(-1, 0, 0);
			LastMoveDirection = ELastMoveDirection::Left; // must update last move direction for correct animation selection
			break;
		case ELastHorizontalMoveDirection::Right:
			BoostDirection = FVector(1, 0, 0);
			LastMoveDirection = ELastMoveDirection::Right;
			break;
		default:
			return;
		}
		CachedInputVector = BoostDirection;
		LaunchCharacter(BoostDirection * ThrustPower, true, true);
	}
	else if (LastInputVector == FVector::ZeroVector)
	{
		LaunchCharacter(CachedInputVector * ThrustPower, true, true);
	}
	else
	{
		CachedInputVector = LastInputVector;
		LaunchCharacter(GetMovementComponent()->GetLastInputVector() * ThrustPower, true, true);
	}
}

void APSCharacter::Die()
{
	bIsDead = true;
	if (APSPlayerController* PC = Cast<APSPlayerController>(GetController()))
	{
		PC->OnCharacterDeath();
	}
	bIsMovementAllowed = false;

	// Flip the sprite so character is facing score screen :)
	FVector FlippedSpriteScale = GetSprite()->GetComponentScale();
	GetSprite()->SetWorldScale3D(FVector(-FlippedSpriteScale.X, FlippedSpriteScale.Y, FlippedSpriteScale.Z));

	GetWorldTimerManager().ClearTimer(SpeedBuffTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	GetWorldTimerManager().ClearTimer(ComboWindowTimerHandle);
	GetWorldTimerManager().ClearTimer(InputBufferTimerHandle);
	bIsComboAttackQueued = false;
	bIsAttacking = false;
}

void APSCharacter::ApplySpeedBuff()
{
	if (!bIsSpeedBuffActive)
	{
		OriginalAttackCooldown = AttackCooldown;

		GetCharacterMovement()->MaxWalkSpeed *= SpeedBuffMultiplier;

		bIsSpeedBuffActive = true;
	}
	GetWorldTimerManager().ClearTimer(SpeedBuffTimerHandle);
	GetWorldTimerManager().SetTimer(SpeedBuffTimerHandle, this, &APSCharacter::RevertSpeedBuff, SpeedBuffDuration);

	if (CachedHUD)
	{
		CachedHUD->UpdateSpeedPowerupWidget(SpeedBuffDuration);
	}
}

void APSCharacter::RevertSpeedBuff()
{
	GetCharacterMovement()->MaxWalkSpeed /= SpeedBuffMultiplier;

	bIsSpeedBuffActive = false;
}

void APSCharacter::ApplyAttackBuff()
{
	if (!bIsAttackBuffActive && DamageComp)
	{
		DamageComp->SetDamageCollisionSize(IncreasedAttackBounds);
		bIsAttackBuffActive = true;
		AttackCooldown = ReducedAttackCooldown;
	}
	GetWorldTimerManager().SetTimer(AttackBuffTimerHandle, this, &APSCharacter::RevertAttackBuff, AttackBuffDuration);

	if (CachedHUD)
	{
		CachedHUD->UpdateAttackPowerupWidget(AttackBuffDuration);
	}
}

void APSCharacter::RevertAttackBuff()
{
	if (DamageComp)
	{
		DamageComp->SetDamageCollisionSize(OriginalAttackBounds);
	}
	bIsAttackBuffActive = false;
	AttackCooldown = OriginalAttackCooldown;
}

void APSCharacter::OnItemPickup(EPickupItemType ItemType)
{
	switch (ItemType)
	{
	case EPickupItemType::OrangeScale:
		CachedScoreController->AddItemToScore(ItemType);
		break;

	case EPickupItemType::BlueScale:
		CachedScoreController->AddItemToScore(ItemType);
		break;

	case EPickupItemType::PurpleScale:
		CachedScoreController->AddItemToScore(ItemType);
		break;

	case EPickupItemType::BlackScale:
		CachedScoreController->AddItemToScore(ItemType);
		break;

	case EPickupItemType::GoldScale:
		CachedScoreController->AddItemToScore(ItemType);
		break;

	case EPickupItemType::Health:
		CurrentHealth += 1;

		if (CurrentHealth > MaxHealth)
		{
			CachedScoreController->AddItemToScore(EPickupItemType::OrangeScale); // pity point for wasted health item
			CurrentHealth = MaxHealth;
		}
		if (CachedHUD)
		{
			CachedHUD->UpdateHealthWidget(CurrentHealth);
		}
		break;

	case EPickupItemType::Speed:
		ApplySpeedBuff();
		break;

	case EPickupItemType::Attack:
		ApplyAttackBuff();
		break;

	default:
		CachedScoreController->AddItemToScore(ItemType);
		break;
	}
}

void APSCharacter::RevertSpriteColor()
{
	GetSprite()->SetSpriteColor(FLinearColor::White);
}
