// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_BuyableDoor.h"
#include "Engine.h"
#include "DES310_MaydayGames/DES310_MaydayGamesCharacter.h"
#include "MyPlayerState.h"
#include "CPP_BuyableDoor.h"

// Sets default values
ACPP_BuyableDoor::ACPP_BuyableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuyableDoor = FBox(FVector(-100, -100, -100), FVector(100, 100, 100));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BuyableDoor::BeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACPP_BuyableDoor::EndOverlap);
}

//void ACPP_BuyableDoor::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
//void ACPP_BuyableDoor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void ACPP_BuyableDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADES310_MaydayGamesCharacter* Character = Cast<ADES310_MaydayGamesCharacter>(OtherActor);
	//Check if player character has entered the zone (this makes sure it wont tick on enemys too lol)
	if (OtherActor && OtherActor->IsA(ADES310_MaydayGamesCharacter::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("InBuyableRange"));

		AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));

		//MyPlayerState->updateScore(50);
		if (MyPlayerState->myGetScore() >= 50)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Door Is Buyable!"));
			//if (Character != nullptr)
			//{
				// Set up action bindings
				if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
				{
					if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
					{
						// Fire
						EnhancedInputComponent->BindAction(BuyDoorAction, ETriggerEvent::Triggered, this, &ACPP_BuyableDoor::RemoveDoor);
					}

				}
			//}
		}
		else
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Door Is not Buyable!"));


		
	}
}

void ACPP_BuyableDoor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ADES310_MaydayGamesCharacter::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OutBuyableRange"));
	}
}

void ACPP_BuyableDoor::RemoveDoor()
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	MyPlayerState->updateScore(-50);
	Destroy();
}