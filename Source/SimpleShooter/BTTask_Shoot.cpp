// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "ShooterCharacter.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ShooterCharacter && PlayerCharacter)
	{
		if (PlayerCharacter->IsAlive())
		{
			ShooterCharacter->FireWeapon();
			return EBTNodeResult::Succeeded;
		}
		
	}

	return EBTNodeResult::Failed;
}
