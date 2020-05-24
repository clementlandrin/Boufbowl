// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlGameMode.h"
#include "BoufbowlPlayerController.h"
#include "BoufbowlPlayer.h"
#include "BoufbowlCamera.h"
#include "BoufbowlGrid.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ABoufbowlGameMode::ABoufbowlGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABoufbowlPlayerController::StaticClass();

	DefaultPawnClass = ABoufbowlCamera::StaticClass();
}

void ABoufbowlGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		ABoufbowlGrid* boufbowl_grid = GetWorld()->SpawnActor<ABoufbowlGrid>(FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);
		boufbowl_grid->Initialize(10, 10, FVector2D(2.0f, 2.0f));

		if (boufbowl_grid)
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlGameMode::BeginPlay properly spawned boufbowl_grid"));

			for (FConstControllerIterator controller_itr = GetWorld()->GetControllerIterator(); controller_itr; ++controller_itr)
			{
				ABoufbowlPlayerController* boufbowl_player_controller = Cast<ABoufbowlPlayerController>(*controller_itr);
				if (boufbowl_player_controller)
				{
					UE_LOG(LogTemp, Warning, TEXT("ABoufbowlGameMode::BeginPlay found player controller"));

					boufbowl_player_controller->SetGrid(boufbowl_grid);
				}
			}
		}
	}
}