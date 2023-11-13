

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "PSEnemy.generated.h"

class UCapsuleComponent;
class UPaperFlipbookComponent;
class UMaterialInstanceDynamic;

UCLASS()
class PROJECTSCALE_API APSEnemy : public AActor, public IPSCombatInterface
{
	GENERATED_BODY()
	
public:	
	APSEnemy();

protected:
	/* Overrides */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* PSCombatInterface Implementation */
	virtual void TakeDamage_Implementation(const float DamageAmount);

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPaperFlipbookComponent> FlipbookComp;

public:


private:

	void RevertSpriteColor();

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
};
