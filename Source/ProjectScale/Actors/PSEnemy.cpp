
#include "PSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PSPickupItem.h"
#include "ProjectScale/Utils/PSGlobals.h"

DEFINE_LOG_CATEGORY(PSEnemy);

APSEnemy::APSEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Enemy"));

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;

	const float CapsuleRadius = 35.0f; 
	const float CapsuleHalfHeight = 44.0f;
	CapsuleComp->SetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComp->SetupAttachment(RootComponent);
	FlipbookComp->bOwnerNoSee = false;
	FlipbookComp->bAffectDynamicIndirectLighting = false;
	FlipbookComp->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FlipbookComp->SetCollisionProfileName(TEXT("DefaultEnemyCollision"));
	FlipbookComp->SetGenerateOverlapEvents(true);
	FlipbookComp->SetRelativeRotation(PSGlobals::SpriteRotation);

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

	CurrentHealth = MaxHealth;

	UpdateDropChances();
}

void APSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldMove)
	{
		//FVector NewLocation = GetActorLocation() + (MovementDirection.GetSafeNormal() * MovementSpeed * DeltaTime);
		//SetActorLocation(NewLocation);

		FVector Offset = MovementDirection.GetSafeNormal() * MovementSpeed * DeltaTime;
		AddActorWorldOffset(Offset, true);
	}

	if (bIsDead)
	{
		FRotator UprightRotation = PSGlobals::SpriteRotation;
		FlipbookComp->SetWorldRotation(UprightRotation);
	}
}

void APSEnemy::TakeDamage_Implementation(const float DamageAmount, const FVector& LaunchDirection)
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
		DamagedLaunchDirection = LaunchDirection;
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

	// change the collision profile and enable physics
	CapsuleComp->SetCollisionProfileName(TEXT("RigidBody"));
	CapsuleComp->SetSimulatePhysics(true);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CapsuleComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);

	float LaunchStrength = 1000.0f; // #TODO: make var

	// Apply launch impulse
	CapsuleComp->AddImpulse(DamagedLaunchDirection * LaunchStrength, NAME_None, true);


	FTimerHandle SpawnItemTimerHandle;
	GetWorldTimerManager().SetTimer(SpawnItemTimerHandle, this, &APSEnemy::SpawnPickupItem, DelayBeforeSpawn, false);

	SetLifeSpan(DestructionDelay);
}
void APSEnemy::UpdateDropChances()
{
	CustomDropChances.Add(EPickupItemType::OrangeScale, 95.0f);
	CustomDropChances.Add(EPickupItemType::Speed, 4.0f);
	CustomDropChances.Add(EPickupItemType::Attack, 1.0f);
}
void APSEnemy::SpawnPickupItem()
{
	if (PickupItemBlueprint)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		APSPickupItem* PickupItem = GetWorld()->SpawnActor<APSPickupItem>(PickupItemBlueprint, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (PickupItem) 
		{
			PickupItem->InitializePickupItem(CustomDropChances);
			PickupItem->EnableCollision();
		}
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