// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */

USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

class UButton;
class UWidgetSwitcher;
class UScrollBox;
class UEditableTextBox;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectRowIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void ExitGame();

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* HostMenuCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuCancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinMenuJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerName;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerList;

	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedRowIndex;

	void UpdateChildren();
};
