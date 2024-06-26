// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_SingleShotFireComponent.h"

#include "Engine.h"
#include "CPP_Enemy.h"
#include "DES310_MaydayGames/DES310_MaydayGamesCharacter.h"

// Sets default values
ATP_SingleShotFireComponent::ATP_SingleShotFireComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ExplosionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectiveMesh"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->InitSphereRadius(400.0f);
	SphereComp->BodyInstance.SetCollisionProfileName("Projectile");
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATP_SingleShotFireComponent::BeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ATP_SingleShotFireComponent::EndOverlap);

	ExplosionMesh->SetupAttachment(RootComponent);
	SphereComp->SetupAttachment(ExplosionMesh);

    
}

// Called when the game starts or when spawned
void ATP_SingleShotFireComponent::BeginPlay()
{
	Super::BeginPlay();

	TimerStart();

}

void ATP_SingleShotFireComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATP_SingleShotFireComponent::TimerStart()
{
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATP_SingleShotFireComponent::Explosion, 0.5f, true);
}

void ATP_SingleShotFireComponent::Explosion()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Explosion met")));
	Destroy();
}

void ATP_SingleShotFireComponent::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("BeginOverlapCalled")));
    AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));

    ACPP_Enemy* enemyHit = Cast<ACPP_Enemy>(OtherActor);
    ADES310_MaydayGamesCharacter* playerHit = Cast<ADES310_MaydayGamesCharacter>(OtherActor);

    UGameplayStatics::SpawnEmitterAtLocation(this, EmitterClass, GetActorLocation());

    if (enemyHit && OtherActor->ActorHasTag("Enemy"))
    {
        // Temporarily stop the enemy's movement
        enemyHit->GetCharacterMovement()->StopMovementImmediately();

        //get enemy health
        float currentHealth = enemyHit->enemyHealth;
        currentHealth -= grenadeDamage; //apply damage

        //if enemy has no health left, destroy the actor
        if (currentHealth <= 0)
        {
            enemyHit->Destroy();
            MyPlayerState->updateScore(enemyHit->enemyGiveScore);
        }
        //else, update health value
        else
        {
            enemyHit->enemyHealth = currentHealth;
        }

        //calculate the direction away from the explosion
        FVector ImpulseDirection = (enemyHit->GetActorLocation() - GetActorLocation()).GetSafeNormal();

        float ImpulseMagnitude = 10000.0f; //change this to control the imoulse of the grenade

        //upward impulse
        float UpwardForce = 500.0f; 
        ImpulseDirection.Z += UpwardForce;

        //backwards impulse
        FVector ImpulseForce = ImpulseDirection * ImpulseMagnitude;
        enemyHit->GetCharacterMovement()->Velocity += ImpulseForce;

        //timer allows the enemy to move again after a delay
        /*FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(UnusedHandle, [enemyHit]()
            {
                enemyHit->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
            }, 1.5f, false);*/
    }
    //if (playerHit && OtherActor->ActorHasTag("Player"))
    //{
    //    FVector ImpulseDirection = (playerHit->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    //    float ImpulseMagnitude = 10000.0f; //change this to control the imoulse of the grenade

    //    //upward impulse
    //    float UpwardForce = 500.0f;
    //    ImpulseDirection.Z += UpwardForce;

    //    //backwards impulse
    //    FVector ImpulseForce = ImpulseDirection * ImpulseMagnitude;
    //    playerHit->GetCharacterMovement()->Velocity += ImpulseForce;

    //    playerHit->DealDamage();
    //}
}

void ATP_SingleShotFireComponent::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("EndOverlap")));
}



