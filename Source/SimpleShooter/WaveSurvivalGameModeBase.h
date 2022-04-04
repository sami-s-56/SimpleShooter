// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "WaveSurvivalGameModeBase.generated.h"

/**
 * Spawn in Control Room 
 * Defend against no of Enemy waves
 * Include level of difficulty
 */
UCLASS()
class SIMPLESHOOTER_API AWaveSurvivalGameModeBase : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	AWaveSurvivalGameModeBase();
	
};
