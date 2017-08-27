// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPllayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


void ATankPllayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GetControlledTank())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current Control Tank is %s"), *GetControlledTank()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller Can't get pawn"));
	}
}

void ATankPllayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}

ATank* ATankPllayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPllayerController::AimTowardCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector OutHitLocation = FVector(2.0f);
	if (GetSightRayHitLocation(OutHitLocation))
	{
		GetControlledTank()->AimAt(OutHitLocation);
	}
}

bool ATankPllayerController::GetSightRayHitLocation(FVector &HitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX*CrossHairXLocation, ViewportSizeY*CrossHairYLocation);
	FVector LookDirection = FVector(1.0f);
	FVector LookLocation = FVector(1.0f);
	if (GetLookDirection(ScreenLocation, LookLocation, LookDirection))
	{
		FVector HitLocation = FVector(1.0f);
		GetlookVectorHitLocation(LookLocation, LookDirection, HitLocation);
	}
	return true;
}

bool ATankPllayerController::GetLookDirection(FVector2D ScreenLocation, FVector &LookLocation, FVector &LookDirection) const
{
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, LookLocation, LookDirection);
}

bool ATankPllayerController::GetlookVectorHitLocation(FVector LookLocation, FVector LookDirection, FVector &HitLocation) const
{
	FHitResult HitResult;
	FVector LineTraceEndPoint = LookLocation + (LookDirection*LookTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, LookLocation, LineTraceEndPoint, ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit : %s"), *HitResult.Actor->GetName());
		return true;
	}
	else
	{
		return false;
	}
	
}
