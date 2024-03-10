// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveVector.h"
#include "ProceduralAnimComponent.generated.h"

class UCharacterMovementComponent;
class UInputAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RT_FPS_API UProceduralAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UProceduralAnimComponent();

public:
	void InitProceduralProcess(UActorComponent* ProceduralComponent, UActorComponent* WeaponMesh);
	void UpdateProceduralMovement();

private:
	void UpdateWalkingMovement(FVector& OutTargetLocation, FRotator& OutTargetRotation);
	float UpdateMoveSway();
	FRotator UpdateLookSway();
	float ClampLookSway(float LookSwayValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSwayZRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator LookSwayRotation;

private:
	UPROPERTY()
	FTimerHandle ProcedulTimer;
	UPROPERTY()
	FVector OriginMoveLocation;
	UPROPERTY()
	FRotator OriginMoveRotation;
	UPROPERTY()
	float CurveDeltaTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ACharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProceduralRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Idle", meta = (AllowPrivateAccess = "true"))
		UCurveVector* IdleLocationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Idle", meta = (AllowPrivateAccess = "true"))
		UCurveVector* IdleRotationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Walk", meta = (AllowPrivateAccess = "true"))
		UCurveVector* WalkingLocationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Walk", meta = (AllowPrivateAccess = "true"))
		UCurveVector* WalkingRotationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Walk", meta = (AllowPrivateAccess = "true"))
	float MoveInterpSpeed = 4.f;

	// Weapon recoil curve using when firing(simular firing camera shake, but recoil curve is additional camera effect)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Recoil", meta = (AllowPrivateAccess = "true"))
		UCurveVector* RecoilLocationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Recoil", meta = (AllowPrivateAccess = "true"))
		UCurveVector* RecoilRotationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Aim", meta = (AllowPrivateAccess = "true"))
		UCurveFloat* AimAnimationCurve;

	// 1P Arm ADS socket position that used when click right mouse button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Aim", meta = (AllowPrivateAccess = "true"))
		FName AimSocketName = "Point_ADS";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Look Sway", meta = (AllowPrivateAccess = "true"))
		float LookSwaySpeed = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Look Sway", meta = (AllowPrivateAccess = "true"))
		float LookSwayScale = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Look Sway", meta = (AllowPrivateAccess = "true"))
		float LookSwayRotCap = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Move Sway", meta = (AllowPrivateAccess = "true"))
		float MoveSwayScale = 2.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Anim|Move Sway", meta = (AllowPrivateAccess = "true"))
		float SwayInterpSpeed = 4.f;
};
