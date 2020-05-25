#include "CellUI.h"

#include "Blueprint/UserWidget.h"

UCellUI::UCellUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void UCellUI::NativeConstruct()
{
	// Do some custom setup

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();
}


void UCellUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here
}