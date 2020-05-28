// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoufbowlPlayerController.generated.h"

class ABoufbowlGrid;
class ABoufbowlCell;
class ABoufbowlCamera;
class ADecalActor;
class ABoufbowlPlayer;
class UMaterial;


UCLASS()
class ABoufbowlPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABoufbowlPlayerController();

	void BeginPlay();

	void SetGrid(ABoufbowlGrid* boufbowl_grid);

	ABoufbowlCell* GetSelectedCell();
	ABoufbowlCell* GetSavedCell();

	void MoveSavedPlayerToSelectedCell();
	void AttackEnnemyWithSavedPlayer();

protected:
	ABoufbowlGrid* m_BoufbowlGrid;
	ABoufbowlCell* m_SelectedCell;

	ABoufbowlCell* m_SavedCell;

	void SaveCell(ABoufbowlCell* cell_to_save);
	void UnsaveCell();
	void SelectCell(ABoufbowlCell* cell_to_select);
	void DeselectCell();

	void InitializeDecal();

	FHitResult m_HitCursor;
	FVector m_CursorInWorldLocation;

	USkeletalMesh* m_SkeletalMesh;

	/** Get a pointer on a decal that will follow the mouse if we're targeting. */
	ADecalActor* m_DecalActor;

	/** Stands for decal material */
	UMaterial* m_DecalMaterial;
	/** Stands for selection material */
	UMaterial* m_SelectionMaterial;
	/** Stands for saved material */
	UMaterial* m_SavedMaterial;
	/** Stands for basic cell material */
	UMaterial* m_BaseCellMaterial;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void LeftMouseButtonClick();
	void RightMouseButtonClick();

	ABoufbowlPlayer* SpawnPlayer(FVector location);
};


