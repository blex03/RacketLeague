// Fill out your copyright notice in the Description page of Project Settings.


#include "RacketPawn.h"
#include "RacketPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARacketPawn::ARacketPawn()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Racket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Racket"));
	Racket->SetupAttachment(Box);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 1.f;
	RotateScale = 50.f;
}


// Called to bind functionality to input
void ARacketPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ARacketPlayerController* RPC = Cast<ARacketPlayerController>(Controller);
	check(EIC && RPC);
	EIC->BindAction(RPC->MoveAction, ETriggerEvent::Triggered, this, &ARacketPawn::Move);
	EIC->BindAction(RPC->RotateAction, ETriggerEvent::Triggered, this, &ARacketPawn::Rotate);

	ULocalPlayer* LocalPlayer = RPC->GetLocalPlayer();
	check(LocalPlayer);
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem < UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(RPC->PawnMappingContext, 0);
}

void ARacketPawn::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	 
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
}

void ARacketPawn::Rotate(const FInputActionValue& ActionValue)
{
	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;
	SetActorRotation(Input);
}

