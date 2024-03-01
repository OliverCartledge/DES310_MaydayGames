// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionZone.h"
#include "DES310_MaydayGames/DES310_MaydayGamesCharacter.h"
#include "Engine.h"

// Sets default values
AExtractionZone::AExtractionZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsExtracting = false;

	ExtractionBox = FBox(FVector(-100, -100, -100), FVector(100, 100, 100));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::BeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AExtractionZone::EndOverlap);
}
// Called when the game starts or when spawned
void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();

}
// Called every frame
void AExtractionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsExtracting)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Tick test"));

}
void AExtractionZone::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if player character has entered the zone (this makes sure it wont tick on enemys too lol)
	if (OtherActor && OtherActor->IsA(ADES310_MaydayGamesCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Entering Zone"));
		IsExtracting = true;
	}
}

void AExtractionZone::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ADES310_MaydayGamesCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Exiting Zone"));
		IsExtracting = false;
	}
}


