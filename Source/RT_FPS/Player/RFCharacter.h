// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "Interface/AbilityInputInterface.h"
#include "Interface/RFMeshInterface.h"
#include "AbilitySystemInterface.h"
#include "RFCharacter.generated.h"

class UInputComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class URFEquipmentComponent;
class URFAbilitySystemComponent;
class URFAttributeSet;
class ARFPlayerState;
class URFAbilitySet;
class UProceduralAnimComponent;
class URFWeaponInstance;

class UInputAction;
class URFAbilityInputAction;
class UInputMappingContext;
class URFAbilityInputData;
struct FInputActionValue;

class UTimelineComponent;
class UCurveVector;
class UCurveFloat;

struct FGameplayEffectSpec;

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	Alive,
	DeathStarted,
	DeathFinished
};

UCLASS()
class RT_FPS_API ARFCharacter : public ACharacter, public IAbilitySystemInterface, public IAbilityInputInterface, public IRFMeshInterface
{
	GENERATED_BODY()

public:
	ARFCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Client only
	virtual void OnRep_PlayerState() override;

	UFUNCTION()
	void InitializeMovement();
	UFUNCTION()
	void InputInitialized();
	UFUNCTION(Server, Reliable)
	void ServerEquipWeapon();
	UFUNCTION()
	void StartEquipWeapon();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	UFUNCTION(Unreliable, Server)
	void UpdateTurnInPlaceData();

	void SwitchCrouch(const FInputActionValue& Value);
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void SwitchAimingTrasition(const FInputActionValue& Value);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);

public:
	//~Weapon
	void EquipWeapon();
	void UnEquipWeapon();
	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasWeapon(bool bNewHasWeapon);
	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasWeapon();
	//~End of Weapon

	//~WeaponAbility
	URFAbilityInputData* GetAbilityInputData() const;
	virtual const UInputAction* GetAbilityInputActionByTag(FGameplayTag InputTag) const override;
	virtual int32 GetAbilityInputActionIDByTag(FGameplayTag InputTag) const override;
	virtual void ResetInputIDByTag(FGameplayTag InputTag) const override;
	virtual void RegisterInputIDByTag(int32 InputID, FGameplayTag InputTag) override;
	const TArray<URFAbilityInputAction*> GetAllAbilityInputAction() const;
	const TMap<FGameplayTag, URFAbilityInputAction*> GetAllAbilityInputMap() const;
	//~End of WeaponAbility

	//~Getter
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	URFAbilitySystemComponent* GetCachedAbilitySystemComponent() const;
	ARFPlayerState* GetRFPlayerState() const;
	const TSubclassOf<URFWeaponInstance> GetWeaoponInstance() const;
	/** Returns Mesh1P subobject **/
	virtual USkeletalMeshComponent* GetFPMesh() const override { return Mesh1P; }
	virtual USkeletalMeshComponent* GetFPLegMesh() const override { return Mesh_Leg; }
	virtual UAnimInstance* GetFPAnimInstance() const override;
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	/* Returns Equipment subobject */
	FORCEINLINE URFEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	//~End Getter

	virtual float GetYawAO() const override { return YawAO; }
	virtual float GetPitchAO() const override { return PitchAO; }
	virtual float GetRotateYaw() const override { return RotateYaw; }
	virtual bool IsTurnRight() const override { return bIsTurnRight; }
	virtual bool IsTurnLeft() const override { return bIsTurnLeft; }

	UFUNCTION(Reliable, Server)
	void SetAiming(bool bInAiming);
	UFUNCTION(BlueprintPure)
	bool IsAiming() const { return bIsAiming; }

	EDeathState GetDeathStatus() const { return DeathStatus; }
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool IsDeathStart() const { return DeathStatus != EDeathState::Alive; }

	void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandleOutOfHealth(AActor* DamageVictim, AActor* DamageCauser, const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude);
	void DeathStart();
	void DeathFinish();

	void StopMovementAfterDeath();
	UFUNCTION(NetMulticast, Reliable)
	void RagDoll();
	void DeathCameraTransition();
	void TrasitionCameraOnDeath();
	void ChangeVisibilityOnDeath();

private:
	UFUNCTION()
	void OnAimTransitionLocUpdate(FVector InLocation);
	UFUNCTION()
	void OnAimTransitionRotUpdate(FVector InRotation);

	UFUNCTION()
	void OnRep_IsAiming();
	UFUNCTION()
	void OnRep_DeathStatus(EDeathState OldDeathStatus);

	UFUNCTION(Client, Reliable)
	void ClientRegisterInputID(int32 InputID, FGameplayTag InputTag);

protected:
	// Caching ASC
	UPROPERTY()
	TWeakObjectPtr<URFAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TWeakObjectPtr<URFAttributeSet> HealthAttributeSet;

private:
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	uint8 bHasWeapon : 1;
	UPROPERTY(ReplicatedUsing = OnRep_IsAiming, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Aim", meta = (AllowPrivateAccess = "true"))
	uint8 bIsAiming : 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Aim", meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Aim", meta = (AllowPrivateAccess = "true"))
	float AimWalkSpeed;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float YawAO;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PitchAO;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RotateYaw;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnRight : 1;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnLeft : 1;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh_Leg;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FirstPersonSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* DeathCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProceduralMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float StandCameraLagSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CrouchTransitionCameraLagSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFEquipmentComponent> EquipmentComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProceduralAnimComponent> ProceduralAnimComponent;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;
	UPROPERTY(VisibleAnywhere, Category = Input, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, int32> AbilityInputID;

	/* Aiming Timeline */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Aim|Timeline", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTimelineComponent> AimTimelineComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Aim|Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveVector* AimLocationCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Aim|Timeline", meta = (AllowPrivateAccess = "true"))
	UCurveVector* AimRotationCurve;

	FTimerHandle RagDollTimer;
	UPROPERTY(ReplicatedUsing = OnRep_DeathStatus)
	EDeathState DeathStatus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Transition", meta = (AllowPrivateAccess = "true"))
	float TransitionSeconds = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Transition", meta = (AllowPrivateAccess = "true"))
	float TransitionSpeed = 2.f;
	FTimerHandle CameraBlendHandle;
};
