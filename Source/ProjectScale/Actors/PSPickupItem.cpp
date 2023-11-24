// Written by Jacob Drake - 2023

#include "PSPickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectScale/Actors/PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "ProjectScale/Components/PSPickupItemWidgetComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
APSPickupItem::APSPickupItem()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComp;

    PickupCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollisionComp->SetupAttachment(RootComponent);
    PickupCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APSPickupItem::OnOverlapBegin);

    PickupFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PickupFlipbook"));
    PickupFlipbookComp->SetupAttachment(RootComponent);

    PSPickupItemWidgetComp = CreateDefaultSubobject<UPSPickupItemWidgetComponent>(TEXT("PSPickupItemWidget"));
    PSPickupItemWidgetComp->SetupAttachment(RootComponent);

    const FRotator NewRotation = FRotator(30.0f, 90.0f, 0.0f); //TODO: make var
    PSPickupItemWidgetComp->SetRelativeRotation(NewRotation);

    const FVector WidgetOffset = FVector(0.0f, 0.0f, 60.0);  //TODO: make var
    PSPickupItemWidgetComp->SetRelativeLocation(WidgetOffset);

    PSPickupItemWidgetComp->SetCastShadow(false);



}

void APSPickupItem::BeginPlay()
{
    Super::BeginPlay();
    
    // start flashing based off total lifespan
    GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &APSPickupItem::StartFlashing, Lifespan - FlashTime, false);
    GetWorldTimerManager().SetTimer(LifespanTimerHandle, this, &APSPickupItem::OnPickedUp, Lifespan, false);
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

    PickupFlipbookComp->SetVisibility(false);
    PickupCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    PSPickupItemWidgetComp->ActivePickupItemWidget(ItemType);

    SetLifeSpan(3.5f);

    GetWorldTimerManager().ClearTimer(FlashTimerHandle);
    GetWorldTimerManager().ClearTimer(LifespanTimerHandle);
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
        PickupFlipbookComp->SetFlipbook(BlueScaleFlipbook);
        UE_LOG(LogTemp, Warning, TEXT("BlueScale Spawned"));
        break;
    case EPickupItemType::OrangeScale:
        PickupFlipbookComp->SetFlipbook(OrangeScaleFlipbook);
        UE_LOG(LogTemp, Warning, TEXT("OrangeScale Spawned"));
        break;
    case EPickupItemType::Health:
        PickupFlipbookComp->SetFlipbook(HealthFlipbook);
        break;
    case EPickupItemType::Speed:
        PickupFlipbookComp->SetFlipbook(SpeedFlipbook);
        break;
    case EPickupItemType::ScreenWipe:
        PickupFlipbookComp->SetFlipbook(ScreenWipeFlipbook);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown pickup item type."));
        break;
    }
}

void APSPickupItem::StartFlashing()
{
    GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &APSPickupItem::FlashEffect, 0.25f, true);
}

void APSPickupItem::FlashEffect()
{
    bool bIsVisible = PickupFlipbookComp->IsVisible();
    PickupFlipbookComp->SetVisibility(!bIsVisible);
}