// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoufbowlPlayerController.generated.h"

class ABoufbowlGrid;
class ABoufbowlCamera;
class ADecalActor;
class UMaterial;

UCLASS()
class ABoufbowlPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABoufbowlPlayerController();

	void BeginPlay();

	void SetGrid(ABoufbowlGrid* boufbowl_grid);

protected:
	ABoufbowlGrid* m_BoufbowlGrid;

	void InitializeDecal();

	FHitResult m_HitCursor;
	FVector m_CursorInWorldLocation;

	USkeletalMesh* m_SkeletalMesh;

	/** Get a pointer on a decal that will follow the mouse if we're targeting. */
	ADecalActor* m_DecalActor;

	/** Stands for decal material */
	UMaterial* m_DecalMaterial;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void LeftMouseButtonClick();

	void SpawnPlayer(FVector location);

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


