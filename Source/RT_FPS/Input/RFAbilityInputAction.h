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
    FGuid InputActionGuid;

public:
    // Reference : https://voidbit.dev/automate-input-action-to-gameplay-ability-bindings/
    int32 GetInputID() const
    {
        static bool bHasInputID = false;
        if (!bHasInputID)
        {
            InputActionGuid.NewGuid();
            bHasInputID = true;
        }
        //FGuids are globally unique. They are made out of four int32 values
        //i.e. the quadruple of these int32 values is unique
        //and so adding them up together, 
        //returns a globally unique int32 value

        return static_cast<int32>(InputActionGuid.A)
            + static_cast<int32>(InputActionGuid.B)
            + static_cast<int32>(InputActionGuid.C)
            + static_cast<int32>(InputActionGuid.D);
    }
};
