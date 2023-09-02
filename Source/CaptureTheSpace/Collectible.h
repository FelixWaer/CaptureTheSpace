// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectible.generated.h"

class URotatingMovementComponent;
class USphereComponent;

UCLASS()
class CAPTURETHESPACE_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> Collider;
	UPROPERTY(EditAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovement;

	//Sounds
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickUpSound;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);


};
