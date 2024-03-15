// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "RFCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class URFEquipmentComponent;
class URFAbilitySystemComponent;
class ARFPlayerState;
class UProceduralAnimComponent;
class URFWeaponInstance;

class UInputAction;
class URFAbilityInputAction;
class UInputMappingContext;
class URFAbilityInputData;
struct FInputActionValue;

UCLASS()
class RT_FPS_API ARFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARFCharacter();
	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	// Client only
	virtual void OnRep_PlayerState() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);

public:
	void EquipWeapon();
	void UnEquipWeapon();

public:
	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasWeapon(bool bNewHasWeapon);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasWeapon();

	const TSubclassOf<URFWeaponInstance> GetWeaoponInstance() const;
	URFAbilityInputData* GetAbilityInputData() const;
	const URFAbilityInputAction* GetAbilityInputActionByTag(FGameplayTag InputTag) const;
	const TArray<URFAbilityInputAction*> GetAllAbilityInputAction() const;
	const TMap<FGameplayTag, URFAbilityInputAction*> GetAllAbilityInputMap() const;
	ARFPlayerState* GetRFPlayerState() const;

	/** Returns Mesh1P subobject **/
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	/* Returns Equipment subobject */
	FORCEINLINE URFEquipmentComponent* GetEquipmentComponent() const { return EquipmentComponent; }
	URFAbilitySystemComponent* GetCachedAbilitySystemComponent() const;

protected:
	// Caching ASC
	UPROPERTY()
	TObjectPtr<URFAbilitySystemComponent> AbilitySystemComponent;

private:
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	bool bHasWeapon;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P_Leg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName HideTPArmSocketName_L;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName HideTPArmSocketName_R;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName HideTPArmSocketName_Neck;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName HideFPThighSocketName_L;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	FName HideFPThighSocketName_R;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProceduralMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProceduralAnimComponent> ProceduralAnimComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
};
