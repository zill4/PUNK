// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"


// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());

	// No two default subjobject calls can use the same string.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(-300.f, 0.f, 300.f));
	Camera->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	CurrentVelocity = FVector(0.f);
	MaxSpeed = 100.f;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveY"), this, &ACritter::MoveY);
	PlayerInputComponent->BindAxis(TEXT("MoveX"), this, &ACritter::MoveX);

}

// Defines the movement of the critter along the Y-Axis i.e. Moving backwards and forwards
void ACritter::MoveY(float value)
{
	// Clamp takes a min, and max, and insures it will be within the two values.
	CurrentVelocity.Y = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed;
}

// Defines the movement of the critter along the X-Axis i.e. left and right
void ACritter::MoveX(float value)
{
	CurrentVelocity.X = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed;
}


