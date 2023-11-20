
#include "PSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PSPickupItem.h"
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

	static ConstructorHelpers::FClassFinder<APSPickupItem> PickupItemClass(TEXT("/Game/ProjectScale/Blueprints/Actors/BP_PSPickupItem.BP_PSPickupItem_C"));
	if (PickupItemClass.Succeeded())
	{
		PickupItemBlueprint = PickupItemClass.Class;
	}
	else
	{
		UE_LOG(PSEnemy, Warning, TEXT("Could not find PickupItem Blueprint class!"));
	}
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

	float DelayBeforeSpawn = 0.45f;  // slightly less than the destruction delay
	float DestructionDelay = 0.5f;

	FTimerHandle SpawnItemTimerHandle;
	GetWorldTimerManager().SetTimer(SpawnItemTimerHandle, this, &APSEnemy::SpawnPickupItem, DelayBeforeSpawn, false);

	SetLifeSpan(DestructionDelay);
}
void APSEnemy::SpawnPickupItem()
{
	if (PickupItemBlueprint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<APSPickupItem>(PickupItemBlueprint, GetActorLocation(), GetActorRotation(), SpawnParams);

		UE_LOG(PSEnemy, Warning, TEXT("ItemSpawned"));
	}
	else
	{
		UE_LOG(PSEnemy, Display, TEXT("PickupItemBlueprint not valid!"));
	}
}


void APSEnemy::RevertSpriteColor()
{
	FlipbookComp->SetSpriteColor(FLinearColor::White);
}