// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGen/DungeonGridUtils.h"

TArray<FInt32Vector2> UDungeonGridUtils::CollectAffectedCells(const FInt32Vector2& a_Origin, const FInt32Vector2& a_Size)
{
	TArray<FInt32Vector2> affectedCells = {};
	for (int i = a_Origin.X; i < a_Origin.X + a_Size.X; i++)
	{
		for (int j = a_Origin.Y; j < a_Origin.Y + a_Size.Y; j++)
		{
			affectedCells.Add(FInt32Vector2(i, j));
		}
	}
	return affectedCells;
}

void UDungeonGridUtils::ChangeCellsInGrid(const TArray<FInt32Vector2>& a_Cells, const ECellType& a_CellType, TArray<TArray<ECellType>>& a_GridToChange)
{
	for (int i = 0; i < a_Cells.Num(); i++)
	{
		if (a_GridToChange[a_Cells[i].X][a_Cells[i].Y] != ECellType::EMPTY) continue;
		a_GridToChange[a_Cells[i].X][a_Cells[i].Y] = a_CellType;
	}
}
