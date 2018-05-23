// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */

class UButton;
class UMainMenu;

UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(UMainMenu* Parent, uint32 Index, struct FServerData Data);

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected = false;

protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerCapacity;

	UPROPERTY()
	UMainMenu* Parent;

	uint32 Index;

	UFUNCTION()
	void SelectIndex();

	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;
};
