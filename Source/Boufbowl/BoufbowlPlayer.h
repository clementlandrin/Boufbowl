// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoufbowlPlayer.generated.h"

class ABoufbowlCamera;
class AAIController;
class UAnimSequence;
class UAnimBlueprint;

UCLASS(Blueprintable)
class ABoufbowlPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABoufbowlPlayer(const FObjectInitializer& ObjectInitializer);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void BeginPlay();
	AAIController* GetAIController();

	void PlayRunAnimation();
private:
	USkeletalMeshComponent* m_MeshComponent;
	AAIController* m_AIController;
	UAnimSequence* m_RunSequence;
};

