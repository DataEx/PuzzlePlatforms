// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Engine/World.h"


AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Allows changes to be pushed from server to client
	if (HasAuthority()) {
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	StartingPosition = GetActorLocation();
	TargetPosition = GetTransform().TransformPosition(TargetPosition);

	AdditiveVector = (TargetPosition - StartingPosition);
	AdditiveVector = AdditiveVector.GetSafeNormal();

	TravelDistance = GetDistance(StartingPosition, TargetPosition);
}


void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers <= 0) { return; }

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += AdditiveVector * Velocity * DeltaTime;

	// Determine if past TargetPosition
	float Distance = GetDistance(StartingPosition, CurrentLocation);
	if (Distance >= TravelDistance) {
		CurrentLocation = TargetPosition;

		FVector Temp = StartingPosition;
		StartingPosition = TargetPosition;
		TargetPosition = Temp;
		AdditiveVector *= -1;
	}

	if(HasAuthority())
		SetActorLocation(CurrentLocation);
}

void AMovingPlatform::AddActivateTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActivateTrigger()
{
	if (ActiveTriggers > 0)
		ActiveTriggers--;

}

float AMovingPlatform::GetDistance(FVector A, FVector B)
{
	FVector Vector = B - A;
	return Vector.Size();
}
