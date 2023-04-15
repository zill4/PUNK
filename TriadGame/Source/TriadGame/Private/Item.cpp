// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "TriadGame/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ItemParticles"));
	ItemParticles->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// too early to place this is constructor
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

float AItem::TransformSin()
{
	return Amplitude * FMath::Sin(RunningTime * Frequency);
}

float AItem::TransformCos()
{
	return Amplitude * FMath::Cos(RunningTime * Frequency);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInterface* PickUp = Cast<IPickupInterface>(OtherActor))
	{
		PickUp->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IPickupInterface* PickUp = Cast<IPickupInterface>(OtherActor))
	{
		PickUp->SetOverlappingItem(nullptr);
	}
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RunningTime += DeltaTime;

	if (ItemState == EItemState::Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformSin()) * DeltaTime);
	}
}

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}