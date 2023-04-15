// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainCharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMainCharacterOverlay::SetHealthPercentage(float HealthPercentage)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercentage);
	}
}

void UMainCharacterOverlay::SetStaminaPercentage(float StaminaPercentage)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(StaminaPercentage);
	}
}

void UMainCharacterOverlay::SetGoldCount(int32 GoldCount)
{
	if (GoldCountText)
	{
		GoldCountText->SetText(FText::FromString(FString::FromInt(GoldCount)));
	}
}

void UMainCharacterOverlay::SetSoulCount(int32 SoulCount)
{
	if (SoulCountText)
	{
		SoulCountText->SetText(FText::FromString(FString::FromInt(SoulCount)));
	}
}
