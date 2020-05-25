// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlCell.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BoufbowlPlayer.h"

ABoufbowlCell::ABoufbowlCell()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::ABoufbowlCell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> static_mesh(TEXT("StaticMesh'/Game/Geometry/Meshes/Plane.Plane'"));

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->TranslucencySortPriority = -1;
	m_StaticMeshComponent->SetupAttachment(RootComponent);

	if (static_mesh.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlGrid::ABoufbowlGrid static mesh found"));
		m_StaticMeshComponent->SetStaticMesh(static_mesh.Object);
	}
}

void ABoufbowlCell::SetId(FIntVector id)
{
	UE_LOG(LogTemp, Log, TEXT("BoufbowlCell::SetId"));

	m_Id.X = id.X;
	m_Id.Y = id.Y;
	m_Id.Z = 0;
}

void ABoufbowlCell::SetLocation(FVector location)
{
	UE_LOG(LogTemp, Log, TEXT("BoufbowlCell::SetLocation"));

	m_Location = location;
}

void ABoufbowlCell::Initialize(FIntVector id, FVector2D size, FVector location)
{
	SetId(id);
	SetLocation(location);
	m_StaticMeshComponent->SetRelativeScale3D(FVector(size.X, size.Y, 1.0f));
}

FVector ABoufbowlCell::GetLocation()
{
	UE_LOG(LogTemp, Log, TEXT("BoufbowlCell::GetLocation"));

	return m_Location;
}

void ABoufbowlCell::SetBoufbowlPlayer(ABoufbowlPlayer* boufbowl_player)
{
	m_BoufbowlPlayer = boufbowl_player;
}

ABoufbowlPlayer* ABoufbowlCell::GetBoufbowlPlayer()
{
	return m_BoufbowlPlayer;
}