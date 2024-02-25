#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyPlayerState.h"
#include "GameFramework/Actor.h"
#include "TP_SingleShotFireComponent.generated.h"

//class UParticleSystemComponent;
class APlayerCharacter;
class MyPlayerState;


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DES310_MAYDAYGAMES_API UTP_SingleShotWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	//this weapon is the base-class pistol
	UPROPERTY(BlueprintReadWrite, category = "score")
	float weaponDamage = 100.f;

	UPROPERTY(BlueprintReadWrite, category = "score")
	int playerScore = 0;

	UPROPERTY(BlueprintReadWrite, category = "ammo")
	int ammoCurrent = 0;

	UPROPERTY(BlueprintReadWrite, category = "ammo")
	int ammoMax = 0;

	/** Projectile class to spawn */
	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
	//TSubclassOf<class APlayerCharacter> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Secondary Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryFireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;


	/** Sets default values for this component's properties */
	UTP_SingleShotWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(APlayerCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable)
	bool LineTraceShot(FHitResult& OutHit);

	UFUNCTION(BlueprintImplementableEvent, Category = "winScreen")
	void winScreen();

	UFUNCTION(Category = "HUD")
	void showScore();


	/*UPROPERTY(EditAnywhere, category = "particle")
	UParticleSystemComponent* ParticleSystem;*/


protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ADSPressed();

	void ADSReleased();

	void Reload();

	void StartShoot();

	void EndShoot();

	////commented out as this CANNOT be used in a skeletalmesh inherited class. Must be AACTOR so move this code.
	// //fire rate for weapon
	FTimerHandle GunFireRate;
	void TimerExpired();
	void StartTimer();
	bool canFire = true;

	//Timer for full auto
	FTimerHandle ShootingTimer;

	UPROPERTY(BlueprintReadOnly)
	bool IsADS;



private:
	/** The Character holding this weapon*/
	APlayerCharacter* Character;

	bool IsShooting;

	float bulletCount = 30;
};
