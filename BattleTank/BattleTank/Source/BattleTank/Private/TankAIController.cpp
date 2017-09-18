// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnProssedTankDeath);
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ConntrolledTank = GetPawn();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerTank && ConntrolledTank)) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent) || !ensure(PlayerTank)) { return; }
	MoveToActor(PlayerTank, AcceptanceRadius);
	AimingComponent->AimAt(PlayerTank->GetActorLocation());
	if (AimingComponent->GetFiringState() == EFiringStatus::Locked)
	{
		AimingComponent->Fire();
	}
}


void ATankAIController::OnProssedTankDeath()
{
	auto ConntrolledTank = GetPawn();
	ConntrolledTank->DetachFromControllerPendingDestroy();
	UE_LOG(LogTemp, Warning, TEXT("Death"));
}
