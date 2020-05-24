// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoufbowlGrid.generated.h"

class UBoufbowlCell;

UCLASS()
class ABoufbowlGrid : public AActor
{
	GENERATED_BODY()

public:
	ABoufbowlGrid();

	void Initialize(uint8 length, uint8 width, FVector2D cell_size);

	FIntVector GetCellIdFromLocation(FVector hit_location);
private:
	TArray<UBoufbowlCell*> m_Cells;
	uint8 m_Length;
	uint8 m_Width;
	FVector2D m_CellSize;
	UStaticMeshComponent* m_StaticMeshComponent;

	void CreateCells();
	void SetDimensions(uint8 length, uint8 width);
	void SetCellSize(FVector2D cell_size);
};
