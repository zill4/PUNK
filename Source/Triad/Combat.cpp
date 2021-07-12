// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat.h"

Combat::Combat()
{

}

Combat::~Combat()
{

}

void Combat::InitiateCombat(AEnemy* Enemy, AMain* Main)
{

	UE_LOG(LogTemp, Error, TEXT("%s %s"), *Enemy->Name, *Main->Name);
}
