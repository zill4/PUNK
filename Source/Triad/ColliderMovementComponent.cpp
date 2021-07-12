// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Call to parent function
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// All of the above are conditions for which we should not update, do nothing.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	// Consumes values then clears it out - Clamping down to mag of 1.
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

	// Check if its not nearly zero, if it is nearly zero then we do nothing.
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		// Takes the desiredmovement, grabs magnitutde + rotation, and a hit just incase we hit something
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bump into something, then we slide along it.
		if (Hit.IsValidBlockingHit())
		{
			// Takes the movment, the time between the hits, and the normals of the hit, as well as the hit obj.
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}

