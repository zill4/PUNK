// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TriadGame\DebugMacros.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Blue)
	PlayHitReactMontage(FName("HitLight"));

	const FVector& EnemyLocation = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - EnemyLocation).GetSafeNormal();

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);

	if (CrossProduct.Z > 0)
	{
		Theta *= -1.f;
	}
	// convert to degrees
	Theta = FMath::RadiansToDegrees(Theta);


	//AddActorWorldRotation(FRotator(0.f, 0.f, 0.f), true);
	//AddActorWorldOffset(FVector(0, -10.f, 0.f), true);

	//if (Theta >= -45.f && Theta < 45.f)
	//{
	//	AddActorWorldOffset(FVector(0.f, 10.f, 0.f), true);
	//}
	//else if (Theta >= -45.f && Theta < 45.f)
	//{
	//	AddActorWorldOffset(FVector(-10.f, 0.f, 0.f), true);
	//	//AddActorWorldRotation(FRotator(0.f, 90.f, 0.f), true);
	//}
	//else if (Theta >= 45.f && Theta < 135.f)
	//{
	//	AddActorWorldOffset(FVector(10.f, 0.f, 0.f), true);
	//	//AddActorWorldRotation(FRotator(0.f, -90.f, 0.f), true);
	//}

	if (GEngine)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Theta :%f"), Theta));
}

