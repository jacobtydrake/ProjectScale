
#include "PSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"

DEFINE_LOG_CATEGORY(PSEnemy);

APSEnemy::APSEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComp->SetupAttachment(RootComponent);
	FlipbookComp->bOwnerNoSee = false;
	FlipbookComp->bAffectDynamicIndirectLighting = false;
	FlipbookComp->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FlipbookComp->SetCollisionProfileName(TEXT("DefaultEnemyCollision"));
	FlipbookComp->SetGenerateOverlapEvents(true);

	DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
	DirectionArrow->SetupAttachment(RootComponent);
	DirectionArrow->ArrowSize = 2.0f;

	DeathEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DeathEffect"));
	DeathEffect->SetupAttachment(RootComponent);
	DeathEffect->bAutoActivate = false;
}

APSEnemy::APSEnemy(FVector NewDirection)
	: MovementDirection(NewDirection)
{
	//do other stuff ig
}

void APSEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldMove)
	{
		FVector Direction = DirectionArrow->GetForwardVector();
		FVector NewLocation = GetActorLocation() + (Direction * MovementSpeed * DeltaTime);
		SetActorLocation(NewLocation);
	}
}

void APSEnemy::TakeDamage_Implementation(const float DamageAmount)
{
	UE_LOG(LogTemp, Display, TEXT("TakeDamage_Implementation: %f"), DamageAmount);

	FlipbookComp->SetSpriteColor(FLinearColor::Red);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APSEnemy::RevertSpriteColor, 0.1f, false);

	CurrentHealth -= 1.0f;

	if (CurrentHealth <= 0.f)
	{
		Die();
	}
}

void APSEnemy::InitializeDirection(const FVector& NewDirection)
{
	MovementDirection = NewDirection.GetSafeNormal();
	DirectionArrow->SetWorldRotation(MovementDirection.Rotation());
}

void APSEnemy::PerformAttack()
{
	UE_LOG(PSEnemy, Display, TEXT("APSEnemy::PerformAttack()"))
}

void APSEnemy::Die()
{
	UE_LOG(PSEnemy, Display, TEXT("APSEnemy::Die()"));

	bShouldMove = false;

	if (DeathEffect)
	{
		DeathEffect->Activate(true);
	}

	float DestructionDelay = 2.0f; 
	SetLifeSpan(DestructionDelay);
}

void APSEnemy::RevertSpriteColor()
{
	FlipbookComp->SetSpriteColor(FLinearColor::White);
}