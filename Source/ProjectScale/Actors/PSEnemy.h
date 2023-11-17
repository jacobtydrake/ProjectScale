

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectScale/Interfaces/PSCombatInterface.h"
#include "PSEnemy.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PSEnemy, Log, All);

class UCapsuleComponent;
class UPaperFlipbookComponent;
class UMaterialInstanceDynamic;

UCLASS()
class PROJECTSCALE_API APSEnemy : public AActor, public IPSCombatInterface
{
	GENERATED_BODY()

public:
	APSEnemy();
	APSEnemy(FVector NewDirection);

	UFUNCTION(BlueprintCallable)
	void InitializeDirection(const FVector& NewDirection);

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> DeathEffect;

	/**/
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	/**/
	void RevertSpriteColor();

	/**/
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth{ 1.0f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MovementDirection{ 1,0,0 };

	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove{ true };

};
