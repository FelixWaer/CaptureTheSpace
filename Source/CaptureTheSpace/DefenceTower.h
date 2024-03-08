// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefenceTower.generated.h"

UCLASS()
class CAPTURETHESPACE_API ADefenceTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenceTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	UStaticMeshComponent* DefenceTowerMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	class USphereComponent* SphereCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	TSubclassOf<class ABullet> BulletClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	float ReloadTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	float BulletSpeed;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ReloadDefenceTower();
private:
	bool ReadyToFire;
	
	FTimerHandle TimeHandler;
};
