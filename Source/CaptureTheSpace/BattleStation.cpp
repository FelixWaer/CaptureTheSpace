// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleStation.h"

// Sets default values

// Viktig!! Hvis bluePrint slettes så må du legge til tag Station i blueprintet og ticke av enemy når blueprint er ut i verden!!!! 
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
	SwapColor();
}

// Called every frame
void ABattleStation::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
}

void ABattleStation::TakeDamage(int Damage)
{
	// tar skade, dette skjer i bullet.cpp
	UE_LOG(LogTemp, Warning, TEXT("FouckYou"));
	Health -= Damage;
	if (Health <= 0)
	{
		OnDefeat();
	}
}

void ABattleStation::OnDefeat()
{
	//når battlestation blir defeated så bytter den lag og farge
	if (IsEnemy == true)
	{
	IsEnemy = false;	
	}
	else
	{
		IsEnemy = true;
	}
	SwapColor();
	Health = MaxHealth;
}

void ABattleStation::SwapColor()
{
	if (IsEnemy == true)
	{
		Mesh->SetMaterial(1, EnemyMaterial);
		
		
	}
	else
	{
		Mesh->SetMaterial(1, FriendlyMaterial);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Friendly")));
	}
}