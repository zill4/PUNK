// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Characters/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetRootComponent());
	// Enable Collision
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Set Collision Response to trigger on all channels
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	// Set Collision Response to ignore on Pawn channel
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Collision Trace
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	}	
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	ItemState = EItemState::Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	if (ItemParticles)
	{
		ItemParticles->Deactivate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) return;

	FHitResult HitResult;
	BoxTrace(HitResult);

	if (HitResult.GetActor() && GetOwner() != HitResult.GetActor())
	{
		if (ActorIsSameType(OtherActor)) return;

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(HitResult);
		CreateFields(HitResult.ImpactPoint);
	}
}

bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return (GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy")));
}

void AWeapon::ExecuteGetHit(FHitResult& HitResult)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner());
	}
}

void AWeapon::BoxTrace(FHitResult& HitResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);
}
