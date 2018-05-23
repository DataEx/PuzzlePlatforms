// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MenuSystem/MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UServerRow::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (!ensure(RowButton != nullptr)) { return false; }

	RowButton->OnClicked.AddDynamic(this, &UServerRow::SelectIndex);

	return true;
}

void UServerRow::Setup(UMainMenu* Parent, uint32 Index, FServerData Data)
{
	this->Parent = Parent;
	this->Index = Index;

	ServerName->SetText(FText::FromString(Data.Name));
	HostUser->SetText(FText::FromString(Data.HostUserName));
	FString CapacityText = FString::Printf(TEXT("%d/%d"), Data.CurrentPlayers, Data.MaxPlayers);
	ServerCapacity->SetText(FText::FromString(CapacityText));
}

void UServerRow::SelectIndex()
{
	if (!ensure(Parent != nullptr)) { return; }

	Parent->SelectRowIndex(Index);
}
