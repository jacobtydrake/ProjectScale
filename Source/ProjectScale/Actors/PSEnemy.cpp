
#include "PSEnemy.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

APSEnemy::APSEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComp;

	// Initialize and set up the Flipbook Component
	FlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComp->SetupAttachment(RootComponent);
	FlipbookComp->bOwnerNoSee = false;
	FlipbookComp->bAffectDynamicIndirectLighting = false;
	FlipbookComp->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	FlipbookComp->SetCollisionProfileName(TEXT("DefaultEnemyCollision"));
	FlipbookComp->SetGenerateOverlapEvents(true);
}

void APSEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APSEnemy::TakeDamage_Implementation(const float DamageAmount)
{
	UE_LOG(LogTemp, Display, TEXT("TakeDamage_Implementation: %f"), DamageAmount);


	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APSEnemy::RevertSpriteColor, 0.1f, false);

	FlipbookComp->SetSpriteColor(FLinearColor::Red);
}

void APSEnemy::RevertSpriteColor()
{
	FlipbookComp->SetSpriteColor(FLinearColor::White);
}