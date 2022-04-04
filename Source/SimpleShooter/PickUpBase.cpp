// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

// Sets default values
APickUpBase::APickUpBase()
{
	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = PickUpMesh;

	PickUpCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	PickUpCollider->SetupAttachment(PickUpMesh);

	PickUpCollider->SetBoxExtent(FVector(50.f));
	PickUpCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickUpCollider->SetGenerateOverlapEvents(true);
}

int APickUpBase::GetPickupValue()
{
	return value;
}

EPickupType APickUpBase::GetPickupType()
{
	return PickupType;
}

void APickUpBase::DestroyPickup()
{
	if (bPermanantly) 
	{
		Destroy();
	}
	else
	{
		PickUpMesh->bHiddenInGame = true;
		PickUpCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FTimerHandle ResetPickupTimer;
		GetWorldTimerManager().SetTimer(ResetPickupTimer, this, &APickUpBase::ResetPickup, 15.f, false, 15.f);
	}
}

void APickUpBase::ResetPickup()
{
	PickUpMesh->bHiddenInGame = false;
	PickUpCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
