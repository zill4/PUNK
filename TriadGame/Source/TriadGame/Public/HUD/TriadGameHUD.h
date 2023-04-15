// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TriadGameHUD.generated.h"


class UMainCharacterOverlay;
/**
 * 
 */
UCLASS()
class TRIADGAME_API ATriadGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainCharacterOverlay> MainCharacterOverlayClass;

	UPROPERTY()
	UMainCharacterOverlay* MainCharacterOverlay;
public:
	FORCEINLINE UMainCharacterOverlay* GetMainCharacterOverlay() const { return MainCharacterOverlay; }
};
