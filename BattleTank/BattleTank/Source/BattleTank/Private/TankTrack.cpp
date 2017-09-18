// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent ,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentTrottle = 0;
}

void UTankTrack::ApplySidewaysForce()
{
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto CorrentionAcceleration = -SlippageSpeed / DeltaTime*GetRightVector();
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrentionForce = (TankRoot->GetMass() * CorrentionAcceleration) / 2;
	TankRoot->AddForce(CorrentionForce);
}

void UTankTrack::SetThrottle(float Trottle)
{
	CurrentTrottle = FMath::Clamp<float>(CurrentTrottle + Trottle, -1, 1);
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentTrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Name: %s  ::  %s"), *GetName(),*ForceLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Name: %s  ::   %s,%s"), *GetName(),*ForceApplied.ToString(), *ForceLocation.ToString());
}


