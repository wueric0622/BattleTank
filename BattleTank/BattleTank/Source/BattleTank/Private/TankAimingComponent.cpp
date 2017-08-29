// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret * TurretToSet)
{
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation,float LunchSpeed)
{
	if (!Barrel) { return; }
	UE_LOG(LogTemp, Warning, TEXT("HitLocation : %s"), *HitLocation.ToString());
	FVector OutLunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projecttile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLunchVelocity,
		StartLocation,
		HitLocation,
		LunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bHaveAimSolution)
	{
		auto AimDirection = OutLunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		TurnTurretTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	Barrel->Eleate(DeltaRotator.Pitch);
}

void UTankAimingComponent::TurnTurretTowards(FVector AimDirection)
{
	auto TurretRotator = Turret->GetComponentRotation();
	auto AimasRotator = AimDirection.Rotation();
	auto DeltaRotator = AimasRotator - TurretRotator;
	Turret->TurretRotate(DeltaRotator.Yaw);
}

