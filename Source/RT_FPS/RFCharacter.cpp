// Fill out your copyright notice in the Description page of Project Settings.


#include "RFCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystem/RFAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "RFEquipmentComponent.h"
#include "ProceduralAnimComponent.h"
#include "RFWeaponInstance.h"
#include "RFPlayerState.h"
#include "RFPlayerData.h"
#include "RFLogMacros.h"

// Sets default values
ARFCharacter::ARFCharacter()
{
	// Character doesnt have a rifle at start
	bHasWeapon = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(34.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	ProceduralMeshComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProceduralMeshComponent"));
	ProceduralMeshComponent->SetupAttachment(FirstPersonCameraComponent);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(ProceduralMeshComponent);
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P->SetCollisionProfileName(FName("NoCollision"));
	// Always animation pose when far from other players
	Mesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	// Prevent attach decal on 1p mesh
	Mesh1P->bReceivesDecals = false;
	// Stop draw shadow on 1p mesh
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-25.f, -15.f, -156.f));
	Mesh1P->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));

	Mesh1P_Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P_Leg"));
	Mesh1P_Leg->SetOnlyOwnerSee(true);
	Mesh1P_Leg->SetupAttachment(GetCapsuleComponent());
	Mesh1P_Leg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh1P_Leg->SetCollisionProfileName(FName("NoCollision"));
	// Always animation pose when far from other players
	Mesh1P_Leg->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	// Prevent attach decal on 1p mesh
	Mesh1P_Leg->bReceivesDecals = false;
	// Stop draw shadow on 1p mesh
	Mesh1P_Leg->bCastDynamicShadow = false;
	Mesh1P_Leg->CastShadow = false;
	Mesh1P_Leg->SetRelativeLocation(FVector(0.f, 0.f, -166.f));

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionProfileName(FName("3PMesh"));
	// Show mesh shadow even if upperbody is hidden or fullbody is hidden
	GetMesh()->bCastHiddenShadow = true;
	GetMesh()->bReceivesDecals = true;
	// Trigger event when character walk to event zone
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	EquipmentComponent = CreateDefaultSubobject<URFEquipmentComponent>(TEXT("EquipmentComponent"));
	ProceduralAnimComponent = CreateDefaultSubobject<UProceduralAnimComponent>(TEXT("ProceduralAnimComponent"));
}

void ARFCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ARFPlayerState* PS = GetRFPlayerState())
	{
		AbilitySystemComponent = PS->GetRFAbilitySystemComponent();
	}
}

// Called when the game starts or when spawned
void ARFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Hide 3P mesh on player accept lowerbody
	if (IsLocallyControlled())
	{
		Mesh1P_Leg->HideBoneByName(HideTPArmSocketName_L, EPhysBodyOp::PBO_None);
		Mesh1P_Leg->HideBoneByName(HideTPArmSocketName_R, EPhysBodyOp::PBO_None);
		Mesh1P_Leg->HideBoneByName(HideTPArmSocketName_Neck, EPhysBodyOp::PBO_None);

		Mesh1P->HideBoneByName(HideFPThighSocketName_L, EPhysBodyOp::PBO_None);
		Mesh1P->HideBoneByName(HideFPThighSocketName_R, EPhysBodyOp::PBO_None);
		Mesh1P->HideBoneByName(HideTPArmSocketName_Neck, EPhysBodyOp::PBO_None);
	}

	// Equip when firstplay
	EquipWeapon();

	ProceduralAnimComponent->InitProceduralProcess(ProceduralMeshComponent, Mesh1P);
}

void ARFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARFCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARFCharacter::Look);
	}
	else
	{
		UE_LOG(LogRF, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARFCharacter::EquipWeapon()
{
	if (bHasWeapon)
	{
		return;
	}

	if (EquipmentComponent->OnEquip(GetWeaoponInstance()))
	{
		bHasWeapon = true;
	}
}

void ARFCharacter::UnEquipWeapon()
{
	if (!bHasWeapon)
	{
		return;
	}

	if (EquipmentComponent->OnUnEquip())
	{
		bHasWeapon = false;
	}
}

void ARFCharacter::Move(const FInputActionValue& Value)
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

void ARFCharacter::Look(const FInputActionValue& Value)
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

void ARFCharacter::SetHasWeapon(bool bNewHasWeapon)
{
	bHasWeapon = bNewHasWeapon;
}

bool ARFCharacter::GetHasWeapon()
{
	return bHasWeapon;
}

const TSubclassOf<URFWeaponInstance> ARFCharacter::GetWeaoponInstance() const
{
	if (ARFPlayerState* PS = GetPlayerState<ARFPlayerState>())
	{
		if (const URFPlayerData* PlayerData = PS->GetPlayerData())
		{
			return PlayerData->EquipWeaponInstance;
		}
	}

	return nullptr;
}

ARFPlayerState* ARFCharacter::GetRFPlayerState() const
{
	return GetPlayerState<ARFPlayerState>();
}
