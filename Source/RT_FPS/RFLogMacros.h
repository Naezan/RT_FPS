// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Logging/LogMacros.h"

RT_FPS_API DECLARE_LOG_CATEGORY_EXTERN(LogRF, Log, All);

class UObject;
RT_FPS_API FString GetClientServerContextString(UObject* ContextObject = nullptr);

/* Reference */

//Current Class Name + Function Name where this is called!
#define RF_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define RF_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define RF_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define RF_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define RF_CUR_CLASS_LINE (RF_CUR_CLASS + "(" + RF_CUR_LINE + ")")

//Current Function Signature where this is called!
#define RF_CUR_FUNCSIG (FString(__FUNCSIG__))

/* ~Reference : https://unrealcommunity.wiki/6100e80c9c9d1a89e0c2fbcc */