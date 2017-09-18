// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Templates/Casts.h"
#include "AutoMortar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMortarDelegate);
UCLASS()
class BATTLETANK_API AAutoMortar : public APawn
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealthPercent();
	FMortarDelegate OnDeath;

private:
	// Sets default values for this pawn's properties
	AAutoMortar();
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		int MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Health")
		int CurrentHealth = MaxHealth;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	
	
};
