// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationWhenSeen.h"
#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationWhenSeen::UBTService_PlayerLocationWhenSeen()
{
	NodeName = TEXT("Update Player Location When Seen");
}

void UBTService_PlayerLocationWhenSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* P = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AShooterAIController* AIController = Cast<AShooterAIController>(OwnerComp.GetAIOwner());

	if (P && AIController)
	{
		if (AIController->LineOfSightTo(P))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), P);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}
}