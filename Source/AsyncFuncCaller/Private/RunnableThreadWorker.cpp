// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
// Copyright 2019 Hujiachuan. All Rights Reserved.


#include "RunnableThreadWorker.h"
#include "Async/Async.h"
#include "AsyncLoopEventManager.h"
//#include <mutex>
//std::timed_mutex data_tmutex;
//Init
bool FRunnableThreadWorker::Init()
{
	UE_LOG(LogTemp, Log, TEXT("FRunnableThreadWorker::Init "));
	return true;
}
//Run
uint32 FRunnableThreadWorker::Run() {
	
	while (!IfEndWork.load()) {
		//UE_LOG(LogTemp, Log, TEXT("FRunnableThreadWorker::run step2, thread:%d "), GetCurrentThreadId());
		//if (AsyncFuncCallerOption.IfNeedPrepare)
		//{
		//	std::lock_guard<std::timed_mutex> lockg(data_tmutex);
		//	AsyncFuncCallerOption.UpdatePrepareEvent.ExecuteIfBound(AsyncFuncCallerOption.objs);
		//}
		if (IfPauseWork.load())
		{
			FPlatformProcess::Sleep(0.1f);
			continue;
		}
		AsyncFuncCallerOption.AsyncEvent.ExecuteIfBound(AsyncFuncCallerOption.objs);
		//FPlatformProcess::Sleep(0.02f);
	}
	return 0;
}

void FRunnableThreadWorker::Exit()
{
	//FuncCallFinishDelegate Handler = FuncCallFinishHandler;
	//UObject * outPut = Target;
	//bool Result = CallResult;
	//
	AsyncTask(ENamedThreads::GameThread, [=]() {
		//AsyncFuncCallerOption..ExecuteIfBound(AsyncFuncCallerOption.objs);
		AsyncFuncCallerOption.FinishEvent.ExecuteIfBound(AsyncFuncCallerOption.objs);
	});
}

void FRunnableThreadWorker::RunCiticalEvent()
{
	//UAsyncLoopEventManager * _manager = (UAsyncLoopEventManager*)(Manager);
	if (Manager)
	{
		std::lock_guard<std::timed_mutex> lockg(Manager->data_mutex);
		AsyncFuncCallerOption.CriticalEvent.ExecuteIfBound(AsyncFuncCallerOption.objs);
	}
	
}