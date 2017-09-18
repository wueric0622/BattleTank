// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPllayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Tank.h"
#include "TankAimingComponent.h"


void ATankPllayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPllayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}

void ATankPllayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPllayerController::OnProssedTankDeath);
	}
}

void ATankPllayerController::AimTowardCrosshair()
{
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector OutHitLocation = FVector(2.0f);
	if (GetSightRayHitLocation(OutHitLocation))
	{
		AimingComponent->AimAt(OutHitLocation);
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
		if (GetlookVectorHitLocation(LookLocation, LookDirection, HitLocation))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
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
		//UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit : %s"), *HitResult.Actor->GetName());
		return true;
	}
	else
	{
		return false;
	}
	
}

void ATankPllayerController::OnProssedTankDeath()
{
	StartSpectatingOnly();
	UE_LOG(LogTemp, Warning, TEXT("Death"));
}
