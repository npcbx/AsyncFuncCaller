// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Runnable.h"
#include "Async.h"

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

