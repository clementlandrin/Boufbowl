// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlGrid.h"
#include "BoufbowlCell.h"
#include "Engine/World.h"

ABoufbowlGrid::ABoufbowlGrid()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlGrid::ABoufbowlGrid"));
}

void ABoufbowlGrid::Initialize(uint8 length, uint8 width, FVector2D cell_size)
{
	SetDimensions(length, width);
	SetCellSize(cell_size);
	CreateCells();
}

void ABoufbowlGrid::CreateCells()
{
	for (uint8 i = 0; i < m_Length; i++)
	{
		for (uint8 j = 0; j < m_Width; j++)
		{
			if (GetWorld())
			{
				ABoufbowlCell* boufbowl_cell = GetWorld()->SpawnActor<ABoufbowlCell>(100.0f * FVector(m_CellSize.X * (float(i) - m_Width / 2.0f + 0.5f), m_CellSize.Y * (float(j) - m_Length / 2.0f + 0.5f), 0.0f), FRotator::ZeroRotator);
				if (boufbowl_cell)
				{
					FIntVector id = FIntVector(i, j, 0);
					boufbowl_cell->Initialize(id, m_CellSize);

					m_Cells.Add(boufbowl_cell);
				}
			}
		}
	}
}

void ABoufbowlGrid::SetDimensions(uint8 length, uint8 width)
{
	m_Length = length;
	m_Width = width;
}

void ABoufbowlGrid::SetCellSize(FVector2D cell_size)
{
	m_CellSize = cell_size;
}

FIntVector ABoufbowlGrid::GetCellIdFromLocation(FVector hit_location)
{
	FVector2D hit_location_on_grid = FVector2D(hit_location.X, hit_location.Y);
	UE_LOG(LogTemp, Log, TEXT("Hit location : %f, %f"), hit_location.X, hit_location.Y);
	FIntVector cell_id;
	// hit location result is in cm
	cell_id.X = hit_location_on_grid.X * 0.01f / m_CellSize.X - m_Width / 2;
	cell_id.Y = hit_location_on_grid.Y * 0.01f / m_CellSize.Y - m_Length / 2;
	cell_id.Z = 0;

	return cell_id;
}