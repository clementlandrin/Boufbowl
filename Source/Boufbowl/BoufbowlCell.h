// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoufbowlCell.generated.h"

UCLASS()
class ABoufbowlCell : public AActor
{
	GENERATED_BODY()

public:
	ABoufbowlCell();

	void Initialize(FIntVector id, FVector2D size);

private:
	void SetId(FIntVector id);

	UStaticMeshComponent* m_StaticMeshComponent;

	FIntVector m_Id;
	FVector m_Location;
};
