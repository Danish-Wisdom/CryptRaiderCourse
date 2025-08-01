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
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
			Component->SetSimulatePhysics(false);
			if (Movers.Num() != 0)
			{
				for (auto SingleMover : Movers)
				{
					SingleMover->SetShouldMove(true);
					
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Acceptable Actor found: UNLOCKING"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No acceptable actor found: RELOCKING"));
		if (Movers.Num() != 0)
		{
			for (auto SingleMover : Movers)
			{
				SingleMover->SetShouldMove(false);
					
			}
		}	
	}

}

AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		bool UnlockTag = OverlappingActor->ActorHasTag(AcceptableTagName);
		bool GrabTag = OverlappingActor->ActorHasTag("Grabbed");
		
		if (UnlockTag && !GrabTag)
		{
			return  OverlappingActor;
		}
	}
	
	return nullptr;
}

void UTriggerComponent::SetMoverLink(UMover* MoverComponent)
{
	Movers.Add(MoverComponent);
}

