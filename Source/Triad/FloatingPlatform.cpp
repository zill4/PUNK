// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	InterpSpeed = 750.f;
	InterpTime = 10.f;
	bInterping = false;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	// StarPoint is set to the intial location of the actor.
	StartPoint = GetActorLocation();
	// End Point is set to start point.
	EndPoint += StartPoint;

	// At begin play it will wait one second and begin interping
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bInterping)
	{
		// Interpelation is a math tech for moving between two points smoothly
		FVector CurrentLocation = GetActorLocation();

		//FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		FVector Interp = FMath::VInterpConstantTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);

		//UE_LOG(LogTemp, Warning, TEXT("Interp: X: %f Y: %f Z: %f "), Interp.X, Interp.Y, Interp.Z);

		SetActorLocation(Interp);
		// Repeat via timer, stop and restart

		// Check if its time to stop interping
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.f)
		{
			// Toggle and set timer
			ToggleInterping();
			
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapStartEndLocation(&StartPoint, &EndPoint);
		}
	}
	
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapStartEndLocation(FVector* Start, FVector* End)
{
	FVector _temp = *Start;
	*Start = *End;
	*End = _temp;
}