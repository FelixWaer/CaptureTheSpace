// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSpaceShip.generated.h"

class USoundCue;
struct FInputActionValue;
UCLASS()
class CAPTURETHESPACE_API APlayerSpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerSpaceShip();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	UStaticMeshComponent* SpaceShipMesh;

	//Pawn Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UFloatingPawnMovement> PawnMovement;
	
	//Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	class UCameraComponent* MyCamera;
	//SpringArm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	class USpringArmComponent* MySpringArm;
	//AimTarget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyVariables")
	class USceneComponent* AimCursor;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//----------------------Input Ref----------------------//
	// Input Mapping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* MovementInput;
	// UpDownMovement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* UpDownMovementInput;
	// CameraMovement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* CameraMovementInput;
	// CameraDistance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* CameraDistanceInout;
	// Shoot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* ShootInput;
	// Aim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* TractorInput;
	//----------------------Input Functions----------------------//
	void MovementFunction(const FInputActionValue& input);
	void UpDownMovementFunction(const FInputActionValue& input);
	void CameraMovementFunction(const FInputActionValue& input);
	void CameraDistanceFunction(const FInputActionValue& input);
	void ShootFunction(const FInputActionValue& input);
	void TractorBeam();
	void TractorBeamStarted();
	void TractorBeamReleased();
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Movement")
	float RotateSpeed = 0.5f;

	//Essential Variables
	UPROPERTY(BlueprintReadWrite)
	int Score = 0;
	UPROPERTY(BlueprintReadWrite)
	int Health = 5;

	//Upgradeable Variables
	UPROPERTY(BlueprintReadWrite)
	float BulletSpeed = 10000.f;
	UPROPERTY(BlueprintReadWrite)
	int AmountOfShots = 1;
	UPROPERTY(BlueprintReadWrite)
	int GoldIncrease = 1;
	
	
	//Camera Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CameraVar")
	float CameraDistanceMin = 300.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CameraVar")
	float CameraDistanceMax = 1500.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CameraVar")
	float CameraDistanceSpeed = 20.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShootingVar")
	float CameraLineTraceDistance = 10000;
	
	//Sounds
	UPROPERTY(EditAnywhere)
	USoundBase* ShootingSound;
	UAudioComponent* SpawnedSound;
	UPROPERTY(EditAnywhere)
	USoundBase* BeamSound;

	//What Type of bullet to shoot
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShootingVar")
	TSubclassOf<class ABullet> BulletClass;
private:

	APlayerController* PlayerController;

	//Movement Functions
	void MovementController(float DeltaTime);
	
	
	//Movement Variables
	FVector CurrentVelocity;

	//Aiming Variables
	void LineTraceFromCamera();
	FVector AimLocation;
	class UPrimitiveComponent* HitComponent;
	FHitResult HitResult;

	//Tractor Beam Variables
	FHitResult TractorResult;

	
};
