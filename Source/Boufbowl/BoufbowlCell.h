// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BoufbowlCell.generated.h"

UCLASS()
class UBoufbowlCell : public UObject
{
	GENERATED_BODY()

public:
	UBoufbowlCell();

	void SetId(FIntVector id);
private:
	FIntVector m_Id;
	FVector m_Location;
};
