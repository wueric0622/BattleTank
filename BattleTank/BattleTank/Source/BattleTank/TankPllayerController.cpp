// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPllayerController.h"
#include "GameFramework/Controller.h"


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

ATank* ATankPllayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}