// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleShooterGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameMode : uint8 {
	GM_Campaign       UMETA(DisplayName = "Campaign"),
	GM_Survival        UMETA(DisplayName = "Survival"),
};

UCLASS()
class SIMPLESHOOTER_API ASimpleShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASimpleShooterGameModeBase();

	virtual void PawnKilled(APawn* KilledPawn);

	virtual void GameWon();

	UFUNCTION(BlueprintCallable)
		void ChangeGameMode(EGameMode SelectedGameMode);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AShooterCharacter> EnemyToSpawn;

private:

	class UUserWidget* CreatedWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> MainMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ACampaignGameModeBase> CampaignGameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AWaveSurvivalGameModeBase> WaveSurvivalGameMode;


	

};
