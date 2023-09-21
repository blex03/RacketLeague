// Fill out your copyright notice in the Description page of Project Settings.
#include "Racket2Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Vector2D.h"
#include "Components/PrimitiveComponent.h"



// Sets default values
ARacket2Pawn::ARacket2Pawn()
{
	Collision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	Racket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Racket"));
	Racket->SetupAttachment(Collision);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Collision);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 1.f;
	RotateScale = 50.f;
	JumpScale = 1.f;
}

// Called when the game starts or when spawned
void ARacket2Pawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(RacketMappingContext, 0);
		}
	}
	
}

void ARacket2Pawn::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>() * MoveScale;

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	if (MovementVector.X != 0 && MovementVector.Y != 0)
	{
		AddMovementInput(Forward, (MovementVector.Y * MoveScale) / sqrt(2));
		AddMovementInput(Right, (MovementVector.X * MoveScale) / sqrt(2));
	}
	else {
		AddMovementInput(Forward, MovementVector.Y * MoveScale);
		AddMovementInput(Right, MovementVector.X * MoveScale);
	}
}


void ARacket2Pawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>() * RotateScale;
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
	
}


void ARacket2Pawn::Jump(const FInputActionValue& Value)
{
	const bool JumpValue = Value.Get<bool>();

	FVector Impulse(0, 0, 500);

	if (JumpValue)
	{
		Racket->AddImpulse(Impulse);
	}

}


// Called to bind functionality to input
void ARacket2Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARacket2Pawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARacket2Pawn::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARacket2Pawn::Jump);
	}

}

