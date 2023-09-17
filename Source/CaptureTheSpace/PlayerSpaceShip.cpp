// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSpaceShip.h"
#include "Kismet/KismetSystemLibrary.h"

//SpringArm and Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//EnhancedInput

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


#include "Bullet.h"
#include "Collectible.h"
#include "Components/AudioComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerSpaceShip::APlayerSpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpaceShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceShipMesh"));
	SetRootComponent(SpaceShipMesh);

	SpaceShipMesh->SetSimulatePhysics(true);
	SpaceShipMesh->SetEnableGravity(false);
	SpaceShipMesh->BodyInstance.bLockXRotation = true;
	SpaceShipMesh->BodyInstance.bLockYRotation = true;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement Component");
	
	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MySpringArm"));
	MySpringArm->SetupAttachment(GetRootComponent());
	MySpringArm->bUsePawnControlRotation = true;
	MySpringArm->bEnableCameraLag = true;
	MySpringArm->bEnableCameraRotationLag = true;
	MySpringArm->CameraLagSpeed = 20;
	MySpringArm->AddLocalOffset(FVector(0,0,100));
	
	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MyCamera"));
	MyCamera->SetupAttachment(MySpringArm, USpringArmComponent::SocketName);

	//Create ViewportCursor
	AimCursor = CreateDefaultSubobject<USceneComponent>(TEXT("AimCursor"));
	AimCursor->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void APlayerSpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(subsystem)
		{
			subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void APlayerSpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTraceFromCamera(); 
	MovementController(DeltaTime);
}

// Called to bind functionality to input
void APlayerSpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementInput, ETriggerEvent::Triggered, this, &APlayerSpaceShip::MovementFunction);
		EnhancedInputComponent->BindAction(UpDownMovementInput, ETriggerEvent::Triggered, this, &APlayerSpaceShip::UpDownMovementFunction);
		EnhancedInputComponent->BindAction(CameraMovementInput, ETriggerEvent::Triggered, this, &APlayerSpaceShip::CameraMovementFunction);
		EnhancedInputComponent->BindAction(CameraDistanceInout, ETriggerEvent::Triggered, this, &APlayerSpaceShip::CameraDistanceFunction);
		EnhancedInputComponent->BindAction(ShootInput, ETriggerEvent::Started, this, &APlayerSpaceShip::ShootFunction);

		//Beam Input
		EnhancedInputComponent->BindAction(TractorInput, ETriggerEvent::Triggered, this, &APlayerSpaceShip::TractorBeam);
		EnhancedInputComponent->BindAction(TractorInput, ETriggerEvent::Started, this, &APlayerSpaceShip::TractorBeamStarted);
		EnhancedInputComponent->BindAction(TractorInput, ETriggerEvent::Completed, this, &APlayerSpaceShip::TractorBeamReleased);
	}
}

void APlayerSpaceShip::MovementFunction(const FInputActionValue& input)
{
	AddMovementInput(GetActorForwardVector(),input.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(),input.Get<FVector2D>().X);
}

void APlayerSpaceShip::UpDownMovementFunction(const FInputActionValue& input)
{
	SpaceShipMesh->AddImpulse(input.Get<float>() * GetActorUpVector() * 1000000 * GetWorld()->GetDeltaSeconds());
}

void APlayerSpaceShip::CameraMovementFunction(const FInputActionValue& input)
{
	SetActorRotation(FRotator(0.f,GetControlRotation().Yaw,0.f));
	AddControllerYawInput(input.Get<FVector2D>().X);
	AddControllerPitchInput(-input.Get<FVector2D>().Y);
}

void APlayerSpaceShip::CameraDistanceFunction(const FInputActionValue& input)
{
	MySpringArm->TargetArmLength = FMath::Clamp(MySpringArm->TargetArmLength + (-input.Get<float>() * CameraDistanceSpeed), CameraDistanceMin, CameraDistanceMax);
}

void APlayerSpaceShip::ShootFunction(const FInputActionValue& input)
{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		ABullet* CurrentBullet;
		UGameplayStatics::PlaySound2D(GetWorld(),ShootingSound);
	for(int i = 0; i<AmountOfShots;i++)
	{
		CurrentBullet = GetWorld()->SpawnActor<ABullet>(
		BulletClass,
		GetActorLocation()+(GetActorForwardVector()*200*i),
		(AimLocation- GetActorLocation() + (GetActorForwardVector()*100)).GetSafeNormal().Rotation(),
		SpawnParameters);
		if (HitComponent && !AimLocation.IsNearlyZero())
		{
			CurrentBullet->BulletSpeed = BulletSpeed;   //Av en eller annen grunn crasher Unreal om jeg skyter for mange bullets -Aslak
			CurrentBullet->SetUpBulletTarget(HitComponent, AimLocation);
		}
	}


	
}

void APlayerSpaceShip::TractorBeam()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),MyCamera->GetComponentLocation(),
		MyCamera->GetComponentLocation() + MyCamera->GetForwardVector() * 10000,100,TraceTypeQuery1,
		false,ActorsToIgnore,EDrawDebugTrace::None,TractorResult,true);

	if(TractorResult.bBlockingHit)
	{
		if(ACollectible* CollectibleActor = Cast<ACollectible>(TractorResult.GetActor()))
		{
			CollectibleActor->Mesh->AddImpulse((GetActorLocation()-CollectibleActor->GetActorLocation())*5000*GetWorld()->DeltaTimeSeconds);
		}
	}
}

void APlayerSpaceShip::TractorBeamStarted()
{
	SpawnedSound = UGameplayStatics::SpawnSound2D(GetWorld(),BeamSound);
}

void APlayerSpaceShip::TractorBeamReleased()
{
	SpawnedSound->Stop();
	SpawnedSound->DestroyComponent();
}

void APlayerSpaceShip::MovementController(float DeltaTime)
{
	FVector TempVector = GetPendingMovementInputVector() * DeltaTime * 2000;
	ConsumeMovementInputVector();
}

void APlayerSpaceShip::LineTraceFromCamera()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		MyCamera->GetComponentLocation(),
		MyCamera->GetComponentLocation() + MyCamera->GetForwardVector() * CameraLineTraceDistance,
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true);

	if (HitResult.bBlockingHit)
	{
		AimLocation = HitResult.ImpactPoint;
		HitComponent = HitResult.GetComponent();
	}
	else
	{
		AimLocation = MyCamera->GetComponentLocation() + MyCamera->GetForwardVector() * CameraLineTraceDistance;
		HitComponent = nullptr;
	}

	if (!AimLocation.IsNearlyZero() && HitComponent)
		{
		AimCursor->SetWorldLocation(AimLocation);
		AimCursor->AttachToComponent(HitComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	else
	{
		AimCursor->SetWorldLocation(AimLocation);
		AimCursor->SetupAttachment(GetRootComponent());
	}
	
}

