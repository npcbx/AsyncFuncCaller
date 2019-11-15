// Fill out your copyright notice in the Description page of Project Settings.

#include "FuncCallerWorker.h"
#include "FuncCaller.h"

//Init
bool FFuncCallerThreadWorker::Init()
{
	return true;
}
//Run
uint32 FFuncCallerThreadWorker::Run() {
	if (Target)
	{
		FOutputDeviceNull DymmyOutputDevice;
		Target->CallFunctionByNameWithArguments(*FuncName, DymmyOutputDevice, Target, true);
	}
	else {
		CallResult = false;
	}

	
	return 0;
}

void FFuncCallerThreadWorker::Exit()
{
	FuncCallFinishDelegate Handler = FuncCallFinishHandler;
	UObject * outPut = Target;
	bool Result = CallResult;
	
	AsyncTask(ENamedThreads::GameThread, [Handler, outPut, Result]() {
		// code to execute on game thread here
		Handler.ExecuteIfBound(outPut, Result);
	});
}

