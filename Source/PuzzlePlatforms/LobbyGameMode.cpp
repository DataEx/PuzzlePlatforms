// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	PlayerCount++;
	UE_LOG(LogTemp, Warning, TEXT("Added player: %d"), PlayerCount);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	PlayerCount--;
	UE_LOG(LogTemp, Warning, TEXT("Removed player: %d"), PlayerCount);
}