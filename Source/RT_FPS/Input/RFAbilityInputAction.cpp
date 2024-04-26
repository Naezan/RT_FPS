// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/RFAbilityInputAction.h"

void URFAbilityInputAction::ResetInputID() const
{
	InputActionGuid.Invalidate();
}
