// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuSystem/ServerRow.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
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

	if (!ensure(MainMenuHostButton != nullptr)) { return false; }
	if (!ensure(MainMenuJoinButton != nullptr)) { return false; }
	if (!ensure(JoinMenuCancelButton != nullptr)) { return false; }
	if (!ensure(JoinMenuJoinButton != nullptr)) { return false; }
	if (!ensure(ExitGameButton != nullptr)) { return false; }
	if (!ensure(HostMenuCancelButton != nullptr)) { return false; }
	if (!ensure(HostMenuJoinButton != nullptr)) { return false; }

	MainMenuHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu); 
	MainMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	JoinMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	JoinMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	HostMenuCancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	HostMenuJoinButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	ExitGameButton->OnClicked.AddDynamic(this, &UMainMenu::ExitGame);

	return true;
}

void UMainMenu::HostServer() {
	if (!ensure(MenuInterface != nullptr)) { return; }
	if (!ensure(ServerName != nullptr)) { return; }

	if (ServerName->Text.ToString() == "") { return; }

	MenuInterface->Host(ServerName->Text.ToString());
}

void UMainMenu::SetServerList(TArray<FServerData> ServerData)
{
	if (!ensure(ServerRowClass != nullptr)) { return; }
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	ServerList->ClearChildren();
	for(int32 i = 0; i < ServerData.Num(); i++)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) { return; }

		Row->Setup(this, i, ServerData[i]);
		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectRowIndex(uint32 Index)
{
	SelectedRowIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++) 
	{
		UWidget* Child = ServerList->GetChildAt(i);
		UServerRow* ServerRowChild = Cast<UServerRow>(Child);
		
		if (ServerRowChild != nullptr) {
			ServerRowChild->bIsSelected = (SelectedRowIndex.IsSet()) && (i == SelectedRowIndex.GetValue());
		}
	}
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

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) { return; }
	if (!ensure(HostMenu != nullptr)) { return; }

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::ExitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ConsoleCommand("Quit");
}





