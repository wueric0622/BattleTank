// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ParticleDefinitions.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Projectile.generated.h"

UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	UProjectileMovementComponent* ProjectileMovementComponent;	
public:	
	// Called every frame
	virtual void BeginPlay() override;
	void LunchProjectile(float LunnchSpeed);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* CollisionMesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UParticleSystemComponent* LaunchBlast;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UParticleSystemComponent* ImpactBlast;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* ExplosionForce;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float DelayTime = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float ProjectileDamage;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float DamageRadius;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void OnTimerExpire();
			
};
