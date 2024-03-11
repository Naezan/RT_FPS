// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Logging/LogMacros.h"

RT_FPS_API DECLARE_LOG_CATEGORY_EXTERN(LogRF, Log, All);

class UObject;
RT_FPS_API FString GetClientServerContextString(UObject* ContextObject = nullptr);