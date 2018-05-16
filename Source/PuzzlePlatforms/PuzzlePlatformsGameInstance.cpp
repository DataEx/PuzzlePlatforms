// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) 
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassBP (TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuClassBP.Class != nullptr)) {return; }

	MenuClass = MenuClassBP.Class;	
}

void UPuzzlePlatformsGameInstance::Init()
{
	if (!ensure(MenuClass != nullptr)) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Reference to menu class: %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (Engine == nullptr) { return; }
	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (World == nullptr) { return; }
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (Engine == nullptr) { return; }
	FString Output = "Join: " + Address;
	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, Output);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) { return; }

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
