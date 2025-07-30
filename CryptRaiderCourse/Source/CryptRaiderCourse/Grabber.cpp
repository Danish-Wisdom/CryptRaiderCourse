// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include  "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
//#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Accessing Enhanced Input System through C++ and adding a mapping context through code
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext,0);

			UE_LOG(LogTemp, Warning, TEXT("Mapping Context Added"));

			bool HasMC = Subsystem->HasMappingContext(InputMappingContext);
			UE_LOG(LogTemp, Warning, TEXT("Has the Mapping Context: %s"), (HasMC ? TEXT("True") : TEXT("False")));
		}
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + (GetForwardVector() * MaxGrabDistance);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 0), false, 10.0f);

	FHitResult Hit;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		Hit,
		StartLocation, EndLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
	if (HasHit)
	{
		// FString HitName = Hit.GetActor()->GetActorLabel();
		// UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitName);

		//AActor* HitActor = Hit.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetActorLabel());
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No hit Actor"));		
	}

}



