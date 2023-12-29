// Written by Jacob Drake - 2023

#include "PSPickupItem.h"
#include "Components/SphereComponent.h"
#include "ProjectScale/Actors/PSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "ProjectScale/Components/PSPickupItemWidgetComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "ProjectScale/Utils/PSGlobals.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

APSPickupItem::APSPickupItem()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComp;

    PickupCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollisionComp->SetupAttachment(RootComponent);
    PickupCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APSPickupItem::OnOverlapBegin);
    PickupCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    PickupFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PickupFlipbook"));
    PickupFlipbookComp->SetupAttachment(RootComponent);
    PickupFlipbookComp->SetRelativeRotation(PSGlobals::SpriteRotation);

    PSPickupItemWidgetComp = CreateDefaultSubobject<UPSPickupItemWidgetComponent>(TEXT("PSPickupItemWidget"));
    PSPickupItemWidgetComp->SetupAttachment(RootComponent);
    PSPickupItemWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PSPickupItemWidgetComp->SetCastShadow(false);
    PSPickupItemWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

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
    SetLifeSpan(Lifespan);

    if (SceneComp)
    {
        const FVector ComponentLocation = SceneComp->GetComponentLocation();
        const FVector AdjustedWorldLocation = FVector(ComponentLocation.X, ComponentLocation.Y, InitialZ);
        SceneComp->SetRelativeLocation(AdjustedWorldLocation);
        SceneComp->SetWorldRotation(FRotator(0, 0, 0));
    }
}

void APSPickupItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // bobbing effect
    FVector NewLocation = GetActorLocation();
    const float DeltaHeight = FMath::Sin(RunningTime * BobSpeed) * BobAmplitude;
    NewLocation.Z = InitialZ + DeltaHeight;
    RunningTime += DeltaTime;
    SetActorLocation(NewLocation);

    if (!bIsInCircle)
    {
        MoveTowardsCircle(DeltaTime);
    }

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
        if (OtherComp == PlayerCharacter->GetCapsuleComponent())
        {
            PlayerCharacter->OnItemPickup(ItemType);
            OnPickedUp();
        }
    }
}

void APSPickupItem::OnPickedUp()
{
    PickupFlipbookComp->SetVisibility(false);
    PickupCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    PSPickupItemWidgetComp->ActivePickupItemWidget(ItemType);

    if (SelectedSound) UGameplayStatics::PlaySound2D(this, SelectedSound);

    SetLifeSpan(3.5f);

    GetWorldTimerManager().ClearTimer(FlashTimerHandle);
}

void APSPickupItem::SelectRandomItemType()
{
    float TotalChance = 0.0f;
    for (const auto& Pair : ItemChances)
    {
        TotalChance += Pair.Value;
    }

    const float RandomValue = FMath::RandRange(0.0f, TotalChance);
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

    UPaperFlipbook* SelectedPickupItem = OrangeScaleFlipbook;
    switch (ItemType)
    {
    case EPickupItemType::OrangeScale:
        SelectedPickupItem = OrangeScaleFlipbook;
        SelectedSound = OrangeScaleSound;
        break;
    case EPickupItemType::BlueScale:
        SelectedPickupItem = BlueScaleFlipbook;
        SelectedSound = BlueScaleSound;
        break;
    case EPickupItemType::PurpleScale:
        SelectedPickupItem = PurpleScaleFlipbook;
        SelectedSound = PurpleScaleSound;
        break;
    case EPickupItemType::BlackScale:
        SelectedPickupItem = BlackScaleFlipbook;
        SelectedSound = BlackScaleSound;
        break;
    case EPickupItemType::GoldScale:
        SelectedPickupItem = GoldScaleFlipbook;
        SelectedSound = GoldScaleSound;
        break;
    case EPickupItemType::Health:
        SelectedPickupItem = HealthFlipbook;
        SelectedSound = OrangeScaleSound;
        break;
    case EPickupItemType::Speed:
        SelectedPickupItem = SpeedFlipbook;
        SelectedSound = SpeedSound;
        break;
    case EPickupItemType::Attack:
        SelectedPickupItem = AttackFlipbook;
        SelectedSound = AttackSound;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown pickup item type."));
        break;
    }

     PickupFlipbookComp->SetFlipbook(SelectedPickupItem);
}

void APSPickupItem::EnableCollision() const
{
    PickupCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APSPickupItem::StartFlashing()
{
    GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &APSPickupItem::FlashEffect, 0.25f, true);
}

void APSPickupItem::FlashEffect() const
{
    const bool bIsVisible = PickupFlipbookComp->IsVisible();
    PickupFlipbookComp->SetVisibility(!bIsVisible);
}

void APSPickupItem::MoveTowardsCircle(float DeltaTime)
{
    const FVector CurrentLocation = GetActorLocation();
    FVector ToCenter = CircleCenter - CurrentLocation;
    ToCenter.Z = 0;

    if (ToCenter.SizeSquared() > FMath::Square(CircleRadius))
    {
        const FVector Direction = ToCenter.GetSafeNormal();
        const FVector NewLocation = CurrentLocation + Direction * MovementSpeedTowardsCircle * DeltaTime;
        SetActorLocation(NewLocation);
    }
    else
    {
        bIsInCircle = true;
    }
}
