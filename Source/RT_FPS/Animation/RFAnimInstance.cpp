// Fill out your copyright notice in the Description page of Project Settings.


#include "RFAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/RFMeshInterface.h"

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
	UpdateAimOffsetData();
	UpdateTurnInPlaceData();
	UpdateIKData();
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

void URFAnimInstance::UpdateAimOffsetData()
{
	if (IRFMeshInterface* CharacterMeshInterface = Cast<IRFMeshInterface>(OwningCharacter))
	{
		YawAO = CharacterMeshInterface->GetYawAO();
		PitchAO = CharacterMeshInterface->GetPitchAO();
	}
}

void URFAnimInstance::UpdateTurnInPlaceData()
{
	BaseAnimRotation = OwningCharacter->GetBaseAimRotation();
	FRotator DeltaRotator = FRotationMatrix::MakeFromX(LastMoveDirection).Rotator() - BaseAnimRotation;
	DeltaRotator.Normalize();

	if (GroundSpeed > 0.f)
	{
		YawOffset = 0.f;
	}
	else if (!bIsTurning)
	{
		if ((!bIsTurnRight && !bIsTurnLeft))
		{
			LastMoveDirection = (!!GroundSpeed) ? Velocity * FVector(1, 1, 0) : LastMoveDirection;
			LastMoveDirection.Normalize();

			YawOffset = DeltaRotator.Yaw;
		}
		else
		{
			bIsTurning = true;
		}

		ClampedYaw = FMath::Clamp(DeltaRotator.Yaw, -90.f, 90.f);

		bIsTurnRight = ClampedYaw < -70.f;
		bIsTurnLeft = ClampedYaw > 70.f;
	}

	if (bIsTurning)
	{
		YawElapseTime += GetWorld()->GetDeltaSeconds();

		LastMoveDirection = FMath::VInterpTo(LastMoveDirection, OwningCharacter->GetActorForwardVector(), GetWorld()->GetDeltaSeconds(), 5.f);
		YawOffset = FMath::FInterpTo(YawOffset, 0.f, GetWorld()->GetDeltaSeconds(), 4.f);

		if (FMath::IsNearlyEqual(YawOffset, 0.f) || YawElapseTime > 0.4f)
		{
			LastMoveDirection = OwningCharacter->GetActorForwardVector();
			YawOffset = 0.f;

			bIsTurning = false;
			YawElapseTime = 0.f;
		}
	}
}

void URFAnimInstance::UpdateIKData()
{
	if (IRFMeshInterface* CharacterMeshInterface = Cast<IRFMeshInterface>(OwningCharacter))
	{
		bDoLHandIK = CharacterMeshInterface->IsLHandIK();
	}
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
