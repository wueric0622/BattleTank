// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds && AmmoAmount > 0)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if(!IsBarrelMoving() && AmmoAmount > 0)
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else if(AmmoAmount <= 0)
	{
		FiringStatus = EFiringStatus::NoAmmo;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}
}

EFiringStatus UTankAimingComponent::GetFiringState() const { return FiringStatus; }

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	return Barrel->GetForwardVector().Equals(AimDirection, 0.1f);
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)|| !ensure(Turret)) { return; }
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
		AimDirection = OutLunchVelocity.GetSafeNormal();
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
	if (FMath::Abs(DeltaRotator.Yaw) > 180)
	{
		Turret->TurretRotate(-DeltaRotator.Yaw);
	}
	else
	{
		Turret->TurretRotate(DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel)) { return; }
	if (FiringStatus != EFiringStatus::Reloading || FiringStatus != EFiringStatus::NoAmmo)
	{
		FVector SpawnLocation = Barrel->GetSocketLocation(FName("Projecttile"));
		FRotator SpawnRotaion = Barrel->GetSocketRotation(FName("Projecttile"));
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotaion);
		Projectile->LunchProjectile(LunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		AmmoAmount--;
	}
}



