
#include "PSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Particles/ParticleSystemComponent.h"

DEFINE_LOG_CATEGORY(PSEnemy);

APSEnemy::APSEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Enemy"));

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComp->SetupAttachment(RootComponent);
	FlipbookComp->bOwnerNoSee = false;
	FlipbookComp->bAffectDynamicIndirectLighting = false;
	FlipbookComp->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FlipbookComp->SetCollisionProfileName(TEXT("DefaultEnemyCollision"));
	FlipbookComp->SetGenerateOverlapEvents(true);

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
		FVector NewLocation = GetActorLocation() + (MovementDirection.GetSafeNormal() * MovementSpeed * DeltaTime);
		SetActorLocation(NewLocation);
	}

	if (FlipbookComp)
	{
		FVector Scale = FlipbookComp->GetComponentScale();
		Scale.X = -FMath::Sign(MovementDirection.X) * FMath::Abs(Scale.X);
		FlipbookComp->SetWorldScale3D(Scale);
	}
}

void APSEnemy::TakeDamage_Implementation(const float DamageAmount)
{
	if (bIsDead)
		return;

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
}

void APSEnemy::Die()
{
	UE_LOG(PSEnemy, Display, TEXT("APSEnemy::Die()"));

	bIsDead = true;
	bShouldMove = false;

	if (DeathEffect)
	{
		DeathEffect->Activate(true);
	}

	float DestructionDelay = 0.5f; 
	SetLifeSpan(DestructionDelay);
}

void APSEnemy::RevertSpriteColor()
{
	FlipbookComp->SetSpriteColor(FLinearColor::White);
}