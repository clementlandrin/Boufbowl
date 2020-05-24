// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoufbowlGrid.generated.h"

class ABoufbowlCell;

UCLASS()
class ABoufbowlGrid : public AActor
{
	GENERATED_BODY()

public:
	ABoufbowlGrid();

	void Initialize(uint8 length, uint8 width, FVector2D cell_size);

	FIntVector GetCellIdFromLocation(FVector hit_location);
private:
	TArray<ABoufbowlCell*> m_Cells;
	uint8 m_Length;
	uint8 m_Width;
	FVector2D m_CellSize;

	void CreateCells();
	void SetDimensions(uint8 length, uint8 width);
	void SetCellSize(FVector2D cell_size);
};
