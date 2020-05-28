#include "CellUI.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BoufbowlCell.h"
#include "BoufbowlPlayer.h"
#include "BoufbowlPlayerController.h"

UCellUI::UCellUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCellUI::SetCell(ABoufbowlCell* cell)
{
	m_BoufbowlCell = cell;
}

void UCellUI::OnClickSpawnButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickSpawnButton"));

	if (!m_BoufbowlCell->GetBoufbowlPlayer())
	{
		ABoufbowlPlayerController* local_player_controller = Cast<ABoufbowlPlayerController>(GetWorld()->GetFirstPlayerController());
		ABoufbowlPlayer::SpawnPlayer(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"), m_BoufbowlCell, local_player_controller);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UCellUI::OnClickSpawnButton won't spawn, cell already occupied"));
	}
}

void UCellUI::OnClickMoveButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickMoveButton"));

	ABoufbowlPlayerController* local_player_controller = Cast<ABoufbowlPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!local_player_controller)
	{
		UE_LOG(LogTemp, Error, TEXT("UCellUI::OnClickMoveButton no local player controller"));
		return;
	}

	local_player_controller->MoveSavedPlayerToSelectedCell();
}

void UCellUI::OnClickAttackButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickAttackButton"));

	ABoufbowlPlayerController* local_player_controller = Cast<ABoufbowlPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!local_player_controller)
	{
		UE_LOG(LogTemp, Error, TEXT("UCellUI::OnClickMoveButton no local player controller"));
		return;
	}

	local_player_controller->AttackEnnemyWithSavedPlayer();
}

void UCellUI::OnClickBallButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickBallButton"));
}

void UCellUI::OnClickInfoButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickInfoButton"));

	if (m_InfoPanel->Visibility == ESlateVisibility::Visible)
	{
		UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickInfoButton hide info panel"));
		m_InfoPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickInfoButton display info panel"));
		m_InfoPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCellUI::NativeConstruct()
{
	// Do some custom setup

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

	if (!m_SpawnButton->OnClicked.IsBound())
		m_SpawnButton->OnClicked.AddDynamic(this, &UCellUI::OnClickSpawnButton);
	if (!m_MoveButton->OnClicked.IsBound())
		m_MoveButton->OnClicked.AddDynamic(this, &UCellUI::OnClickMoveButton);
	if (!m_AttackButton->OnClicked.IsBound())
		m_AttackButton->OnClicked.AddDynamic(this, &UCellUI::OnClickAttackButton);
	if (!m_BallButton->OnClicked.IsBound())
		m_BallButton->OnClicked.AddDynamic(this, &UCellUI::OnClickBallButton);
	if (!m_InfoButton->OnClicked.IsBound())
		m_InfoButton->OnClicked.AddDynamic(this, &UCellUI::OnClickInfoButton);
}


void UCellUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here
}