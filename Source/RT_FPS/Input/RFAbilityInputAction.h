// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "RFAbilityInputAction.generated.h"

/**
 * 
 */
UCLASS()
class RT_FPS_API URFAbilityInputAction : public UInputAction
{
	GENERATED_BODY()

protected:
    //FGuids are globally unique.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GUID")
    mutable FGuid InputActionGuid;

public:
    // Reference : https://voidbit.dev/automate-input-action-to-gameplay-ability-bindings/
    int32 GetInputID() const
    {
        if (!InputActionGuid.IsValid())
        {
            FPlatformMisc::CreateGuid(InputActionGuid);
        }
        //FGuids are globally unique. They are made out of four int32 values
        //i.e. the quadruple of these int32 values is unique
        //and so adding them up together, 
        //returns a globally unique int32 value

        return GetTypeHash(InputActionGuid);
    }
};
