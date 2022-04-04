// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "CampaignGameModeBase.h"
#include "WaveSurvivalGameModeBase.h"
#include "TimerManager.h"
#include "PickUpBase.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterHealth = MaxHealth;

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponReference);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Weapon->SetOwner(this);

	CurrentGameMode = GetWorld()->GetAuthGameMode();


}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideward"), this, &AShooterCharacter::MoveSideward);

	PlayerInputComponent->BindAxis(TEXT("LookVerticle"), this, &AShooterCharacter::LookVerticle);
	PlayerInputComponent->BindAxis(TEXT("LookHorizontal"), this, &AShooterCharacter::LookHorizontal);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShooterCharacter::FireWeapon);

	PlayerInputComponent->BindAction(TEXT("PerformAction"), IE_Pressed, this, &AShooterCharacter::PerformAction);

	PlayerInputComponent->BindAction(TEXT("UseHealth"), IE_Pressed, this, &AShooterCharacter::UseHealth);

}

#pragma region Functions Binded to Input

void AShooterCharacter::MoveForward(float AxisVal)
{
	APawn::AddMovementInput(GetActorForwardVector(), AxisVal);
}

void AShooterCharacter::MoveSideward(float AxisVal)
{
	APawn::AddMovementInput(GetActorRightVector(), AxisVal);
}

void AShooterCharacter::LookVerticle(float AxisVal)
{
	APawn::AddControllerPitchInput(AxisVal);
}

void AShooterCharacter::LookHorizontal(float AxisVal)
{
	APawn::AddControllerYawInput(AxisVal);
}

void AShooterCharacter::FireWeapon()
{
	if (Weapon)
	{
		Weapon->PullTrigger();
	}
}

void AShooterCharacter::PerformAction()
{
	if (bCanHack)
	{
		HackTerminal();
	}

	if (bNearPickUp)
	{
		CollectPickup();
	}
}

void AShooterCharacter::UseHealth()
{
	if (HealthPacks.Num() == 0 || CharacterHealth == MaxHealth)
		return;

	CharacterHealth = (CharacterHealth + HealthPacks[0]) <= MaxHealth ? CharacterHealth + HealthPacks[0] : MaxHealth;
	HealthPacks.RemoveAt(0);

}


#pragma endregion

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterHealth -= DamageApplied;

	UE_LOG(LogTemp, Display, TEXT("Health: %f"), CharacterHealth);

	if (CharacterHealth <= 0)
	{
		//Death Logic
		bIsAlive = false;

		ASimpleShooterGameModeBase* GameMode = Cast<ASimpleShooterGameModeBase>(CurrentGameMode);

		if (GameMode)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageApplied;
}

void AShooterCharacter::HackTerminal()
{
	ACampaignGameModeBase* CampaignGameMode = Cast<ACampaignGameModeBase>(CurrentGameMode);

	if (CampaignGameMode)
	{
		if (CampaignGameMode->GetCurrentGameState() != EGameState::GS_HackTerminal)
		{
			return;
		}

		FTimerHandle hackTimer;
		GetWorldTimerManager().SetTimer(hackTimer, this, &AShooterCharacter::TerminalHacked, 1.f, false, 60.f);
		//Start updating Widget blueprint (progress bar)

		CampaignGameMode->OnHackBegin();
	}
}

void AShooterCharacter::TerminalHacked()
{
	ACampaignGameModeBase* CampaignGameMode = Cast<ACampaignGameModeBase>(CurrentGameMode);

	if (CampaignGameMode)
	{
		CampaignGameMode->OnHackComplete();
	}
}

void AShooterCharacter::CollectPickup()
{
	if (OverlappingPickup == nullptr)
		return;

	switch (OverlappingPickup->GetPickupType())
	{
	case EPickupType::PT_Ammo:
		if (Weapon)
		{
			Weapon->AddAdditionalAmmo(OverlappingPickup->GetPickupValue());
		}
		break;
	case EPickupType::PT_Collectible:
		//Either update collectible counter here or in game mode
		break;
	case EPickupType::PT_Health:
		HealthPacks.Add(OverlappingPickup->GetPickupValue());
		break;
	default:
		break;
	}

	OverlappingPickup->DestroyPickup();
}

float AShooterCharacter::GetHealth() const
{
	return (CharacterHealth / MaxHealth);
}

int AShooterCharacter::GetHealthPackCount() const
{
	return HealthPacks.Num();
}

#pragma region Trigger Events

void AShooterCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ACampaignGameModeBase* CampaignGameMode = Cast<ACampaignGameModeBase>(CurrentGameMode);

	if (CampaignGameMode)
	{
		CampaignGameMode->HandleTriggerEvents(OtherActor);
	}

	FString triggerName = OtherActor->GetFName().ToString();

	if (triggerName == "TerminalTrigger")
	{
		bCanHack = true;
		return;
	}
	else
	{
		bNearPickUp = true;
		OverlappingPickup = Cast<APickUpBase>(OtherActor);
	}
}

void AShooterCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	FString triggerName = OtherActor->GetFName().ToString();

	if (triggerName == "TerminalTrigger")
	{
		bCanHack = false;
	}
	else 
	{
		bNearPickUp = false;
		OverlappingPickup = nullptr;
	}
}

#pragma endregion 
