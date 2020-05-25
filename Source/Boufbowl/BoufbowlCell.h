// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoufbowlCell.generated.h"

class ABoufbowlPlayer;

UCLASS()
class ABoufbowlCell : public AActor
{
	GENERATED_BODY()

public:
	ABoufbowlCell();

	void Initialize(FIntVector id, FVector2D size, FVector location);

	FVector GetLocation();

	ABoufbowlPlayer* GetBoufbowlPlayer();
	void SetBoufbowlPlayer(ABoufbowlPlayer* boufbowl_player);
private:
	ABoufbowlPlayer* m_BoufbowlPlayer;

	void SetLocation(FVector location);
	void SetId(FIntVector id);

	UStaticMeshComponent* m_StaticMeshComponent;

	FIntVector m_Id;
	FVector m_Location;
};
