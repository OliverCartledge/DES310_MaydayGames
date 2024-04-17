// Copyright Epic Games, Inc. All Rights Reserved.

#include "DES310_MaydayGamesProjectile.h"
#include "CPP_Enemy.h"
#include "TP_SingleShotFireComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ADES310_MaydayGamesProjectile::ADES310_MaydayGamesProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADES310_MaydayGamesProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ADES310_MaydayGamesProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, EmitterClass, GetActorLocation());

	ACPP_Enemy* enemyHit = Cast<ACPP_Enemy>(OtherActor);
	UClass* ExplosionClass = ATP_SingleShotFireComponent::StaticClass();
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)/* && OtherActor->ActorHasTag("Enemy")*/)
	{
		FVector SpawnLocation = Hit.ImpactPoint;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AActor>(ExplosionClass, SpawnLocation, SpawnRotation);

		//OtherComp->DestroyComponent();

		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, SpawnLocation);
		}
	}
	Destroy();
} 