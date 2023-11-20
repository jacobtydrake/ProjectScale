// Written by Jacob Drake - 2023

#include "PSPickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectScale/Actors/PSCharacter.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// Sets default values
APSPickupItem::APSPickupItem()
{
    PrimaryActorTick.bCanEverTick = true;

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMesh;

    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->SetupAttachment(RootComponent);
    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &APSPickupItem::OnOverlapBegin);

    PickupSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PickupSprite"));
    PickupSprite->SetupAttachment(RootComponent);

    ItemChances.Add(EPickupItemType::Scale, 90.0f);
    ItemChances.Add(EPickupItemType::Health, 3.0f);    
    ItemChances.Add(EPickupItemType::Speed, 6.0f);
    ItemChances.Add(EPickupItemType::ScreenWipe, 1.0f); 

}

void APSPickupItem::BeginPlay()
{
    Super::BeginPlay();
    SelectRandomItemType();
}

void APSPickupItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void APSPickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APSCharacter* PlayerCharacter = Cast<APSCharacter>(OtherActor))
    {
        OnPickedUp();
    }
}

void APSPickupItem::OnPickedUp()
{
    // sound, anim, etc

    Destroy();
}

void APSPickupItem::SelectRandomItemType()
{
    float TotalChance = 0.0f;
    for (const auto& Pair : ItemChances)
    {
        TotalChance += Pair.Value;
    }

    float RandomValue = FMath::RandRange(0.0f, TotalChance);
    float AccumulatedChance = 0.0f;

    for (const auto& Pair : ItemChances)
    {
        AccumulatedChance += Pair.Value;
        if (RandomValue <= AccumulatedChance)
        {
            ItemType = Pair.Key;
            break;
        }
    }

    switch (ItemType)
    {
    case EPickupItemType::Scale:
        PickupSprite->SetSprite(ScaleSpriteAsset);
        break;
    case EPickupItemType::Health:
        PickupSprite->SetSprite(HealthSpriteAsset);
        break;
    case EPickupItemType::Speed:
        PickupSprite->SetSprite(SpeedSpriteAsset);
        break;
    case EPickupItemType::ScreenWipe:
        PickupSprite->SetSprite(ScreenWipeSpriteAsset);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown pickup item type."));
        break;
    }
}
