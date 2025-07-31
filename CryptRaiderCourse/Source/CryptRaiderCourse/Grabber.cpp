// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include  "DrawDebugHelpers.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

//#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGrabber::UGrabber(): InputMappingContext(nullptr), GrabInputAction(nullptr)
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

	SetupMappingContext();

	UInputComponent* PlayerInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	SetupPlayerInputComponent(PlayerInputComponent);

	

}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr){return;}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + (GetForwardVector() * HoldDistance);
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}                                                                                   	
																						
void UGrabber::Grab()
{
	
	GrabLogic();
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr){return;}
	
	UE_LOG(LogTemp, Warning, TEXT("Releasing Grabbed Object"));

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}
	
}

void UGrabber::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	                                
						                                                                                                            
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))           
	{                                                                                                                           
		EnhancedInputComponent->BindAction(GrabInputAction, ETriggerEvent::Started, this, &UGrabber::Grab);                   
	}                                                                                                                           
}

void UGrabber::GrabLogic() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr){return;}
	
	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + (GetForwardVector() * MaxGrabDistance);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 10.0f);
	DrawDebugSphere(GetWorld(), EndLocation, 10, 10, FColor::Blue, false, 10.0f);

	
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
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitName);

		//AActor* HitActor = Hit.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetActorLabel());

		//DrawDebugSphere(GetWorld(), Hit.Location, 10, 10, FColor::Purple, false, 10.0f);
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10, 10, FColor::Green, false, 10.0f);

		UPrimitiveComponent* HitComponent = Hit.GetComponent();
		HitComponent->WakeAllRigidBodies();
		
		// Using Physics Handle to grab objects in the world
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			Hit.ImpactPoint,
			GetComponentRotation()
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No hit Actor"));		
	}
}


void UGrabber::SetupMappingContext()const
{
	
	// Accessing Enhanced Input System through C++ and adding a mapping context through code
	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext,0);

			UE_LOG(LogTemp, Warning, TEXT("Mapping Context Added"));

			const bool HasMC = Subsystem->HasMappingContext(InputMappingContext);
			UE_LOG(LogTemp, Warning, TEXT("Has the Mapping Context: %s"), (HasMC ? TEXT("True") : TEXT("False")));
		}
	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr)
	{
		return PhysicsHandle;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber requires a UPhysicsHandleComponent"));
		return nullptr;
	}

}



