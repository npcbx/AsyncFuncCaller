// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncBpLibrary.h"
//#include<function>
#include "RunnableThreadWorker.h"

#include<functional>
#include<Async/Async.h>

#include "AllowWindowsPlatformTypes.h"
#include "windows.h"
#include "HideWindowsPlatformTypes.h"


void URunnableThreadCaller::PauseThread() 
{
	
	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->PauseWork();
	}

}

void URunnableThreadCaller::ResumeThread() 
{
	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->ResumeWork();
	}

}

void URunnableThreadCaller::EndThread() 
{

	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->EndWork();
	}
}


void URunnableThreadCaller::ExecuteGameThreadEvent()
{
	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->RunEventOnGameThread();
	}

}


void URunnableThreadCaller::ExecuteCriticalEvent()
{
	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->RunCiticalEvent();
	}

}

UAsyncFuncWorker* UAsyncBpLibrary::StartAsyncFuncWithOption(const FAsyncFuncOption& _option)
{
	UAsyncFuncWorker * asyncBpWorker = NewObject<UAsyncFuncWorker>();
	asyncBpWorker->option = _option;
	asyncBpWorker->AddToRoot();
	asyncBpWorker->DoThread();

	return asyncBpWorker;
}

URunnableThreadCaller* UAsyncBpLibrary::StartRunnableThreadWithOption(const FRunnableThreadOption& _option)
{

	//URunnableThreadCaller * loader = NewObject<URunnableThreadCaller>();

	FString ThreadName = FString::Printf(TEXT("AsyncRunnableThread:"));
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());

	URunnableThreadCaller * caller = NewObject<URunnableThreadCaller>();
	caller->ThreadRunner = new FRunnableThreadWorker(_option);
	caller->RunningThread = FRunnableThread::Create(caller->ThreadRunner, *ThreadName);
	return caller;
}

int32 UAsyncBpLibrary::GetNowThreadId()
{
	return GetCurrentThreadId();
}

int32 UAsyncBpLibrary::GetHardWareConcurrencyNum()
{
	return std::thread::hardware_concurrency();
}

void UAsyncBpLibrary::MakeCurrentThreadSleep(float seconds)
{
	FPlatformProcess::Sleep(seconds);
}