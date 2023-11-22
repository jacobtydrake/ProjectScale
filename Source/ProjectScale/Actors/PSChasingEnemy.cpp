// Written by Jacob Drake - 2023


#include "PSChasingEnemy.h"
#include "PSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectScale/Components/PSDamageComponent.h"
#include "TimerManager.h"
#include "PSPickupItem.h"

APSChasingEnemy::APSChasingEnemy()
{
    //Tags.Add("Enemy");
    DamageComp = CreateDefaultSubobject<UPSDamageComponent>(TEXT("DamageComponent"));
    DamageComp->SetTeamTag("Enemy");
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
   if (!bIsAttacking)
   {
       UpdateChaseBehavior();
   }
   Super::Tick(DeltaTime);
}

void APSChasingEnemy::Die()
{
    Super::Die();

    TimerManager->ClearTimer(AttackTimerHandle);
    TimerManager->ClearTimer(LaunchDelayTimerHandle);
    TimerManager->ClearTimer(LaunchTimerHandle);
}

void APSChasingEnemy::UpdateDropChances()
{
    CustomDropChances.Add(EPickupItemType::BlueScale, 90.0f);
    CustomDropChances.Add(EPickupItemType::Speed, 6.0f);
    CustomDropChances.Add(EPickupItemType::Health, 3.0f);
    CustomDropChances.Add(EPickupItemType::ScreenWipe, 1.0f);
}

void APSChasingEnemy::UpdateChaseBehavior()
{
    if (PlayerCharacter && !bIsAttacking)
    {
        const float DistanceToPlayer = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

        if (bHasInfiniteDetectionRange || DistanceToPlayer <= ChaseRange)
        {
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
    UE_LOG(PSEnemy, Display, TEXT("Enemy Performing Attack..."));
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
    DamageComp->ActivateDamageCollision();
    bIsLaunching = true;
    GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &APSChasingEnemy::HandleLaunch, 0.016f, true);
}

void APSChasingEnemy::HandleLaunch()
{
    if (bIsLaunching) {
        // update the enemy's position based on the launch velocity
        FVector NewLocation = GetActorLocation() + MovementDirection * 1000.0f * GetWorld()->GetDeltaSeconds(); //TODO replace 1000.0f with var
        SetActorLocation(NewLocation);

        // reduce the launch duration
        LaunchDuration -= GetWorld()->GetDeltaSeconds();
        if (LaunchDuration <= 0) {
            // stop the launch
            bIsLaunching = false;
            LaunchDuration = 0.3f;
            DamageComp->DeactivateDamageCollision();
            GetWorldTimerManager().ClearTimer(LaunchTimerHandle);
        }
    }
}
