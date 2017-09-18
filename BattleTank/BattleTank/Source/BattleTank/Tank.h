// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Templates/Casts.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,Category = "Health")
		float GetHealthPercent();
	FTankDelegate OnDeath;

private:
	// Sets default values for this pawn's properties
	ATank();
	UPROPERTY(EditDefaultsOnly,Category = "Setup")
		int MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		int CurrentHealth;
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
