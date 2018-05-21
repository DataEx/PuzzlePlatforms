// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (!ensure(CancelButton != nullptr)) { return false; }
	if (!ensure(QuitButton != nullptr)) { return false; }

	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CloseMenu);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::LeaveServer);

	return true;
}

void UInGameMenu::CloseMenu()
{
	Teardown();
}

void UInGameMenu::LeaveServer()
{
	if (!ensure(MenuInterface != nullptr)) { return; }

	MenuInterface->LeaveServer();

}
