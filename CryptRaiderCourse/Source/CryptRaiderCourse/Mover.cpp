// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover(): MoveOffset(), OriginalLocation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldMove)
	{
		MoveToOffset(DeltaTime);
	}
	else
	{
		ReturnToOGLocation(DeltaTime);
	}

}

void UMover::SetShouldMove(bool Move)
{
	ShouldMove = Move;
	UE_LOG(LogTemp, Warning, TEXT("%s"), ShouldMove ? TEXT("true") : TEXT("false"));
}

void UMover::MoveToOffset(float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = OriginalLocation + MoveOffset;
	float Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;
	
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation,TargetLocation,DeltaTime,Speed);

	GetOwner()->SetActorLocation(NewLocation);
}


void UMover::ReturnToOGLocation(float DeltaTime)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Speed = MoveOffset.Length() / MoveTime;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation,OriginalLocation,DeltaTime,Speed);

	GetOwner()->SetActorLocation(NewLocation);
}
