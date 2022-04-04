// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	virtual void RestartLevel() override; 

protected:

	virtual void BeginPlay();

private:

	class UUserWidget* CreatedWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> RespawnWidgetClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> WinWidgetClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> HudWidgetClass;

};
