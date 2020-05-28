// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlCell.h"

#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "BoufbowlPlayer.h"
#include "CellUI.h"
#include "Engine/World.h"
#include "BoufbowlPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/Classes/Kismet/KismetStringLibrary.h"

ABoufbowlCell::ABoufbowlCell()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::ABoufbowlCell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> static_mesh(TEXT("StaticMesh'/Game/Geometry/Meshes/Plane.Plane'"));

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	m_StaticMeshComponent->TranslucencySortPriority = -1;
	m_StaticMeshComponent->SetupAttachment(RootComponent);

	if (static_mesh.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::ABoufbowlCell static mesh found"));
		m_StaticMeshComponent->SetStaticMesh(static_mesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> cell_ui(TEXT("WidgetBlueprint'/Game/TopDownCPP/Blueprints/UI/CellUIWidget.CellUIWidget'"));
	if (cell_ui.Succeeded())
	{
		m_CellUIClass = (UClass*)cell_ui.Object->GeneratedClass;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABoufbowlCell::ABoufbowlCell didn't find cell ui blueprint"));
	}
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABoufbowlCell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Really hope this won't break in multiplayer
	ABoufbowlPlayerController* local_player_controller = Cast<ABoufbowlPlayerController>(GetWorld()->GetFirstPlayerController());
	if (m_CellUI && local_player_controller && local_player_controller->GetSelectedCell() && local_player_controller->GetSelectedCell() == this)
	{
		FVector2D screen_location;
		local_player_controller->ProjectWorldLocationToScreen(m_Location, screen_location);
		m_CellUI->SetPositionInViewport(screen_location);
	}
}

void ABoufbowlCell::CreateCellUI()
{
	m_CellUI = CreateWidget<UCellUI>(GetWorld(), m_CellUIClass);
	if (m_CellUI)
	{
		m_CellUI->AddToViewport();
		m_CellUI->SetCell(this);

		ABoufbowlPlayerController* local_player_controller = Cast<ABoufbowlPlayerController>(GetWorld()->GetFirstPlayerController());
		ABoufbowlPlayerController* ennemy_player_controller = nullptr;

		if (m_BoufbowlPlayer)
		{
			ennemy_player_controller = m_BoufbowlPlayer->GetOwnerController();
		}

		// Hide move button
		if (m_BoufbowlPlayer || (local_player_controller && !local_player_controller->GetSelectedCell()))
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::CreateCellUI hidding move button"));
			m_CellUI->m_MoveButton->SetVisibility(ESlateVisibility::Hidden);
		}

		// Hide spawn button
		if (m_BoufbowlPlayer)
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::CreateCellUI hidding spawn button"));
			m_CellUI->m_SpawnButton->SetVisibility(ESlateVisibility::Hidden);
		}

		// Hide attack button
		if (!m_BoufbowlPlayer || !local_player_controller->GetSavedCell() || (ennemy_player_controller && (ennemy_player_controller == local_player_controller)) )
		{
			UE_LOG(LogTemp, Log, TEXT("ABoufbowlCell::CreateCellUI hidding attack button"));
			m_CellUI->m_AttackButton->SetVisibility(ESlateVisibility::Hidden);
		}

		// Set info pannel
		if (m_BoufbowlPlayer)
		{
			m_CellUI->m_InfoPanel->SetText(FText::AsNumber(m_BoufbowlPlayer->GetHealth()));
		}
	}
}

void ABoufbowlCell::DestroyCellUI()
{
	if (!m_CellUI)
		return;
	m_CellUI->RemoveFromViewport();
	m_CellUI->RemoveFromParent();
	CollectGarbage(RF_NoFlags);
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