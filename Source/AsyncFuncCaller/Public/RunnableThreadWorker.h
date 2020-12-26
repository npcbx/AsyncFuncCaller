// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Hujiachuan. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "AsyncBpLibrary.h"
#include <atomic>

DECLARE_DELEGATE_TwoParams(AsyncFuncCallFinishDelegate, UObject*, bool);

class FRunnableThreadWorker : public FRunnable
{
public:
	//construct func
	FRunnableThreadWorker(const FRunnableThreadOption& _AsyncFuncCallerOption):
		AsyncFuncCallerOption(_AsyncFuncCallerOption)
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
		IfPauseWork.store(true);
	}
	void ResumeWork()
	{
		IfPauseWork.store(false);
	}
	void EndWork()
	{
		IfEndWork.store(true);
	}
	void RunEventOnGameThread()
	{
		AsyncTask(ENamedThreads::GameThread, [=] {
			AsyncFuncCallerOption.GameThreadEvent.ExecuteIfBound(AsyncFuncCallerOption.objs);
		});
	}
	void RunCiticalEvent();
private:
	FRunnableThreadOption AsyncFuncCallerOption;
};

