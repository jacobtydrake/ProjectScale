
#include "PSDamageComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UPSDamageComponent::UPSDamageComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Create the box component and attach it to the component's owner
    DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
    DamageCollision->SetBoxExtent(DamageCollisionSize);
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetRelativeLocation(DamageCollisionRelativeLocation);
    DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &UPSDamageComponent::OnDamageCollisionOverlap);
}

TObjectPtr<UBoxComponent> UPSDamageComponent::GetDamageCollisionBox()
{
    return DamageCollision;
}

void UPSDamageComponent::ActivateDamageCollision()
{
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageCollision->SetGenerateOverlapEvents(true);

    bIsDamageCollisionActive = true;
}

void UPSDamageComponent::DeactivateDamageCollision()
{
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetGenerateOverlapEvents(false);

    bIsDamageCollisionActive = false;
}

void UPSDamageComponent::BeginPlay()
{
	Super::BeginPlay();

    if (AActor* OwnerActor = GetOwner())
    {
        if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(OwnerActor->GetRootComponent()))
        {
            DamageCollision->AttachToComponent(CapsuleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            DamageCollision->SetRelativeLocation(DamageCollisionRelativeLocation);
        }
    }
}

void UPSDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (DamageCollision->IsCollisionEnabled())
    {
        DrawDebugBox
        (
            GetWorld(),
            DamageCollision->GetComponentLocation(),
            DamageCollision->GetScaledBoxExtent(),
            FQuat(DamageCollision->GetComponentRotation()),
            FColor::Red,
            false,
            0.01f,
            10,
            2
        );
    }
}

void UPSDamageComponent::OnDamageCollisionOverlap
(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, 
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, 
    bool bFromSweep, 
    const FHitResult& SweepResult
)
{
    AActor* OwnerActor = GetOwner();

    // Check if the OtherActor has the required tag
    if (OtherActor->ActorHasTag(TeamTag))
    {
        return;
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Tags are different."))
    }
    if (OtherActor->Implements<UPSCombatInterface>())
    {
        IPSCombatInterface::Execute_TakeDamage(OtherActor, DamageAmount);
    }
}

