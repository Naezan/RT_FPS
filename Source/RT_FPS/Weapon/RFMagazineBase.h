// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MagazineInterface.h"
#include "RFMagazineBase.generated.h"

UCLASS()
class RT_FPS_API ARFMagazineBase : public AActor, public IMagazineInterface
{
	GENERATED_BODY()
	
public:	
	ARFMagazineBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void OnAllAmmoConsumed() override;

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MagMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMesh* EmptyMagMesh;
};
