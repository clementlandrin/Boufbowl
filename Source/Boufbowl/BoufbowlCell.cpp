// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlCell.h"

UBoufbowlCell::UBoufbowlCell()
{
	UE_LOG(LogTemp, Log, TEXT("UBoufbowlCell::BoufbowlCell"));
}

void UBoufbowlCell::SetId(FIntVector id)
{
	UE_LOG(LogTemp, Log, TEXT("BoufbowlCell::SetId"));

	m_Id.X = id.X;
	m_Id.Y = id.Y;
	m_Id.Z = 0;
}