// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/IntVector.h"
#include "RFBuildingGenerator.generated.h"

class ARFBuildingGenerator;
class UStaticMesh;

UENUM()
enum EDir
{
	DIR_F, 
	DIR_B, 
	DIR_L, 
	DIR_R, 
	DIR_T, 
	DIR_BT, 
	DIR_MAX
};

UENUM()
enum EBlockEdgeType
{
	BLOCK_PLANE_NONE,
	BLOCK_PLANE_Full,    // ㅁ
	BLOCK_PLANE_H,  // ㅐ .
	BLOCK_PLANE_HF, // . ㅐ
	BLOCK_PLANE_HB, // ㅛ
	BLOCK_PLANE_HBF, // ㅠ

	BLOCK_PLANE_I,  // | .
	BLOCK_PLANE_IF, // . |
	BLOCK_PLANE_IB, // bottom ─
	BLOCK_PLANE_IBF, // top ─

	BLOCK_PLANE_L,  // └
	BLOCK_PLANE_LF, // ┘
	BLOCK_PLANE_LB,  // ┌
	BLOCK_PLANE_LBF, // ┐
};

USTRUCT(BlueprintType)
struct FBlockRuleData
{
	GENERATED_BODY()

public:
	FBlockRuleData()
	{
		DirectionBlockEdge.Add(DIR_F, BLOCK_PLANE_NONE);
		DirectionBlockEdge.Add(DIR_B, BLOCK_PLANE_NONE);
		DirectionBlockEdge.Add(DIR_L, BLOCK_PLANE_NONE);
		DirectionBlockEdge.Add(DIR_R, BLOCK_PLANE_NONE);
		DirectionBlockEdge.Add(DIR_T, BLOCK_PLANE_NONE);
		DirectionBlockEdge.Add(DIR_BT, BLOCK_PLANE_NONE);
	}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<TEnumAsByte<EDir>, TEnumAsByte<EBlockEdgeType>> DirectionBlockEdge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UStaticMesh*> Meshes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FEdgeRuleData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TEnumAsByte<EBlockEdgeType>> EdgeList;
};

USTRUCT(BlueprintType)
struct FBlock
{
	GENERATED_BODY()

public:
	FBlock()
	{
		Owner = nullptr;
		Entropy = Possibilities.Num();
	}

	FBlock(ARFBuildingGenerator* InOwner);

public:
	void AddNeighbour(EDir InDir, FBlock* Block)
	{
		if (!Neighbours.Contains(InDir))
		{
			Neighbours.Add(InDir, Block);
		}
	}
	FBlock* GetNeighbour(EDir InDir)
	{
		return Neighbours[InDir];
	}

	TArray<TEnumAsByte<EDir>> GetDirections() const
	{
		TArray<TEnumAsByte<EDir>> OutDirection;
		Neighbours.GetKeys(OutDirection);

		return OutDirection;
	}

	const TArray<FString>& GetPossibilities() const
	{
		return Possibilities;
	}

	void Collapse();
	FString RandomChoice(const TArray<int32>& Weights);
	bool Constrain(const TArray<FString>& NeighbourPossibilities, EDir InDir);

public:
	UPROPERTY()
	TArray<FString> Possibilities;
	UPROPERTY()
	int32 Entropy;
	TMap<TEnumAsByte<EDir>, FBlock*> Neighbours;
	UPROPERTY()
	ARFBuildingGenerator* Owner;
};

UCLASS()
class RT_FPS_API ARFBuildingGenerator : public AActor
{
	GENERATED_BODY()
	
	friend struct FBlock;
public:
	ARFBuildingGenerator();

	UFUNCTION(BlueprintCallable)
	void SetDimention(int32 X, int32 Y, int32 Z);
	UFUNCTION(BlueprintCallable)
	void InitializeBlock();
	UFUNCTION(BlueprintCallable)
	bool WaveFunctionCollapse();
	UFUNCTION(BlueprintCallable)
	void GenerateBlock();

private:
	int32 GetEntropy(int32 X, int32 Y, int32 Z);
	FString GetBlockType(int32 X, int32 Y, int32 Z);
	int32 GetLowestEntropy();
	TArray<FBlock*> GetLowestEntropyBlocks();
	
public:
	// Creating Building GridData -> X : Width, Y : Depth, Z : Height
	FIntVector3 Dimention;
	TArray<TArray<TArray<FBlock>>> Blocks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FBlockRuleData> BlockRules;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
	TMap<FString, int32> BlockWeights;
	//서로 붙을 수 있는 블럭 엣지 타입모음
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<TEnumAsByte<EBlockEdgeType>, FEdgeRuleData> CoexistEdgeRules;
};

template<typename T>
class TStack
{
public:
	TStack() {}

	void Push(const T& Item)
	{
		Items.Add(Item);
	}

	T Pop()
	{
		if (Items.Num() == 0)
		{
			return T();
		}
		else
		{
			T TopItem = Items.Last();
			Items.RemoveAt(Items.Num() - 1);
			return TopItem;
		}
	}

	T Top() const
	{
		if (Items.Num() == 0)
		{
			return T();
		}
		else
		{
			return Items.Last();
		}
	}

	bool IsEmpty() const
	{
		return Items.Num() == 0;
	}

	int32 Size() const
	{
		return Items.Num();
	}

private:
	TArray<T> Items;
};