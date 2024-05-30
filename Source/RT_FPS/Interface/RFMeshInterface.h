// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RFMeshInterface.generated.h"

class USkeletalMeshComponent;
class UAnimInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URFMeshInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RT_FPS_API IRFMeshInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual USkeletalMeshComponent* GetFPMesh() const { return nullptr; }
	virtual USkeletalMeshComponent* GetFPLegMesh() const { return nullptr; }
	virtual UAnimInstance* GetFPAnimInstance() const { return nullptr; }

	virtual float GetYawAO() const { return 0.f; }
	virtual float GetPitchAO() const { return 0.f; }
	virtual float GetRotateYaw() const { return 0.f; }
	virtual bool IsTurnRight() const { return false; }
	virtual bool IsTurnLeft() const { return false; }

	virtual bool IsLHandIK() const { return false; }
};
