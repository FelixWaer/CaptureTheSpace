// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class CAPTURETHESPACE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	UStaticMeshComponent* BulletMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	class USceneComponent* AimCursor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	float BulletSpeed = 10000;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	float BulletDamage = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "MyVariables")
	float BulletLifeTime = 10;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetUpBulletTarget(class UPrimitiveComponent* TargetComponent, FVector TargetLocation);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	void MovementFunction(float DeltaTime);
	bool HasTarget = false;
	
};
