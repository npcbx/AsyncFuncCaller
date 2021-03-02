// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Hujiachuan. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "AsyncBpLibrary.h"
#include <atomic>
#include "AsyncLoopEventManager.h"

DECLARE_DELEGATE_TwoParams(AsyncFuncCallFinishDelegate, UObject*, bool);


class FRunnableThreadWorker : public FRunnable
{
public:
	//construct func
	FRunnableThreadWorker(const FRunnableThreadOption& _AsyncFuncCallerOption, UAsyncLoopEventManager* _manager):
		AsyncFuncCallerOption(_AsyncFuncCallerOption), Manager(_manager)
	{
		IfPauseWork.store(false);
		IfEndWork.store(false);
	}

	//thread work
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	std::atomic<bool> IfPauseWork;
	std::atomic<bool> IfEndWork;
	void PauseWork() {
		if (!IfPauseWork.load()) {
			IfPauseWork.store(true);
		}
	}
	void ResumeWork()
	{
		if (IfPauseWork.load()) {
			IfPauseWork.store(false);
		}
	}
	void EndWork()
	{
		if (!IfEndWork.load()) {
			IfEndWork.store(true);
		}
	}
	//void RunEventOnGameThread(const FRunnabelCommonDataType& data)
	void RunEventOnGameThread(const TArray<int32>& intArray, const TArray<float>& floatArray, const TArray<FString>& StringArray)
	{
		AsyncTask(ENamedThreads::GameThread, [=] {
			//AsyncFuncCallerOption.GameThreadEvent.ExecuteIfBound(data);
			AsyncFuncCallerOption.GameThreadEvent.ExecuteIfBound(intArray, floatArray, StringArray);
		});
	}
	void RunCiticalEvent();
private:
	FRunnableThreadOption AsyncFuncCallerOption;
	UAsyncLoopEventManager* Manager;
};

