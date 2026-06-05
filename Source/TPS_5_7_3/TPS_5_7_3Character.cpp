// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_5_7_3Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "TPS_5_7_3/Components/TPSInventoryComponent.h"
#include "TPS_5_7_3/Public/Components/GridPlacementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TPS_5_7_3.h"

ATPS_5_7_3Character::ATPS_5_7_3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	InventoryComponent = CreateDefaultSubobject<UTPSInventoryComponent>(TEXT("Inventory"));
}

void ATPS_5_7_3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPS_5_7_3Character::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPS_5_7_3Character::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPS_5_7_3Character::Look);
	}
	else
	{
		UE_LOG(LogTPS_5_7_3, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPS_5_7_3Character::BeginPlay()
{
	Super::BeginPlay();
	//check(Controller);
	//check(IsValid(Controller));
	check(HealthData.MaxHealth > 0.0f);
	Health = HealthData.MaxHealth;
	
	OnTakeAnyDamage.AddDynamic(this, &ATPS_5_7_3Character::OnAnyDamageReceived);
}

float ATPS_5_7_3Character::GetHealthPercent() const
{
	return Health / HealthData.MaxHealth;
}

void ATPS_5_7_3Character::OnAnyDamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	const auto IsAlive = [&](){ return Health > 0.0f;};
	if (Damage <= 0.0f || !IsAlive()) return;
	
	Health = FMath::Clamp(Health - Damage, 0.0f, HealthData.MaxHealth);
	
	if (IsAlive())
	{
		GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ATPS_5_7_3Character::OnHealing, HealthData.HealRate, true, -1.0f);
	} 
	else
	{
		OnDeath();
	}
}

void ATPS_5_7_3Character::OnHealing()
{
	Health = FMath::Clamp(Health + HealthData.HealModifier, 0.0f, HealthData.MaxHealth);
	if (FMath::IsNearlyEqual(Health, HealthData.MaxHealth))
	{
		Health = HealthData.MaxHealth;
		GetWorldTimerManager().ClearTimer(HealTimerHandle);
	}
}

void ATPS_5_7_3Character::OnDeath()
{
	GetWorldTimerManager().ClearTimer(HealTimerHandle);
	
	check(GetCharacterMovement());
	check(GetCapsuleComponent());
	check(GetMesh());
	
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
	
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	
	SetLifeSpan(HealthData.LifeSpan);
}

void ATPS_5_7_3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPS_5_7_3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATPS_5_7_3Character::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ATPS_5_7_3Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATPS_5_7_3Character::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ATPS_5_7_3Character::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
