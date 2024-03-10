// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralAnimComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// Sets default values for this component's properties
UProceduralAnimComponent::UProceduralAnimComponent()
{

}

void UProceduralAnimComponent::InitProceduralProcess(UActorComponent* ProceduralComponent, UActorComponent* WeaponMesh)
{
	OwningCharacter = Cast<ACharacter>(GetOwner());
	CharacterMovementComponent = Cast<ACharacter>(GetOwner())->GetCharacterMovement();
	ProceduralRootComponent = Cast<USceneComponent>(ProceduralComponent);
	WeaponSkeletalMesh = Cast<USkeletalMeshComponent>(WeaponMesh);

	if (ProceduralRootComponent)
	{
		OriginMoveLocation = ProceduralRootComponent->GetRelativeLocation();
		OriginMoveRotation = ProceduralRootComponent->GetRelativeRotation();

		GetWorld()->GetTimerManager().SetTimer(
			ProcedulTimer,
			this,
			&UProceduralAnimComponent::UpdateProceduralMovement,
			0.01f,
			true);
	}
}

void UProceduralAnimComponent::UpdateProceduralMovement()
{
	FVector ResultLocation;
	FRotator ResultRotation;

	FVector TargetMoveLocation;
	FRotator TargetMoveRotation;

	UpdateWalkingMovement(TargetMoveLocation, TargetMoveRotation);
	MoveSwayZRotation = UpdateMoveSway();
	LookSwayRotation = UpdateLookSway();

	ResultLocation = TargetMoveLocation;
	ResultRotation = TargetMoveRotation;

	// Apply MoveSway on ResultRotation
	ResultRotation.Roll = FMath::Lerp(ResultRotation.Roll, MoveSwayZRotation, 0.5f);
	// Apply LookSway on ResultRotation
	ResultRotation.Pitch = FMath::Lerp(ResultRotation.Pitch, LookSwayRotation.Pitch, 0.5f);
	ResultRotation.Yaw = FMath::Lerp(ResultRotation.Yaw, LookSwayRotation.Yaw, 0.5f);

	ProceduralRootComponent->SetRelativeLocation(ResultLocation);
	ProceduralRootComponent->SetRelativeRotation(ResultRotation);
}

void UProceduralAnimComponent::UpdateWalkingMovement(FVector& OutTargetLocation, FRotator& OutTargetRotation)
{
	CurveDeltaTime += 0.01f;

	FVector TargetLocation;
	FRotator TargetRotation;

	float BeginTimeLocation = 0.f;
	float EndTimeLocation = 0.f;

	float BeginTimeRotation = 0.f;
	float EndTimeRotation = 0.f;

	if (CharacterMovementComponent->Velocity != FVector(0, 0, 0) && !CharacterMovementComponent->IsFalling()) /* Walk */
	{
		if (WalkingLocationCurve != nullptr)
		{
			WalkingLocationCurve->GetTimeRange(BeginTimeLocation, EndTimeLocation);
			TargetLocation = WalkingLocationCurve->GetVectorValue(CurveDeltaTime);
		}
		else
		{
			TargetLocation = OriginMoveLocation;
		}

		if (WalkingRotationCurve != nullptr)
		{
			WalkingRotationCurve->GetTimeRange(BeginTimeRotation, EndTimeRotation);
			TargetRotation = FRotator(WalkingRotationCurve->GetVectorValue(CurveDeltaTime).X,
				WalkingRotationCurve->GetVectorValue(CurveDeltaTime).Y,
				WalkingRotationCurve->GetVectorValue(CurveDeltaTime).Z);
		}
		else
		{
			TargetRotation = OriginMoveRotation;
		}
	}
	else /* Idle*/
	{
		if (IdleLocationCurve != nullptr)
		{
			IdleLocationCurve->GetTimeRange(BeginTimeLocation, EndTimeLocation);
			TargetLocation = IdleLocationCurve->GetVectorValue(CurveDeltaTime);
		}
		else
		{
			TargetLocation = OriginMoveLocation;
		}

		if (IdleRotationCurve != nullptr)
		{
			IdleRotationCurve->GetTimeRange(BeginTimeRotation, EndTimeRotation);
			TargetRotation = FRotator(IdleRotationCurve->GetVectorValue(CurveDeltaTime).X,
				IdleRotationCurve->GetVectorValue(CurveDeltaTime).Y,
				IdleRotationCurve->GetVectorValue(CurveDeltaTime).Z);
		}
		else
		{
			TargetRotation = OriginMoveRotation;
		}
	}

	if (CurveDeltaTime >= EndTimeLocation)
	{
		CurveDeltaTime = 0;
	}

	OutTargetLocation = FMath::VInterpTo(
		ProceduralRootComponent->GetRelativeLocation(),
		TargetLocation,
		0.01f,
		MoveInterpSpeed);
	OutTargetRotation = FMath::RInterpTo(
		ProceduralRootComponent->GetRelativeRotation(),
		TargetRotation,
		0.01f,
		MoveInterpSpeed);
}

float UProceduralAnimComponent::UpdateMoveSway()
{
	const FVector Velocity = CharacterMovementComponent->Velocity;
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const FVector Right = GetOwner()->GetActorRightVector();
	// Character capsulecomponent base movement direciton
	FVector MovementDir = FVector(FVector::DotProduct(Velocity, Forward), FVector::DotProduct(Velocity, Right), 0);
	MovementDir.Normalize();

	// How mush rolling arm
	return FMath::FInterpTo(
		ProceduralRootComponent->GetRelativeRotation().Roll,
		FMath::Clamp(MovementDir.Y * MoveSwayScale, -MoveSwayScale, MoveSwayScale) /* How much moved from left/right(Y) */,
		0.01f,
		SwayInterpSpeed);
}

FRotator UProceduralAnimComponent::UpdateLookSway()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwningCharacter->InputComponent);

	if (!EnhancedInputComponent)
	{
		return ProceduralRootComponent->GetRelativeRotation();
	}

	FEnhancedInputActionValueBinding* MoveActionBinding = &EnhancedInputComponent->BindActionValue(LookAction);
	const FVector2D InputValue = MoveActionBinding->GetValue().Get<FVector2D>();
	
	const FRotator TargetRotation = FRotator(
		ClampLookSway(-InputValue.Y * LookSwayScale),
		ClampLookSway(InputValue.X * LookSwayScale),
		0
	);

	return FMath::Lerp(
		ProceduralRootComponent->GetRelativeRotation(),
		TargetRotation, LookSwaySpeed * GetWorld()->DeltaTimeSeconds);
}

float UProceduralAnimComponent::ClampLookSway(float LookSwayValue)
{
	return LookSwayValue > LookSwayRotCap ? LookSwayRotCap : LookSwayValue < -LookSwayRotCap ? -LookSwayRotCap : LookSwayValue;
}
