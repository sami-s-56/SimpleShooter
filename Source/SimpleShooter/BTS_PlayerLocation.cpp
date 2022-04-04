// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

UBTS_PlayerLocation::UBTS_PlayerLocation()
{
	NodeName = TEXT("Get Player Location");
}

void UBTS_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* P = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (P)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), P->GetActorLocation());
	}
}
