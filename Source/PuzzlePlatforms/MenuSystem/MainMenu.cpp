// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuSystem/ServerRow.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowClassBP(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowClassBP.Class != nullptr)) { return; }

	ServerRowClass = ServerRowClassBP.Class;
}

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

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (!ensure(ServerRowClass != nullptr)) { return; }
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	ServerList->ClearChildren();
	for(int i = 0; i < ServerNames.Num(); i++)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) { return; }
		Row->ServerName->SetText(FText::FromString(ServerNames[i]));
		Row->Setup(this, i);
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectRowIndex(uint32 Index)
{
	SelectedRowIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr)) { return; }

	if (SelectedRowIndex.IsSet()) {
		MenuInterface->Join(SelectedRowIndex.GetValue());
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedRowIndex is not set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(JoinMenu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (!ensure(MenuInterface != nullptr)) { return; }
	MenuInterface->RefreshServerList();
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





