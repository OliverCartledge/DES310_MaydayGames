// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TP_WeaponComponent.generated.h"

//class UParticleSystemComponent;
class ADES310_MaydayGamesCharacter;


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DES310_MAYDAYGAMES_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:

	//this weapon is the base-class pistol
	float weaponDamage = 50.f;
	int playerScore = 0;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ADES310_MaydayGamesProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Secondary Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryFireAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ADES310_MaydayGamesCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable)
	bool LineTraceShot(FHitResult& OutHit);

	UFUNCTION(BlueprintImplementableEvent, Category = "winScreen")
	void winScreen();

	UFUNCTION(Category="HUD")
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

	////commented out as this CANNOT be used in a skeletalmesh inherited class. Must be AACTOR so move this code.
	// //fire rate for weapon
	FTimerHandle GunFireRate;
	void TimerExpired();
	void StartTimer();
	bool canFire = true;

private:
	/** The Character holding this weapon*/
	ADES310_MaydayGamesCharacter* Character;

	bool IsADS;

	float bulletCount = 10;
};
