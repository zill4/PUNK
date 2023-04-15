// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"



void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInterface->AddGold(this);

		SpawnPickupSound();
		Destroy();
	}
}
