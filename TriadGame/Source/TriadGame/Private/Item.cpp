// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "DrawDebugHelpers.h"

#define THIRTY 30
#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 24, FColor::Red, true);

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Hello World Called!!!!!!!!!!!!!!!!!!")); // Because this is a macro the semicolon is actually optional.

	UWorld* World = GetWorld();

	if (World)
	{
		FVector Location = GetActorLocation();
		DRAW_SPHERE(Location)
	}

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		FString Name = GetName();
		FString Message = FString::Printf(TEXT("Item Name: %s, %f"), *Name, DeltaTime);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);
	}
}

