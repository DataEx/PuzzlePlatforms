// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true), Category = "Setup")
	FVector TargetPosition;
	
	UPROPERTY(EditAnywhere, Category = "Setup")
	float Velocity;

private:
	float GetDistance(FVector A, FVector B);

	FVector StartingPosition;
	FVector AdditiveVector;
	float TravelDistance;
};
