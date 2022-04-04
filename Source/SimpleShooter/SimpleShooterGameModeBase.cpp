// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterGameModeBase.h"
#include "ShooterController.h"
#include "ShooterAIController.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ASimpleShooterGameModeBase::ASimpleShooterGameModeBase()
{

}

void ASimpleShooterGameModeBase::PawnKilled(APawn* KilledPawn)
{

	AShooterController* ShooterController = Cast<AShooterController>(KilledPawn->GetController());
	
	if (ShooterController)
	{
		ShooterController->GameHasEnded(nullptr, false);
		return;
	}
	else
	{
		for (AShooterAIController* AI : TActorRange<AShooterAIController>(GetWorld()))
		{
			if (AI->IsAlive())
			{
				return;
			}
		}
		GameWon();
	}

	
}

void ASimpleShooterGameModeBase::GameWon()
{
	AShooterController* ShooterController = Cast<AShooterController>(UGameplayStatics::GetPlayerController(this, 0));

	if (ShooterController) 
	{
		ShooterController->GameHasEnded(ShooterController->GetPawn(), true);
	}
}


void ASimpleShooterGameModeBase::BeginPlay()
{
	CreatedWidget = CreateWidget(GetWorld(), MainMenu);

	if (CreatedWidget)
	{
		CreatedWidget->AddToViewport();
	}
}

void ASimpleShooterGameModeBase::ChangeGameMode(EGameMode SelectedGameMode)
{
	switch (SelectedGameMode)
	{
	case EGameMode::GM_Campaign:
		UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/Map1"), true, TEXT("?game=/Game/Blueprints/BP_CampaignGameMode.BP_CampaignGameMode_C"));
		break;
	case EGameMode::GM_Survival:
		UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/Map1"), true, TEXT("?game=/Game/Blueprints/BP_SurvivalGameMode.BP_SurvivalGameMode_C"));
		break;
	default:
		break;
	}
	
}


