// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MenuSystem/MainMenu.h"
#include "Components/Button.h"

bool UServerRow::Initialize()
{
	if (!Super::Initialize()) { return false; }

	if (!ensure(RowButton != nullptr)) { return false; }

	RowButton->OnClicked.AddDynamic(this, &UServerRow::SelectIndex);

	return true;
}

void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	this->Parent = Parent;
	this->Index = Index;
}

void UServerRow::SelectIndex()
{
	Parent->SelectRowIndex(Index);
}
