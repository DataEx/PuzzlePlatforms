// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) 
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuClassBP (TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuClassBP.Class != nullptr)) {return; }

	MainMenuClass = MainMenuClassBP.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuClassBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuClassBP.Class != nullptr)) { return; }

	InGameMenuClass = InGameMenuClassBP.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!ensure(Subsystem != nullptr)) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());

	SessionInterface = Subsystem->GetSessionInterface();
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,
		&UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,
		&UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
		&UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,
		&UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (!ensure(MainMenuClass != nullptr)) { return; }
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (!ensure(MainMenu != nullptr)) { return; }

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) { return; }
	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

	if (!ensure(InGameMenu != nullptr)) { return; }

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	this->DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr) 
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else 
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success) { 
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session"));
		return;
	}

	if (MainMenu != nullptr){
		MainMenu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) { return; }
	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr) { return; }
	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) 
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && MainMenu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Search complete"));

		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		TArray<FServerData> ServerNames;
		for (size_t i = 0; i < SearchResults.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *SearchResults[i].GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResults[i].Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResults[i].Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResults[i].Session.OwningUserName;
			Data.Name = SearchResults[i].GetSessionIdStr();

			FString ServerName;
			if (SearchResults[i].Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get server name at index: %d"), i);
			}

			ServerNames.Add(Data);
		}

		MainMenu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) { return; }

	UEngine* Engine = GetEngine();
	if (Engine == nullptr) { return; }

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	FString ConnectAddress;
	bool Success = SessionInterface->GetResolvedConnectString(SessionName, ConnectAddress);

	if (Success) 
	{
		PlayerController->ClientTravel(ConnectAddress, ETravelType::TRAVEL_Absolute);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get Connect String"));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) 
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;

		if (DesiredServerName != "")
		{
			SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		}

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (!SessionInterface.IsValid()) { return; }
	SessionInterface->StartSession(SESSION_NAME);
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid()) { return; }
	if (!SessionSearch.IsValid()) { return; }

	if (MainMenu != nullptr) {
		MainMenu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::LeaveServer()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid() && SessionInterface.IsValid())
	{
//		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("searching for sessions"));
	}
}
