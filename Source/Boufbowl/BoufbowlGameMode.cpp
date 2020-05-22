// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlGameMode.h"
#include "BoufbowlPlayerController.h"
#include "BoufbowlPlayer.h"
#include "BoufbowlCamera.h"
#include "UObject/ConstructorHelpers.h"

ABoufbowlGameMode::ABoufbowlGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABoufbowlPlayerController::StaticClass();

	DefaultPawnClass = ABoufbowlCamera::StaticClass();
}