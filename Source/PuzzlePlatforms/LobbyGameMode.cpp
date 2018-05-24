// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerCount++;
	UE_LOG(LogTemp, Warning, TEXT("Added player: %d"), PlayerCount);



	if (PlayerCount >= 2) 
	{	
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10.0f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerCount--;
	UE_LOG(LogTemp, Warning, TEXT("Removed player: %d"), PlayerCount);
}

void ALobbyGameMode::StartGame()
{
	UPuzzlePlatformsGameInstance* PuzzlePlatformsGameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (PuzzlePlatformsGameInstance == nullptr) { return; }
	PuzzlePlatformsGameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) { return; }

	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Game?listen");
}
