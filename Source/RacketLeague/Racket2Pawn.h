// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Racket2Pawn.generated.h"


class UInputMappingContext;
class UInputAction;
class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS()
class RACKETLEAGUE_API ARacket2Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARacket2Pawn();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* RacketMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void JumpCancel(const FInputActionValue& Value);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Collision;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Racket;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

	UPROPERTY(EditAnywhere)
	float JumpScale;

	bool isCanceled;

};
