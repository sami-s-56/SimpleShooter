// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	RootComponent = Root;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);

	Ammo = MagCap;
	AdditionalAmmo = 0;
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::PullTrigger()
{
	if (Ammo == 0 && AdditionalAmmo == 0)
		return;

	if (Ammo == 0 && AdditionalAmmo > 0)
	{
		//Reload
		ReloadWeapon();
	}

	Ammo--;

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn)
	{
		AController* OwnerController = Cast<AController>(OwnerPawn->GetController());
		if (OwnerController)
		{
			FVector Position;
			FRotator Rotation;
			OwnerController->GetPlayerViewPoint(Position, Rotation);

			FVector StartPoint = Position + Rotation.Vector() * 275.f;	//To insure that our character is not hit
			FVector EndPoint = Position + Rotation.Vector() * WeaponRange;

			//DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Emerald, true);

			FHitResult BulletHit;
			
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);
			Params.AddIgnoredActor(this->GetOwner());

			if (GetWorld()->LineTraceSingleByChannel(BulletHit, StartPoint, EndPoint, ECC_GameTraceChannel1, Params))
			{
				//DrawDebugPoint(GetWorld(), BulletHit.ImpactPoint, 20.f, FColor::Black, true);
				FVector ShotDirection = -Rotation.Vector();

				PlayImpactFX(BulletHit, ShotDirection);
				
				AActor* HitActor = BulletHit.GetActor();
				if (HitActor)
				{
					//Cause Damage
					FPointDamageEvent DamageEvent(Damage, BulletHit, ShotDirection, nullptr);
					HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				}
				
			}
		}
	}

	PlayFireFX();
}

void AWeapon::ReloadWeapon()
{
	if (AdditionalAmmo < MagCap)
	{
		Ammo += AdditionalAmmo;
		AdditionalAmmo = 0;
	}
	else
	{
		Ammo = MagCap;
		AdditionalAmmo -= MagCap;
	}
}

void AWeapon::AddAdditionalAmmo(int amount)
{
	AdditionalAmmo += amount;	
}

void AWeapon::PlayFireFX()
{
	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponMesh, TEXT("MuzzleFlashSocket"));
	}
	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, TEXT("MuzzleFlashSocket"));
	}
}

void AWeapon::PlayImpactFX(FHitResult& BulletHit, FVector& ShotDirection)
{
	if (ImpactEffects)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffects, BulletHit.Location, ShotDirection.Rotation());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, BulletHit.Location);
	}
}
