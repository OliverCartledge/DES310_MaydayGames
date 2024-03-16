// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_SingleShotFireComponent.h"

#include "Engine.h"
#include "CPP_Enemy.h"

// Sets default values
ATP_SingleShotFireComponent::ATP_SingleShotFireComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectiveMesh"));


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(400.0f);
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATP_SingleShotFireComponent::BeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ATP_SingleShotFireComponent::EndOverlap);

	ExplosionMesh->SetupAttachment(RootComponent);
	SphereComp->SetupAttachment(ExplosionMesh);
}

// Called when the game starts or when spawned
void ATP_SingleShotFireComponent::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Explosion met")));

	TimerStart();
}

void ATP_SingleShotFireComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATP_SingleShotFireComponent::TimerStart()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("TimerStartCalled")));

	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATP_SingleShotFireComponent::Explosion, 5.0f, true);
}

void ATP_SingleShotFireComponent::Explosion()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Explosion met")));
	Destroy();
}


void ATP_SingleShotFireComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("BeginOverlapCalled")));

	//ACPP_Enemy* enemyHit = Cast<ACPP_Enemy>(OtherActor);
	//if (OtherActor->ActorHasTag("Enemy"))
	//{
		//OtherActor->Destroy();
	//}
}

void ATP_SingleShotFireComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("EndOverlap")));
}



