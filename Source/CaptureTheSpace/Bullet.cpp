// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BattleStation.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(BulletMesh);

	AimCursor = CreateDefaultSubobject<USceneComponent>(TEXT("AimCursor"));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(BulletLifeTime);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Movement
	MovementFunction(DeltaTime);

	//Destroy bullet if close to target
	if ((GetActorLocation() - AimCursor->GetComponentLocation()).Length() < 10)
	{
		SetLifeSpan(0.5f);
	}
}

void ABullet::SetUpBulletTarget(UPrimitiveComponent* TargetComponent, FVector TargetLocation)
{
	//SetupTarget for bullet to hit
	if (TargetComponent != nullptr)
	{
		AimCursor->AttachToComponent(TargetComponent, FAttachmentTransformRules::KeepWorldTransform);
		HasTarget = true;
	}
	if(!TargetLocation.IsNearlyZero())
	{
		AimCursor->SetWorldLocation(TargetLocation);
		HasTarget = true;
	}
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if hit actor has ENEMY TAG
	if(OtherActor->ActorHasTag("Enemy"))
	{
		
		Destroy();
		OtherActor->Destroy();
	}
	
//check if hit actor has STATION TAG
	if (OtherActor->ActorHasTag("Station"))
	{
		ABattleStation* Station = Cast<ABattleStation>(OtherActor);
		if(Station)
		{
			if (Station->IsEnemy == true)
			{
				Station->TakeDamage(BulletDamage);
			}
			Destroy();
		}
	}
}
	void ABullet::MovementFunction(float DeltaTime)
	{
		//Rotate To target if it has one
		if(HasTarget && !AimCursor->GetComponentLocation().IsNearlyZero())
		{
			FVector LookVector = AimCursor->GetComponentLocation() - GetActorLocation();
			LookVector.Normalize();
			SetActorRotation(LookVector.Rotation());
		}
		//Moves forward
		AddActorWorldOffset(GetActorForwardVector() * BulletSpeed * DeltaTime);
	}
