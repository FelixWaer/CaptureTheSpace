// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStation.h"

// Sets default values
ABattleStation::ABattleStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	
	MaxHealth = 12;
	Health = MaxHealth;
}

// Called when the game starts or when spawned
void ABattleStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleStation::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
}

void ABattleStation::TakeDamage(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("FouckYou"));
	Health -= Damage;
	if (Health <= 0)
	{
		OnDefeat();
	}
}

void ABattleStation::OnDefeat()
{
	UE_LOG(LogTemp, Warning, TEXT("BattleStation Defeated"));
	
	if (Enemy == true)
	{
	Enemy = false;	
	}
	else
	{
		Enemy = true;
	}
	SwapColor();
	Health = MaxHealth;
}

void ABattleStation::SwapColor()
{
	if (Enemy == true)
	{
		Mesh->SetMaterial(1, EnemyMaterial);
		//write text on screen that says enemy
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Enemy")));
	}
	else
	{
		Mesh->SetMaterial(1, FriendlyMaterial);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Friendly")));
	}
}