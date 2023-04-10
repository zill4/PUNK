// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Weapon.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack(const FInputActionValue& Value)
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::DirectionalHitReaction(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Blue)
	PlayHitReactMontage(FName("HitLight"));

	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward| |ToHit| * cos(Theta)
	// |Forward| = 1, |ToHit| = 1, So Forward * ToHit = cos(Theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(Theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// convert from radians to degrees
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	// If CrossProduct points down, Theta should be negative
	if (CrossProduct.Z > 0)
	{
		Theta *= -1.f;
	}

	if (Theta >= -45.f && Theta < 45.f)
	{
		AddActorWorldOffset(FVector(-10.f, 0.f, 0.f), true);
	}
	else if (Theta >= -45.f && Theta < 45.f)
	{
		AddActorWorldOffset(FVector(0.f, -10.f, 0.f), true);
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		AddActorWorldOffset(FVector(0.f, 10.f, 0.f), true);
	}
	else {
		AddActorWorldOffset(FVector(10.f, 0.f, 0.f), true);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{

	if (EquippedWeapon && EquippedWeapon->GetWeaponCollision())
	{
		EquippedWeapon->GetWeaponCollision()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

