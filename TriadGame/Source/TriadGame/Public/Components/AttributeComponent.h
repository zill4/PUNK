// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRIADGAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 1.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 1.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold = 0;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls = 0;

public:	
	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void AddGold(int32 NewGold) { Gold += NewGold; }
	FORCEINLINE void AddSouls(int32 NewSouls) { Souls += NewSouls; }
};
