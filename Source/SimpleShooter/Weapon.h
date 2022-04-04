// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SIMPLESHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();

	void ReloadWeapon();

	void AddAdditionalAmmo(int amount);
	

private:

	UPROPERTY(EditAnywhere)
		float WeaponRange = 2000.f;

	UPROPERTY(EditAnywhere)
		float Damage = 0.f;

	//Initial Mag Capacity
	UPROPERTY(EditDefaultsOnly)
		int MagCap = 50;

	//Amount of ammo present
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int Ammo = 0;

	//Amount of ammo collected
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int AdditionalAmmo = 0;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* ImpactEffects;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* ImpactSound;

	void PlayFireFX();

	void PlayImpactFX(FHitResult& BulletHit, FVector& ShotDirection);
};
