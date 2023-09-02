// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

#include "PlayerSpaceShip.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(false);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>("Rotating Movement");
	
	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	this->OnActorBeginOverlap.AddDynamic(this, &ACollectible::OnOverlapBegin);
}

void ACollectible::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor->IsA<APlayerSpaceShip>())
	{
		APlayerSpaceShip* PlayerSpaceShip = Cast<APlayerSpaceShip>(OtherActor);
		PlayerSpaceShip->Score+=1*PlayerSpaceShip->GoldIncrease;
		UGameplayStatics::PlaySound2D(GetWorld(),PickUpSound);
		Destroy();
	}
}


