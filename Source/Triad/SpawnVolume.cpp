// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Critter.h"
// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	RootComponent = SpawningBox;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	// Static function that returns a vector.
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return (Point);
}

// The '_Implementation' lets unreal know that this partially written in C++ and accessible in Blueprints
void ASpawnVolume::SpawnAPawn_Implementation(UClass* Pawn, const FVector& Location)
{
	if (Pawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		if (World)
		{
			ACritter* CritterSpawned = World->SpawnActor<ACritter>(Pawn, Location, FRotator(0.f), SpawnParams);
		}
	}
}