// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (!ensure(HostButton != nullptr)) { return false; }
	if (!ensure(JoinButton != nullptr)) { return false; }
	if (!ensure(CancelButton != nullptr)) { return false; }
	if (!ensure(JoinServerButton != nullptr)) { return false; }
	if (!ensure(ExitGameButton != nullptr)) { return false; }

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::HostServer() {
	if (!ensure(MenuInterface != nullptr)) { return; }

	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr)) { return; }
	if (!ensure(IPAddressField != nullptr)) { return; }

	FString IPAddress = IPAddressField->GetText().ToString();
	MenuInterface->Join(IPAddress);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(JoinMenu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(MainMenu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ConsoleCommand("Quit");
}



