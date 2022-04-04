// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehavior)
	{
		
		if (RunBehaviorTree(AIBehavior))
		{
			AIBlackBoard = GetBlackboardComponent();
		}
	}
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetPawn() != nullptr && !bInitialLocationSet)
	{
		SetInitialLocation();
	}

	/*if (PlayerPawn)
	{
		if (LineOfSightTo(PlayerPawn))
		{
			AIBlackBoard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
			AIBlackBoard->SetValueAsVector(TEXT("LastPlayerLocation"), PlayerPawn->GetActorLocation());
		}
		else
		{
			AIBlackBoard->ClearValue(TEXT("PlayerLocation"));
		}
	}*/
}

bool AShooterAIController::IsAlive()
{
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ControlledCharacter)
	{
		return ControlledCharacter->IsAlive();
	}

	return false;
}

void AShooterAIController::SetInitialLocation()
{
	SetInitialLocation(GetPawn()->GetActorLocation(), false);
}

void AShooterAIController::SetInitialLocation(FVector Position, bool bVal)
{
	AIBlackBoard->SetValueAsVector(TEXT("StartPosition"), Position);
	AIBlackBoard->SetValueAsBool(TEXT("OverrideStartPos"), bVal);
	bInitialLocationSet = true;
}