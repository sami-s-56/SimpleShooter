// Fill out your copyright notice in the Description page of Project Settings.


#include "CampaignGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/World.h"
#include "ShooterCharacter.h"
#include "ShooterAIController.h"
#include "ShooterController.h"
#include "TimerManager.h"

ACampaignGameModeBase::ACampaignGameModeBase()
{

}

void ACampaignGameModeBase::BeginPlay()
{
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	SpawnPlayer();
}

void ACampaignGameModeBase::FetchPlayerSpawnPoint()
{
	StartPos = FindPlayerStart(PlayerController, "CampaignStart");
	UE_LOG(LogTemp, Warning, TEXT("Found the needed player start"));
	
}

void ACampaignGameModeBase::SpawnPlayer()
{
	//Setup Player Start Pos
	FetchPlayerSpawnPoint();

	//SpawnDefaultPlayer at location
	FActorSpawnParameters params;
	APawn* PlayerPawn = GetWorld()->SpawnActor<AShooterCharacter>(Player, StartPos->GetActorLocation(), StartPos->GetActorRotation(), params);
	if (PlayerController)
	{
		PlayerController->Possess(PlayerPawn);
	}

	//SetGameState
	SetCurrentState(EGameState::GS_ProceedToTerminal);
}

void ACampaignGameModeBase::OnHackBegin()
{
	if (CurrentState != EGameState::GS_HackTerminal)
	{
		return;
	}

	SetCurrentState(EGameState::GS_WaitForHack);

	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &ACampaignGameModeBase::SpawnEnemies, FMath::RandRange(2.f, 4.f), true, 1.f);
}

void ACampaignGameModeBase::SpawnEnemies()
{
	if (EnemyToSpawn != NULL)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), TEXT("EnemySpawnPoints"), EnemyPos);

		if (EnemyPos[0] != nullptr) 
		{
			FActorSpawnParameters params;
			APawn* EnemyPawn = GetWorld()->SpawnActor<AShooterCharacter>(EnemyToSpawn, EnemyPos[0]->GetActorLocation(), EnemyPos[0]->GetActorRotation(), params);

			AShooterAIController* EnemyController = Cast<AShooterAIController>(EnemyPawn->GetController());
			if (EnemyController)
			{
				UE_LOG(LogTemp, Warning, TEXT("Valid Enemy Controller Found!"));
				EnemyController->SetInitialLocation(TerminalPosition, true);
			}
		}	
	}
}

void ACampaignGameModeBase::OnHackComplete()
{
	SetCurrentState(EGameState::GS_ProceedToExit);

	GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);

	SpawnSecondWave();
}

void ACampaignGameModeBase::SpawnSecondWave()
{
	if (EnemyToSpawn != NULL)
	{
		EnemyPos.Empty();
		UGameplayStatics::GetAllActorsOfClassWithTag(this, APlayerStart::StaticClass(), TEXT("SecondWave"), EnemyPos);

		if (EnemyPos[0] != nullptr)
		{
			FActorSpawnParameters params;

			for (AActor* a : EnemyPos)
			{
				APawn* EnemyPawn = GetWorld()->SpawnActor<AShooterCharacter>(EnemyToSpawn, a->GetActorLocation(), EnemyPos[0]->GetActorRotation(), params);
			}
		}
	}
}

void ACampaignGameModeBase::HandleTriggerEvents(AActor* TriggerActor)
{
	if (TriggerActor->GetFName().ToString() == "TerminalTrigger")
	{
		if (CurrentState == EGameState::GS_ProceedToTerminal)
		{
			TerminalPosition = TriggerActor->GetActorLocation();
			SetCurrentState(EGameState::GS_HackTerminal);
		}
	}
}

void ACampaignGameModeBase::SetCurrentState(EGameState _GameState) 
{
	CurrentState = _GameState;
	
}

void ACampaignGameModeBase::PawnKilled(APawn* KilledPawn)
{
	AShooterController* ShooterController = Cast<AShooterController>(KilledPawn->GetController());

	if (ShooterController)
	{
		ShooterController->GameHasEnded(nullptr, false);
		return;
	}
}
