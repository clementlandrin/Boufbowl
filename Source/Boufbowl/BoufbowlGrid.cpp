// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlGrid.h"
#include "Engine/SkeletalMesh.h"
#include "Components/StaticMeshComponent.h"
#include "BoufbowlCell.h"
#include "UObject/ConstructorHelpers.h"

ABoufbowlGrid::ABoufbowlGrid()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlGrid::ABoufbowlGrid"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> static_mesh(TEXT("StaticMesh'/Game/Geometry/Meshes/Plane.Plane'"));

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->TranslucencySortPriority = -1;
	m_StaticMeshComponent->SetupAttachment(RootComponent);

	if (static_mesh.Object)
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlGrid::ABoufbowlGrid static mesh found"));
		m_StaticMeshComponent->SetStaticMesh(static_mesh.Object);
	}
}

void ABoufbowlGrid::Initialize(uint8 length, uint8 width, FVector2D cell_size)
{
	SetDimensions(length, width);
	SetCellSize(cell_size);
	CreateCells();

	m_StaticMeshComponent->SetRelativeScale3D(FVector(width * cell_size.X, length * cell_size.Y, 1.0f));
}

void ABoufbowlGrid::CreateCells()
{
	for (uint8 i = 0; i < m_Length; i++)
	{
		for (uint8 j = 0; j < m_Width; j++)
		{
			UBoufbowlCell* boufbowl_cell = NewObject<UBoufbowlCell>();
			FIntVector id = FIntVector(i, j , 0);
			boufbowl_cell->SetId(id);

			m_Cells.Add(boufbowl_cell);
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