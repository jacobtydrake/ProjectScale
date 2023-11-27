// Written by Jacob Drake - 2023


#include "PSAttackingEnemy.h"
#include "ProjectScale/Components/PSDamageComponent.h"
#include "TimerManager.h"
#include "ProjectScale/Actors/PSPickupItem.h"

APSAttackingEnemy::APSAttackingEnemy()
{
	DamageComp = CreateDefaultSubobject<UPSDamageComponent>(TEXT("DamageComponent"));
	DamageComp->SetTeamTag("Enemy");
	MovementSpeed = 250;
}

void APSAttackingEnemy::BeginPlay()
{
	Super::BeginPlay();

	TimerManager = &GetWorld()->GetTimerManager();

	TimerManager->SetTimer(AttackLoopTimerHandle, this, &APSAttackingEnemy::PerformAttack, AttackCooldown, true);
}

void APSAttackingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APSAttackingEnemy::Die()
{
	Super::Die();

	TimerManager->ClearTimer(AttackLoopTimerHandle);
	TimerManager->ClearTimer(AttackLengthTimerHandle);

	DamageComp->DeactivateDamageCollision();
}

void APSAttackingEnemy::UpdateDropChances()
{
	CustomDropChances.Add(EPickupItemType::PurpleScale, 90.0f);
	CustomDropChances.Add(EPickupItemType::Speed, 5.0f);
	CustomDropChances.Add(EPickupItemType::Health, 1.0f);
	CustomDropChances.Add(EPickupItemType::Attack, 4.0f);
}

void APSAttackingEnemy::PerformAttack()
{
	if (bIsDead) return;

	bIsAttacking = true;

	ActivateDamageCollision();

	TimerManager->SetTimer(AttackLengthTimerHandle, this, &APSAttackingEnemy::DeactivateDamageCollision, AttackAnimationLength, false);
}

void APSAttackingEnemy::ActivateDamageCollision()
{
	DamageComp->ActivateDamageCollision();
}

void APSAttackingEnemy::DeactivateDamageCollision()
{
	bIsAttacking = false;

	DamageComp->DeactivateDamageCollision();
}
