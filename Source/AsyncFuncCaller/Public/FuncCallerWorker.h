// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Hujiachuan. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE_TwoParams(FuncCallFinishDelegate, UObject*, bool);

class ASYNCFUNCCALLER_API FFuncCallerThreadWorker : public FRunnable
{
public:
	//construct func
	FFuncCallerThreadWorker(FString _funcName, UObject* _target, FuncCallFinishDelegate _funcCallFinishHandler):
		FuncName(_funcName), Target(_target), FuncCallFinishHandler(_funcCallFinishHandler)
	{

	}

	//thread work
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

private:
	FuncCallFinishDelegate FuncCallFinishHandler;
	FString FuncName;
	UObject * Target;
	bool CallResult = true;
};

