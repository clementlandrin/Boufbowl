// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "AIController.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimBlueprint.h"
#include "BoufbowlCell.h"

ABoufbowlPlayer* ABoufbowlPlayer::SpawnPlayer(FString mesh_path, ABoufbowlCell* cell, ABoufbowlPlayerController* owner_controller)
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::SpawnPlayer"));
	
	if (!cell->GetBoufbowlPlayer())
	{
		USkeletalMesh* skeletal_mesh = LoadObject<USkeletalMesh>(NULL, *mesh_path, NULL, LOAD_None, NULL);

		FVector location = cell->GetLocation();
		if (skeletal_mesh)
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::SpawnPlayer found skeletal mesh"))
				FVector BoundsBoxExtent = skeletal_mesh->GetBounds().BoxExtent;
			location.Z += BoundsBoxExtent.Z / 2 + 10.0f;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::SpawnPlayer no skeletal mesh"));
		}

		FTransform actual_spawn_transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), location);

		FActorSpawnParameters spawn_parameters;
		spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABoufbowlPlayer* spawned_player = cell->GetWorld()->SpawnActor<ABoufbowlPlayer>(
			ABoufbowlPlayer::StaticClass(), actual_spawn_transform, spawn_parameters);

		if (spawned_player)
		{
			spawned_player->SetOwnerController(owner_controller);
			cell->SetBoufbowlPlayer(spawned_player);
		}
		return spawned_player;
	}
	else
	{
		return nullptr;
	}
}

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

	m_MeshComponent = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	//= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	m_MeshComponent->TranslucencySortPriority = -1;
	m_MeshComponent->SetupAttachment(RootComponent);

	if (skeletal_mesh.Succeeded())
	{
		m_MeshComponent->SetSkeletalMesh(skeletal_mesh.Object);
		FVector box = m_MeshComponent->CalcBounds(m_MeshComponent->GetComponentTransform()).BoxExtent;
		m_MeshComponent->AddRelativeLocation(FVector(box.X / 2.0f, 0.0f, - box.Z / 2.0f));
		m_MeshComponent->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> anim(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	if (anim.Succeeded())
	{
		m_MeshComponent->SetAnimClass(anim.Object->GeneratedClass);
	}

	AIControllerClass = AAIController::StaticClass();

	USkeleton* mesh_skeleton = CreateDefaultSubobject<USkeleton>(TEXT("meshskeleton"));

	static ConstructorHelpers::FObjectFinder<USkeleton> skeleton_obj(TEXT("Skeleton'/Game/Mannequin/Character/Mesh/UE4_Mannequin_Skeleton.UE4_Mannequin_Skeleton'"));

	if (skeleton_obj.Succeeded())
	{
		mesh_skeleton = skeleton_obj.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayer::ABoufbowlPlayer failed to load skeleton"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> run_anim_obj(TEXT("AnimSequence'/Game/Mannequin/Animations/ThirdPersonRun.ThirdPersonRun'"));
	if (run_anim_obj.Succeeded())
	{
		m_RunSequence = run_anim_obj.Object;
		m_RunSequence->SetSkeleton(mesh_skeleton);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayer::ABoufbowlPlayer failed to load run animation"));
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> damage_anim_obj(TEXT("AnimSequence'/Game/Mannequin/Animations/NewAnimation.NewAnimation'"));
	if (damage_anim_obj.Succeeded())
	{
		m_DamageSequence = damage_anim_obj.Object;
		m_DamageSequence->SetSkeleton(mesh_skeleton);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayer::ABoufbowlPlayer failed to load damage animation"));
	}
}

void ABoufbowlPlayer::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
	m_AIController = Cast<AAIController>(GetController());
	m_AIController->Possess(this);
}

void ABoufbowlPlayer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

AAIController* ABoufbowlPlayer::GetAIController()
{
	return m_AIController;
}

ABoufbowlPlayerController* ABoufbowlPlayer::GetOwnerController()
{
	return m_OwnerController;
}

void ABoufbowlPlayer::SetOwnerController(ABoufbowlPlayerController* owner_controller)
{
	m_OwnerController = owner_controller;
}

void ABoufbowlPlayer::TakeDamage(uint8 damage_amount)
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::TakeDamage"));

	m_Health -= damage_amount;
	
	PlayDamageAnimation();
}

void ABoufbowlPlayer::PlayRunAnimation()
{
	GetMesh()->SetAnimation(m_RunSequence);

	if (m_RunSequence->IsValidToPlay())
	{
		GetMesh()->Play(true);
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::PlayRunAnimation playing running animation"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayer::PlayRunAnimation run sequence is not valid"));
	}
}

void ABoufbowlPlayer::PlayDamageAnimation()
{
	GetMesh()->SetAnimation(m_DamageSequence);

	if (m_DamageSequence->IsValidToPlay())
	{
		GetMesh()->Play(true);
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayer::PlayDamageAnimation playing damage animation"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayer::PlayDamageAnimation damage sequence is not valid"));
	}
}

uint8 ABoufbowlPlayer::GetHealth()
{
	return m_Health;
}
