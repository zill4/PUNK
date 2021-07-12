// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Main.h"
#include "Enemy.h"

/**
 * 
 */

class TRIAD_API Combat
{
public:
	Combat();
	~Combat();

	void InitiateCombat(AEnemy* Enemy, AMain* Main);
};
