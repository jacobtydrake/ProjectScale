// Written by Jacob Drake - 2023

#include "PSPickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectScale/Actors/PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

// Sets default values
APSPickupItem::APSPickupItem()
{
    PrimaryActorTick.bCanEverTick = true;

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    RootComponent = PickupMesh;

    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->SetupAttachment(RootComponent);
    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &APSPickupItem::OnOverlapBegin);

    PickupFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PickupFlipbook"));
    PickupFlipbook->SetupAttachment(RootComponent);
}

void APSPickupItem::BeginPlay()
{
    Super::BeginPlay();
}

void APSPickupItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APSPickupItem::InitializePickupItem(const TMap<EPickupItemType, float>& NewItemChances)
{
    ItemChances = NewItemChances;
    SelectRandomItemType();
}

void APSPickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APSCharacter* PlayerCharacter = Cast<APSCharacter>(OtherActor))
    {
        PlayerCharacter->OnItemPickup(ItemType);
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
    case EPickupItemType::BlueScale:
        PickupFlipbook->SetFlipbook(BlueScaleFlipbook);
        UE_LOG(LogTemp, Warning, TEXT("BlueScale Spawned"));
        break;
    case EPickupItemType::OrangeScale:
        PickupFlipbook->SetFlipbook(OrangeScaleFlipbook);
        UE_LOG(LogTemp, Warning, TEXT("OrangeScale Spawned"));
        break;
    case EPickupItemType::Health:
        PickupFlipbook->SetFlipbook(HealthFlipbook);
        break;
    case EPickupItemType::Speed:
        PickupFlipbook->SetFlipbook(SpeedFlipbook);
        break;
    case EPickupItemType::ScreenWipe:
        PickupFlipbook->SetFlipbook(ScreenWipeFlipbook);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown pickup item type."));
        break;
    }
}
