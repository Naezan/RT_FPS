// Fill out your copyright notice in the Description page of Project Settings.


#include "RFCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystem/RFAbilitySystemComponent.h"
#include "AbilitySystem/RFAbilityInputData.h"
#include "Input/RFAbilityInputAction.h"
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

			if (URFAbilityInputData* InputData = GetAbilityInputData())
			{
				const UInputMappingContext* AbilityInputMappingContext = InputData->GetAbilityInputMapping();
				Subsystem->AddMappingContext(AbilityInputMappingContext, 1);
			}
			else
			{
				UE_LOG(LogRF, Error, TEXT("'%s' Failed to load AbilityInputMapping"), *RF_CUR_CLASS_LINE);
			}
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

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			// Equip when firstplay
			EquipWeapon();
		}));

	ProceduralAnimComponent->InitProceduralProcess(ProceduralMeshComponent, Mesh1P);
}

void ARFCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent = GetCachedAbilitySystemComponent();

	if (AbilitySystemComponent != nullptr)
	{
		if (ARFPlayerState* PS = GetRFPlayerState())
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
		else
		{
			UE_LOG(LogRF, Error, TEXT("'%s' Failed to find PlayerState"), *RF_CUR_CLASS_LINE);
		}
	}
	else
	{
		UE_LOG(LogRF, Error, TEXT("'%s' Failed to find AbilitySystemComponent"), *RF_CUR_CLASS_LINE);
	}

}

void ARFCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent = GetCachedAbilitySystemComponent();

	if (AbilitySystemComponent != nullptr)
	{
		if (ARFPlayerState* PS = GetRFPlayerState())
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
		else
		{
			UE_LOG(LogRF, Error, TEXT("'%s' Failed to find PlayerState"), *RF_CUR_CLASS_LINE);
		}
	}
	else
	{
		UE_LOG(LogRF, Error, TEXT("'%s' Failed to find AbilitySystemComponent"), *RF_CUR_CLASS_LINE);
	}
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

		const TMap<FGameplayTag, URFAbilityInputAction*> AllAbilityInputMap = GetAllAbilityInputMap();
		for (const auto& InputInfo : AllAbilityInputMap)
		{
			EnhancedInputComponent->BindAction(InputInfo.Value, ETriggerEvent::Triggered, this, &ARFCharacter::OnAbilityInputPressed, InputInfo.Key);
			EnhancedInputComponent->BindAction(InputInfo.Value, ETriggerEvent::Completed, this, &ARFCharacter::OnAbilityInputReleased, InputInfo.Key);
		}
	}
	else
	{
		UE_LOG(LogRF, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARFCharacter::OnAbilityInputPressed(FGameplayTag InputTag)
{
	const URFAbilityInputAction* InputAction = Cast<URFAbilityInputAction>(GetAbilityInputActionByTag(InputTag));

	AbilitySystemComponent->AbilityLocalInputPressed(InputAction->GetInputID());
}

void ARFCharacter::OnAbilityInputReleased(FGameplayTag InputTag)
{
	const URFAbilityInputAction* InputAction = Cast<URFAbilityInputAction>(GetAbilityInputActionByTag(InputTag));

	AbilitySystemComponent->AbilityLocalInputReleased(InputAction->GetInputID());
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
	if (ARFPlayerState* PS = GetRFPlayerState())
	{
		if (const URFPlayerData* PlayerData = PS->GetPlayerData())
		{
			return PlayerData->EquipWeaponInstance;
		}
	}

	return nullptr;
}

URFAbilityInputData* ARFCharacter::GetAbilityInputData() const
{
	ARFPlayerState* PS = GetRFPlayerState();
	if (PS == nullptr)
	{
		UE_LOG(LogRF, Error, TEXT("Fail to found PlayerState [%s]"), *RF_CUR_CLASS_FUNC);
		return nullptr;
	}

	if (const URFPlayerData* PlayerData = PS->GetPlayerData())
	{
		if (PlayerData->AbilityInputData != nullptr)
		{
			return PlayerData->AbilityInputData;
		}
	}

	return nullptr;
}

const UInputAction* ARFCharacter::GetAbilityInputActionByTag(FGameplayTag InputTag) const
{
	URFAbilityInputData* InputData = GetAbilityInputData();

	if (InputData != nullptr)
	{
		return InputData->FindAbilityInputActionByTag(InputTag);
	}

	return nullptr;
}

int32 ARFCharacter::GetAbilityInputActionIDByTag(FGameplayTag InputTag) const
{
	if (const URFAbilityInputAction* InputAction = Cast<URFAbilityInputAction>(GetAbilityInputActionByTag(InputTag)))
	{
		return InputAction->GetInputID();
	}

	return INDEX_NONE;
}

const TArray<URFAbilityInputAction*> ARFCharacter::GetAllAbilityInputAction() const
{
	URFAbilityInputData* InputData = GetAbilityInputData();

	TArray<URFAbilityInputAction*> InputActions;
	if (InputData != nullptr)
	{
		InputActions = InputData->GetAllInputActions();
	}

	return InputActions;
}

const TMap<FGameplayTag, URFAbilityInputAction*> ARFCharacter::GetAllAbilityInputMap() const
{
	URFAbilityInputData* InputData = GetAbilityInputData();

	TArray<URFAbilityInputAction*> InputActions;
	if (InputData != nullptr)
	{
		return InputData->GetAbilityInputInfo();
	}

	return TMap<FGameplayTag, URFAbilityInputAction*>();
}

ARFPlayerState* ARFCharacter::GetRFPlayerState() const
{
	return GetPlayerState<ARFPlayerState>();
}

UAnimInstance* ARFCharacter::GetFPAnimInstance() const
{
	const USkeletalMeshComponent* FPMesh = GetFPMesh();
	return FPMesh ? FPMesh->GetAnimInstance() : nullptr;
}

URFAbilitySystemComponent* ARFCharacter::GetCachedAbilitySystemComponent() const
{
	if (ARFPlayerState* PS = GetRFPlayerState())
	{
		return PS->GetRFAbilitySystemComponent();
	}

	return nullptr;
}
