// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacterOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TRIADGAME_API UMainCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercentage(float HealthPercentage);
	void SetStaminaPercentage(float StaminaPercentage);
	void SetGoldCount(int32 GoldCount);
	void SetSoulCount(int32 SoulCount);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulCountText;
};
