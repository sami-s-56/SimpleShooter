// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8 {
	PT_Health				UMETA(DisplayName = "Health"),
	PT_Ammo					UMETA(DisplayName = "Ammo"),
	PT_Collectible			UMETA(DisplayName = "Collectible"),
};


UCLASS()
class SIMPLESHOOTER_API APickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpBase();
	
	int GetPickupValue();

	EPickupType GetPickupType();

	void DestroyPickup();

private:

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* PickUpMesh;

	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* PickUpCollider;

	UPROPERTY(EditAnywhere)
		int value = 0;

	UPROPERTY(EditDefaultsOnly)
		EPickupType PickupType;

	UPROPERTY(EditInstanceOnly)
	bool bPermanantly = false;

	void ResetPickup();
};
