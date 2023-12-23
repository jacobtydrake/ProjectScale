// Written by Jacob Drake - 2023

#include "PSScaleWithPhysics.h"
#include "PaperFlipbookComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectScale/Actors/PSPickupItem.h"
#include "ProjectScale/Utils/PSGlobals.h"

APSScaleWithPhysics::APSScaleWithPhysics()
{
	PrimaryActorTick.bCanEverTick = true;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    RootComponent = SceneComp;

    PhysicsSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PhysicsSphere->SetupAttachment(RootComponent);
    PhysicsSphere->SetSimulatePhysics(true);
    PhysicsSphere->SetEnableGravity(true);
    PhysicsSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    PhysicsSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);


    PickupFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PickupFlipbook"));
    PickupFlipbookComp->SetupAttachment(PhysicsSphere);
    PickupFlipbookComp->SetMobility(EComponentMobility::Movable);
    //PickupFlipbookComp->SetRelativeRotation(PSGlobals::SpriteRotation);
}

void APSScaleWithPhysics::BeginPlay()
{
    Super::BeginPlay();
}

void APSScaleWithPhysics::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PickupFlipbookComp)
    {
        FRotator ModifiedRotation = PhysicsSphere->GetRelativeRotation();
        ModifiedRotation.Roll = PSGlobals::SpriteRotation.Roll;
        PhysicsSphere->SetRelativeRotation(ModifiedRotation);
    }
}

void APSScaleWithPhysics::SetSelectedFlipBook(EPickupItemType SelectedScale)
{
    switch (SelectedScale)
    {
    case EPickupItemType::OrangeScale:
        PickupFlipbookComp->SetFlipbook(OrangeScaleFlipbook);
        break;

    case EPickupItemType::BlueScale:
        PickupFlipbookComp->SetFlipbook(BlueScaleFlipbook);
        break;

    case EPickupItemType::PurpleScale:
        PickupFlipbookComp->SetFlipbook(PurpleScaleFlipbook);
        break;

    case EPickupItemType::BlackScale:
        PickupFlipbookComp->SetFlipbook(BlackScaleFlipbook);
        break;

    case EPickupItemType::GoldScale:
        PickupFlipbookComp->SetFlipbook(GoldScaleFlipbook);
        break;

    default:
        PickupFlipbookComp->SetFlipbook(OrangeScaleFlipbook);
    }
}
