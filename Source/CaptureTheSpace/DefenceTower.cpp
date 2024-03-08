// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceTower.h"

#include "Bullet.h"
#include "Components/SphereComponent.h"

// Sets default values
ADefenceTower::ADefenceTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefenceTowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefenceTowerMesh"));
	SetRootComponent(DefenceTowerMesh);
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetRootComponent());
	SphereCollider->SetSphereRadius(3000.f);

	ReadyToFire = true;
	ReloadTime = 3.f;
	BulletSpeed = 5.f;
}

// Called when the game starts or when spawned
void ADefenceTower::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ADefenceTower::OnOverlapBegin);

	GetWorldTimerManager().SetTimer(TimeHandler, this, &ADefenceTower::ReloadDefenceTower, ReloadTime, true);
}

// Called every frame
void ADefenceTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefenceTower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy") && ReadyToFire == true)
	{
		ABullet* spawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, GetActorLocation(), GetActorRotation());
		spawnedBullet->shoot_BulletFromTower(OtherActor->GetActorLocation() - spawnedBullet->GetActorLocation());
		spawnedBullet->BulletSpeed = BulletSpeed;
		//ReadyToFire = false;
	}
}

void ADefenceTower::ReloadDefenceTower()
{
	ReadyToFire = true;
}

