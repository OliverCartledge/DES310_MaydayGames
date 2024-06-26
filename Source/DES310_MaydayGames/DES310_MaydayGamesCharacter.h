#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "DES310_MaydayGamesCharacter.generated.h"

//delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShouldShowCrosshair, bool, isADS);

//forward declare
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UMyGameInstance;



UCLASS(config=Game)
class ADES310_MaydayGamesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	//Check ADS to stop movement 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ADSAction;

	//Crouch action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	
public:
	ADES310_MaydayGamesCharacter();

protected:
	virtual void BeginPlay();
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void HealPlayer();

	UFUNCTION()
	void ADSPressed();

	UFUNCTION()
	void ADSReleased();

public:

	UPROPERTY(BlueprintAssignable, category = "crosshair")
		FShouldShowCrosshair ShouldShowCrosshair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* WalkSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAudioComponent* TestWalk;

	UFUNCTION(BlueprintCallable, category = "UI")
		void shouldDisplayCrosshair(bool isADS);

	UFUNCTION(BlueprintImplementableEvent, category = "UI")
		void displayCrosshair();

	UFUNCTION(BlueprintImplementableEvent, category = "UI")
		void hideCrosshair();

	//Collision component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UBoxComponent* CollisionBox;

	//Input action for looking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	//Setter for the gun
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	//Getter for if the player has the gun
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UFUNCTION(BlueprintImplementableEvent, Category="deathScreen")
	void deathScreen();

	//Function to check the players jump status to enable/disable shooting and crosshair display
	UFUNCTION(BlueprintCallable)
	bool GetJumpStatus();

	//Initialized players health
	UPROPERTY(BlueprintReadWrite, category="health")
	float playerHealth = 0;

	//Float to display the damage vignette overlay 
	UPROPERTY(BlueprintReadWrite, category = "health")
	float oppasityHeath = 0;

	bool IsADS;

	//Bool for checking if the player is in the air (used to allow for firing or displaying the crosshair)
	bool InAirTest;
	
	//Bool for the tick to heal the player
	UPROPERTY(BlueprintReadWrite, category = "health")
	bool Healing;

	//Bool for the tick to deal damage
	UPROPERTY(BlueprintReadWrite, category = "health")
	bool TakeDamage;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	FTimerHandle DamageTimeHandle;

	FTimerHandle HealingTimeHandle;

private: 
	//Bool to check if the walk sound is being played to avoid overlapping sounds
	bool bWalkSoundPlayed;


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

