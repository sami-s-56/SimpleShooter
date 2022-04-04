// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (!bIsWinner)
	{
		CreatedWidget = CreateWidget(this, RespawnWidgetClass);
	}
	else
	{
		CreatedWidget = CreateWidget(this, WinWidgetClass);
	}
	
	
	if (CreatedWidget)
	{
		CreatedWidget->AddToViewport();
	}

	FTimerHandle RespawnTimer;

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &APlayerController::RestartLevel, 5.f);
}

void AShooterController::RestartLevel()
{
	Super::RestartLevel();

	if (CreatedWidget)
	{
		CreatedWidget->RemoveFromViewport();
	}
}

void AShooterController::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* HUD = CreateWidget(this, HudWidgetClass);

	if (HUD)
	{
		HUD->AddToViewport();
	}
}
