// Written by Jacob Drake - 2023


#include "PSChasingEnemy.h"
#include "PSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectScale/Components/PSDamageComponent.h"
#include "TimerManager.h"
#include "PSPickupItem.h"

APSChasingEnemy::APSChasingEnemy()
{
    DamageComp = CreateDefaultSubobject<UPSDamageComponent>(TEXT("DamageComponent"));
    DamageComp->SetTeamTag("Enemy");
    MovementSpeed = 150;
}

void APSChasingEnemy::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = Cast<APSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (DamageComp)
    {
        DamageComp->DeactivateDamageCollision();
    }

    TimerManager = &GetWorld()->GetTimerManager();
}

void APSChasingEnemy::Tick(float DeltaTime)
{
   if (!bIsAttacking && !bIsCharacterDead)
   {
       UpdateChaseBehavior();
   }
   Super::Tick(DeltaTime);
}

void APSChasingEnemy::Die()
{
    Super::Die();

    if (DamageComp)
    {
        DamageComp->DeactivateDamageCollision();
    }

    TimerManager->ClearTimer(AttackTimerHandle);
    TimerManager->ClearTimer(LaunchDelayTimerHandle);
    TimerManager->ClearTimer(LaunchTimerHandle);
}

void APSChasingEnemy::UpdateDropChances()
{
    CustomDropChances.Add(EPickupItemType::BlueScale, 97.50f);
    CustomDropChances.Add(EPickupItemType::Speed, 1.0f);
    CustomDropChances.Add(EPickupItemType::Health, 0.5f);
    CustomDropChances.Add(EPickupItemType::Attack, 1.0f);
}

void APSChasingEnemy::UpdateChaseBehavior()
{
    if (PlayerCharacter && !bIsAttacking)
    {
        const float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

        if (bHasInfiniteDetectionRange || DistanceToPlayer <= ChaseRange)
        {
            bHasInfiniteDetectionRange = true;
            FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            DirectionToPlayer.Z = 0.0f; // don't change in vertical direction
            MovementDirection = DirectionToPlayer;

            const float CurrentTime = GetWorld()->GetTimeSeconds();

            if (DistanceToPlayer <= AttackRange && CurrentTime - LastAttackTime > AttackCooldown)
            {
                bShouldMove = false;
                bIsAttacking = true;
                PerformAttack();
            }
        }
    }
}

void APSChasingEnemy::PerformAttack()
{
    GetWorldTimerManager().SetTimer(LaunchDelayTimerHandle, this, &APSChasingEnemy::StartLaunch, AttackToLaunchDelay, false);
    GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &APSChasingEnemy::EndAttack, TotalAttackDuration, false);
    LastAttackTime = GetWorld()->GetTimeSeconds();
}

void APSChasingEnemy::EndAttack()
{
    bIsAttacking = false;
    bShouldMove = true;
}

void APSChasingEnemy::StartLaunch()
{
    bIsLaunching = true;
    CurrentLaunchDuration = TotalLaunchDuration;
    GetWorldTimerManager().SetTimer(DamageCollisionTimerHandle, this, &APSChasingEnemy::ToggleDamageCollision,  .2, false);
    GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &APSChasingEnemy::HandleLaunch, 0.016f, true);
}

void APSChasingEnemy::HandleLaunch()
{
    if (bIsLaunching) 
    {
        // update the enemy's position based on the launch velocity
        FVector NewLocation = GetActorLocation() + MovementDirection * AttackThrustPower * GetWorld()->GetDeltaSeconds();
        SetActorLocation(NewLocation);

        // reduce the launch duration
        CurrentLaunchDuration -= GetWorld()->GetDeltaSeconds();
        if (CurrentLaunchDuration <= 0) {
            // stop the launch
            bIsLaunching = false;
            CurrentLaunchDuration = TotalLaunchDuration;
            DamageComp->DeactivateDamageCollision();
            GetWorldTimerManager().ClearTimer(LaunchTimerHandle);
        }
    }
}

void APSChasingEnemy::ToggleDamageCollision()
{
    if (DamageComp->GetIsDamageCollisionActive())
    {
        DamageComp->DeactivateDamageCollision();
    }
    else
    {
        DamageComp->ActivateDamageCollision();
    }
}
