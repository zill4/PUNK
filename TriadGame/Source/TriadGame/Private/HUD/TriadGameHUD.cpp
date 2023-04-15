// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/TriadGameHUD.h"
#include "HUD/MainCharacterOverlay.h"

void ATriadGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && MainCharacterOverlayClass)
		{
			MainCharacterOverlay = CreateWidget<UMainCharacterOverlay>(Controller, MainCharacterOverlayClass);
			MainCharacterOverlay->AddToViewport();
		}
	}
}
