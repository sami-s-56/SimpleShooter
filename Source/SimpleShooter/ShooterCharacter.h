// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Another way of Dealing Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

    bool IsAlive() const { return bIsAlive; }

	void FireWeapon();

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	int GetHealthPackCount() const;

private:

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
		float CharacterHealth;

	UPROPERTY(BlueprintReadOnly ,meta = (AllowPrivateAccess = "true"))
		bool bIsAlive = true;

	AGameModeBase* CurrentGameMode;

	UPROPERTY(VisibleAnywhere)
	bool bCanHack = false;

	UPROPERTY(VisibleAnywhere)
	bool bNearPickUp = false;

	//When near some pikup, it will store the overlapping actor temporarily
	class APickUpBase* OverlappingPickup = nullptr;

	TArray<int> HealthPacks;


	void MoveForward(float AxisVal);

	void MoveSideward(float AxisVal);

	void LookVerticle(float AxisVal);

	void LookHorizontal(float AxisVal);

	//Depending upon the game state, perform the required action
	void PerformAction();

	void HackTerminal();

	//Collect pickup on keypress
	void CollectPickup();

	void UseHealth();

	//Function to call once Terminal Hack is complete
	void TerminalHacked();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeapon> WeaponReference;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AWeapon* Weapon;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
