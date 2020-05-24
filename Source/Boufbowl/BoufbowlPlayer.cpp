// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

ABoufbowlPlayer::ABoufbowlPlayer(const FObjectInitializer& ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal_mesh(TEXT("Skeletal'/Game/Mannequin/Character/Mesh/SK_Mannequin'"));

	m_MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_MeshComponent->TranslucencySortPriority = -1;
	m_MeshComponent->SetupAttachment(RootComponent);

	if (skeletal_mesh.Object)
	{
		//m_MeshComponent->SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMesh>(this, TEXT("Skeletal Mesh")); // text("") can be just about anything.
		m_MeshComponent->SetSkeletalMesh(skeletal_mesh.Object);
		FVector box = m_MeshComponent->CalcBounds(m_MeshComponent->GetComponentTransform()).BoxExtent;
		m_MeshComponent->AddRelativeLocation(FVector(0.0f, 0.0f, - box.Z / 2.0f));
	}

}

void ABoufbowlPlayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}
