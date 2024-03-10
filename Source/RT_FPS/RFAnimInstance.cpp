// Fill out your copyright notice in the Description page of Project Settings.


#include "RFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URFAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
	MovementComponent = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;
}

void URFAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(OwningCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to load OwningCharacter!"), *GetNameSafe(this));
		return;
	}

	CharacterRotation = OwningCharacter->GetActorRotation();

	// Calculate IsFalling
	bIsFalling = MovementComponent->IsFalling();

	UpdateVelocityData();
	UpdateAccelerationData();
	UpdateTurnInPlaceData();
	UpdateMovementStateData();
}

void URFAnimInstance::UpdateVelocityData()
{
	// Caculate MovementSpeed
	Velocity = MovementComponent->Velocity;
	Velocity2D = Velocity * FVector(1, 1, 0);
	GroundSpeed = Velocity2D.Size();

	// Last Moved velocity
	LastVelocity2D = CharacterRotation.UnrotateVector(Velocity2D);

	// Calculate IsMove
	bHasVelocity = !FMath::IsNearlyZero(LastVelocity2D.SizeSquared2D());
}

void URFAnimInstance::UpdateAccelerationData()
{
	Acceleration = MovementComponent->GetCurrentAcceleration();
	Acceleration2D = Acceleration * FVector(1, 1, 0);
	LastAcceleration2D = CharacterRotation.UnrotateVector(Acceleration2D);

	bHasAcceleration = !FMath::IsNearlyZero(LastAcceleration2D.SizeSquared2D());
}

void URFAnimInstance::UpdateTurnInPlaceData()
{
	BaseAnimRotation = OwningCharacter->GetBaseAimRotation();

	// Diff with movement direction and animation direction
	FRotator Delta = FRotationMatrix::MakeFromX(Velocity).Rotator() - BaseAnimRotation;
	Delta.Normalize();

	// Update YawOffset
	YawOffset = Delta.Yaw;
}

void URFAnimInstance::UpdateMovementStateData()
{
	bIsCrouching = MovementComponent->IsCrouching();

	LocalDirectionAngle = CalculateDirectionAngle(Velocity2D, CharacterRotation);
}

float URFAnimInstance::CalculateDirectionAngle(const FVector& TargetVelocity, const FRotator& BaseRotation)
{
	if (!TargetVelocity.IsNearlyZero())
	{
		const FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		const FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		const FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		const FVector NormalizedVel = TargetVelocity.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		const float ForwardCosAngle = static_cast<float>(FVector::DotProduct(ForwardVector, NormalizedVel));
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		const float RightCosAngle = static_cast<float>(FVector::DotProduct(RightVector, NormalizedVel));
		if (RightCosAngle < 0.f)
		{
			ForwardDeltaDegree *= -1.f;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}
