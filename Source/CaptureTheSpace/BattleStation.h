// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleStation.generated.h"
class UStaticMeshComponent;
UCLASS()

class CAPTURETHESPACE_API ABattleStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleStation();

	//Create a state machine for the battle station with the states enemy and friendly
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	bool IsEnemy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
    		UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
			UMaterial* EnemyMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variable")
	UMaterial* FriendlyMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int MaxHealth;
	int Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	 void TakeDamage(int Damage);
	void OnDefeat();
	void SwapColor();
	

	
};
