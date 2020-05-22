// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoufbowlPlayer.generated.h"

class ABoufbowlCamera;

UCLASS(Blueprintable)
class ABoufbowlPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABoufbowlPlayer(const FObjectInitializer& ObjectInitializer);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

private:
	USkeletalMeshComponent* m_MeshComponent;
};

