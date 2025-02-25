// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadState.h"

void DeadState::Enter(UActionComponent* ActionComponent)
{
}

void DeadState::Tick(UActionComponent* ActionComponent, float DeltaTime)
{
}

void DeadState::Exit(UActionComponent* ActionComponent)
{
}

bool DeadState::CanChangeState(ECharacterStateType CharacterStateType)
{
	return false;
}

const FString DeadState::GetStateToString()
{
	return "Dead";
}

bool DeadState::CanMove() const
{
	return false;
}
