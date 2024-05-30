// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "RecoilPatternAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class RECOILSYSTEMEDITOR_API URecoilPatternAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	URecoilPatternAssetFactory(const FObjectInitializer& ObjectInitializer);
	
public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
		EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// Tooltips that pop up when creating in the editor
	virtual FText GetToolTip() const override;
};
