// Fill out your copyright notice in the Description page of Project Settings.


#include "PCG/RFBuildingGenerator.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ARFBuildingGenerator::ARFBuildingGenerator()
{
}

void ARFBuildingGenerator::SetDimention(int32 X, int32 Y, int32 Z)
{
	Dimention.X = X;
	Dimention.Y = Y;
	Dimention.Z = Z;
}

void ARFBuildingGenerator::InitializeBlock()
{
	Blocks.Init(TArray<TArray<FBlock>>(), Dimention.Z);
	for (int32 Z = 0; Z < Dimention.Z; ++Z)
	{
		Blocks[Z].Init(TArray<FBlock>(), Dimention.Y);
		for (int32 Y = 0; Y < Dimention.Y; ++Y)
		{
			Blocks[Z][Y].Init(FBlock(this), Dimention.X);
		}
	}

	for (int32 Z = 0; Z < Dimention.Z; ++Z)
	{
		for (int32 Y = 0; Y < Dimention.Y; ++Y)
		{
			for (int32 X = 0; X < Dimention.X; ++X)
			{
				FBlock& Block = Blocks[Z][Y][X];

				if (X > 0)
					Block.AddNeighbour(EDir::DIR_B, &Blocks[Z][Y][X - 1]);
				if (X < Dimention.X - 1)
					Block.AddNeighbour(EDir::DIR_F, &Blocks[Z][Y][X + 1]);
				if (Y > 0)
					Block.AddNeighbour(EDir::DIR_L, &Blocks[Z][Y - 1][X]);
				if (Y < Dimention.Y - 1)
					Block.AddNeighbour(EDir::DIR_R, &Blocks[Z][Y + 1][X]);
				if (Z > 0)
					Block.AddNeighbour(EDir::DIR_BT, &Blocks[Z - 1][Y][X]);
				if (Z < Dimention.Z - 1)
					Block.AddNeighbour(EDir::DIR_T, &Blocks[Z + 1][Y][X]);
			}
		}
	}
}

bool ARFBuildingGenerator::WaveFunctionCollapse()
{
	TArray<FBlock*> EntropyBlocks = GetLowestEntropyBlocks();

	if (EntropyBlocks.IsEmpty())
	{
		// Select Random Bottom Block
		EntropyBlocks.Add(&Blocks[0][FMath::RandRange(0, Dimention.Y - 1)][FMath::RandRange(0, Dimention.X - 1)]);
	}

	int32 Index = FMath::RandRange(0, EntropyBlocks.Num() - 1);
	FBlock* BlockToCollapse = EntropyBlocks[Index];
	BlockToCollapse->Collapse();

	TStack<FBlock*> Stack;
	Stack.Push(BlockToCollapse);

	while (!Stack.IsEmpty())
	{
		FBlock* Block = Stack.Pop();
		TArray<FString> PossibilityBlocks = Block->GetPossibilities();
		TArray<TEnumAsByte<EDir>> Directions = Block->GetDirections();
		for (auto Direction : Directions)
		{
			FBlock* Neighbour = Block->GetNeighbour(Direction);
			if (Neighbour->Entropy != 0)
			{
				// �̿��� �� �� �˸��� ���� ������Ʈ ����
				bool Reduced = Neighbour->Constrain(PossibilityBlocks, Direction);
				if (Reduced)
				{
					Stack.Push(Neighbour);
				}
			}
		}
	}

	return true;
}

void ARFBuildingGenerator::GenerateBlock()
{
	for (int32 Z = 0; Z < Dimention.Z; ++Z)
	{
		for (int32 Y = 0; Y < Dimention.Y; ++Y)
		{
			for (int32 X = 0; X < Dimention.X; ++X)
			{
				if (Blocks[Z][Y][X].Possibilities.Num() > 0)
				{
					FString BlockType = Blocks[Z][Y][X].Possibilities[0];

					if (BlockRules[BlockType].Meshes.Num() > 0)
					{
						int32 RandomIndex = FMath::RandRange(0, BlockRules[BlockType].Meshes.Num() - 1);
						UStaticMesh* BlockMesh = BlockRules[BlockType].Meshes[RandomIndex];

						if (BlockMesh)
						{
							FTransform SpawnTransform(FRotator::ZeroRotator, FVector(X * 300, Y * 300, Z * 300));
							AStaticMeshActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnTransform);
							SpawnedActor->FinishSpawning(SpawnTransform);
							SpawnedActor->GetStaticMeshComponent()->SetStaticMesh(BlockMesh);
						}
					}
				}
			}
		}
	}
}

int32 ARFBuildingGenerator::GetEntropy(int32 X, int32 Y, int32 Z)
{
	return Blocks[Z][Y][X].Entropy;
}

FString ARFBuildingGenerator::GetBlockType(int32 X, int32 Y, int32 Z)
{
	return Blocks[Z][Y][X].Possibilities[0];
}

int32 ARFBuildingGenerator::GetLowestEntropy()
{
	int32 LowestEntropy = BlockRules.Num();

	for (int32 Z = 0; Z < Dimention.Z; ++Z)
	{
		for (int32 Y = 0; Y < Dimention.Y; ++Y)
		{
			for (int32 X = 0; X < Dimention.X; ++X)
			{
				int32 BlockEntropy = Blocks[Z][Y][X].Entropy;

				if (BlockEntropy > 0)
				{
					if (BlockEntropy < LowestEntropy)
						LowestEntropy = BlockEntropy;
				}
			}
		}
	}

	return LowestEntropy;
}

TArray<FBlock*> ARFBuildingGenerator::GetLowestEntropyBlocks()
{
	int32 LowestEntropy = BlockRules.Num();
	TArray<FBlock*> BlockList;

	for (int32 Z = 0; Z < Dimention.Z; ++Z)
	{
		for (int32 Y = 0; Y < Dimention.Y; ++Y)
		{
			for (int32 X = 0; X < Dimention.X; ++X)
			{
				int32 BlockEntropy = Blocks[Z][Y][X].Entropy;

				if (BlockEntropy > 0)
				{
					if (BlockEntropy < LowestEntropy)
					{
						BlockList.Empty();
						LowestEntropy = BlockEntropy;
					}
					if (BlockEntropy == LowestEntropy)
					{
						BlockList.Add(&Blocks[Z][Y][X]);
					}
				}
			}
		}
	}

	return BlockList;
}

FBlock::FBlock(ARFBuildingGenerator* InOwner)
{
	Owner = InOwner;

	Possibilities.Empty();
	Owner->BlockRules.GetKeys(Possibilities);
	Entropy = Possibilities.Num();
}

void FBlock::Collapse()
{
	TArray<int32> Weights;
	for (int32 i = 0; i < Owner->BlockRules.Num(); ++i)
	{
		FString Key = Possibilities[i];
		FString RealKey = FString();
		RealKey.AppendChar(Key[0]);

		// 0 -> 0, 1 -> 1, i + ���� -> i, l + ���� -> l, y + ���� -> y
		// l + ���ĺ� -> la, y + �˹ٺ� -> ya
		if (Key.IsValidIndex(1) && FChar::IsAlpha(Key[1]))
		{
			RealKey.AppendChar('a');
		}

		Weights.Add(Owner->BlockWeights[RealKey]);
	}

	FString BlockType = RandomChoice(Weights);
	Possibilities.Empty();
	Possibilities.Add(BlockType);
	Entropy = 0;
}

FString FBlock::RandomChoice(const TArray<int32>& Weights)
{
	float TotalWeight = 0.f;
	for (float Weight : Weights)
	{
		TotalWeight += Weight;
	}

	float RandomValue = FMath::FRandRange(0.f, TotalWeight);

	float WeightSum = 0.f;
	for (int32 i = 0; i < Possibilities.Num(); ++i)
	{
		WeightSum += Weights[i];
		if (RandomValue <= WeightSum)
		{
			return Possibilities[i];
		}
	}

	return Possibilities.Last();
}

bool FBlock::Constrain(const TArray<FString>& NeighbourPossibilities, EDir InDir)
{
	bool bReduced = false;

	if (Entropy > 0)
	{
		TArray<TEnumAsByte<EBlockEdgeType>> ConnectorEdgeTypes;
		// NeighbourPossibilities�� �̿����� ǥ��Ǿ� ������ �����δ� �̹� Constrain�� ���� �ٷ� �� ���� ������ ������Ʈ
		for (const auto& NeighbourPosibility : NeighbourPossibilities)
		{
			EBlockEdgeType EdgeType = Owner->BlockRules[NeighbourPosibility].DirectionBlockEdge[InDir];
			ConnectorEdgeTypes.Add(EdgeType);
		}

		EDir OppositeDir = EDir::DIR_MAX;
		OppositeDir = InDir == EDir::DIR_F ? EDir::DIR_B : OppositeDir;
		OppositeDir = InDir == EDir::DIR_B ? EDir::DIR_F : OppositeDir;
		OppositeDir = InDir == EDir::DIR_L ? EDir::DIR_R : OppositeDir;
		OppositeDir = InDir == EDir::DIR_R ? EDir::DIR_L : OppositeDir;
		OppositeDir = InDir == EDir::DIR_T ? EDir::DIR_BT : OppositeDir;
		OppositeDir = InDir == EDir::DIR_BT ? EDir::DIR_T : OppositeDir;

		// �������� ���� ����.
		if (OppositeDir == EDir::DIR_MAX)
		{
			return bReduced;
		}

		// �� Ÿ�Ը���Ʈ(������ �����ϰ����ϴ� �̿��� ��Ÿ�� ����Ʈ)
		TArray<FString> CopyPossibilities(Possibilities);
		for (const auto Possibility : CopyPossibilities)
		{
			// ���ֺ��� ������ �� Ÿ��(�����ϰ����ϴ� �̿��� �� ���� Ÿ��)
			EBlockEdgeType OppositeEdgeType = Owner->BlockRules[Possibility].DirectionBlockEdge[OppositeDir];

			//���� OppositeEdgeType�� �� �� �ִ� ����Ʈ�� ConnectorEdgeTypes�� ���ٸ� ����
			bool bIscoexist = false;
			for (const auto EdgeType : ConnectorEdgeTypes)
			{
				if (bIscoexist)
				{
					break;
				}

				if (!bIscoexist && Owner->CoexistEdgeRules[EdgeType].EdgeList.Contains(OppositeEdgeType))
				{
					bIscoexist = true;
				}
			}
			
			if (!bIscoexist)
			{
				Possibilities.Remove(Possibility);
				bReduced = true;
			}
		}

		Entropy = Possibilities.Num();
	}

	return bReduced;
}
