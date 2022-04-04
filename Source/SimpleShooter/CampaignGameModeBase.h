// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "CampaignGameModeBase.generated.h"

/**
 * Start from somewhere (Done)
 * Proceed to Control Room 
 * Hack the terminal
 * Fend of reinforcements
 * Make way to Exit
 */

UENUM(BlueprintType)
enum class EGameState : uint8 {
	GS_Spawned					UMETA(DisplayName = "Spawned"),
	GS_ProceedToTerminal        UMETA(DisplayName = "ProceedToTerminal"),
	GS_HackTerminal				UMETA(DisplayName = "HackTheTerminal"),
	GS_WaitForHack				UMETA(DisplayName = "WaitForHackToComplete"),
	GS_ProceedToExit			UMETA(DisplayName = "ProceedToExit"),
	GS_MissionComplete			UMETA(DisplayName = "MissionComplete"),
};

UCLASS()
class SIMPLESHOOTER_API ACampaignGameModeBase : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()
	
public:

	ACampaignGameModeBase();

protected:


	virtual void BeginPlay() override;

	void FetchPlayerSpawnPoint();

private:

	//From where player should start
	AActor* StartPos;

	//Reference to Player Controller Object
	APlayerController* PlayerController;

	//Spawn positions for enemies during hack
	TArray<AActor*> EnemyPos;

	//List of initially placed enemies 
	TArray<class AShooterAIController*> PlacedEnemies;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AShooterCharacter> Player;

	//GameState
	EGameState CurrentState;

	//To make enemies run towards that position while hack is in progress
	FVector TerminalPosition;

	//Override default spawn player 
	void SpawnPlayer();

	//Timer handle to spawn enemies while hacking
	FTimerHandle SpawnEnemyTimer;

	//Spawn enemies at time of hacking
	void SpawnEnemies();

	void SpawnSecondWave();

public:

	UFUNCTION(BlueprintPure)
	EGameState GetCurrentGameState() { return CurrentState; }

	void SetCurrentState(EGameState GS);

	void HandleTriggerEvents(AActor* TriggerActor);

	void OnHackBegin();

	void OnHackComplete();

	void PawnKilled(APawn* KilledPawn) override;
};
