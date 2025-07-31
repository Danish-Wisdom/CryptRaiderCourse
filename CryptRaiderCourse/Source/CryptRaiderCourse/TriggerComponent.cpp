// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"




UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);
}



void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("TriggerComponent Working"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();
	if (Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Acceptable Actor found: UNLOCKING"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No acceptable actor found: RELOCKING"));
		if (Mover != nullptr)
		{
			Mover->SetShouldMove(true);
		}
	}

}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag(AcceptableTagName))
		{
			return  OverlappingActor;
		}
	}
	
	return nullptr;
}

void UTriggerComponent::SetMoverLink(UMover* MoverComponent)
{
	Mover = MoverComponent;
}

