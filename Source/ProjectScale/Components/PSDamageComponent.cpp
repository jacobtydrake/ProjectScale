
#include "PSDamageComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPSDamageComponent::UPSDamageComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Create the box component and attach it to the component's owner
    DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
    DamageCollision->SetBoxExtent(DamageCollisionSize);
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Start with collision off
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

    UE_LOG(LogTemp, Warning, TEXT("ActivateDamageCollision"));
}

void UPSDamageComponent::DeactivateDamageCollision()
{
    DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageCollision->SetGenerateOverlapEvents(false);

    UE_LOG(LogTemp, Warning, TEXT("DeactivateDamageCollision"));
}

void UPSDamageComponent::BeginPlay()
{
	Super::BeginPlay();

    if (GetOwner()->GetRootComponent())
    {
        DamageCollision->SetupAttachment(GetOwner()->GetRootComponent());
    }
}

void UPSDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Only draw the debug box if collision is enabled
 /*   if (DamageCollision->IsCollisionEnabled())
    {*/
        DrawDebugBox(
            GetWorld(),
            DamageCollision->GetComponentLocation(),
            DamageCollision->GetScaledBoxExtent(),
            FQuat(DamageCollision->GetComponentRotation()),
            FColor::Red,
            false,
            0.1f,
            10,
            20
        );
    //}
}

void UPSDamageComponent::OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == GetOwner()) return;
        
    if (OtherActor->Implements<UPSCombatInterface>())
    {
        IPSCombatInterface::Execute_TakeDamage(OtherActor, DamageAmount);
    }
}

