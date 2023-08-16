// Copyright Epic Games, Inc. All Rights Reserved.

#include "AudioGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AAudioGameCharacter

AAudioGameCharacter::AAudioGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a sphere component for ping
	SphereCollisionForPing = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionForPing"));
	SphereCollisionForPing->SetupAttachment(GetCapsuleComponent());
	SphereCollisionForPing->SetSphereRadius(0.f);

}

void AAudioGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void AAudioGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update Ping Radius
	if (IsPinging)
	{
		GrowPingSphere(DeltaTime);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AAudioGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(PingAction, ETriggerEvent::Started, this, &AAudioGameCharacter::StartPing);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAudioGameCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAudioGameCharacter::Look);
	}
}


void AAudioGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AAudioGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAudioGameCharacter::StartPing()
{
	IsPinging = true;
}

void AAudioGameCharacter::GrowPingSphere(float DeltaTime)
{
	if (SphereCollisionForPing)
	{
		// Get current radius
		float CurrentRadius = SphereCollisionForPing->GetScaledSphereRadius();

		// Get new radius
		float NewRadius = CurrentRadius + (DeltaTime * PingPropagationSpeed);

		// Set new radius
		SphereCollisionForPing->SetSphereRadius(NewRadius);

		// Check if ping is complete
		if (NewRadius >= MaxPingRadius)
		{
			IsPinging = false;
			SphereCollisionForPing->SetSphereRadius(0.f);
		}

	} else { UE_LOG(LogTemp, Warning, TEXT("SphereCollisionForPing is null")); }
}