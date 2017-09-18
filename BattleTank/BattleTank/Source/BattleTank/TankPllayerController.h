// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPllayerController.generated.h"

class UTankAimingComponent;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPllayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
		void FoundAimingComponent(UTankAimingComponent*AimCompRef);
	UFUNCTION()
		void OnProssedTankDeath();
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn * InPawn) override;

	UPROPERTY(EditAnywhere)
		float CrossHairXLocation = 0.5f;
	UPROPERTY(EditAnywhere)
		float CrossHairYLocation = 0.33333f;
	UPROPERTY(EditAnywhere)
		float LookTraceRange = 1000000;


	void AimTowardCrosshair();
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector &LookLocation, FVector &LookDirection) const;
	bool GetlookVectorHitLocation(FVector LookLocation, FVector LookDirection, FVector &HitLocation) const;
};
