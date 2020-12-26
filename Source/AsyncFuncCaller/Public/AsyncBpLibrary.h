// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <thread>
#include "HAL/Runnable.h"
#include "AsyncBpLibrary.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FAsyncFuncCallback, const TArray<UObject*>&, objs);
//DECLARE_DYNAMIC_DELEGATE(FAsyncFuncCallback);


USTRUCT(BlueprintType)
struct FAsyncFuncOption
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
		TArray<UObject*> objs = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
		FAsyncFuncCallback AsyncEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
		FAsyncFuncCallback GameThreadEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
		FAsyncFuncCallback FinishEvent;
};


USTRUCT(BlueprintType)
struct FRunnableThreadOption
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		TArray<UObject*> objs = {};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		FAsyncFuncCallback CriticalEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		FAsyncFuncCallback AsyncEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		FAsyncFuncCallback GameThreadEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		FAsyncFuncCallback FinishEvent;
};


UCLASS(Blueprintable)
class URunnableThreadCaller : public UObject
{
	GENERATED_BODY()
public:
	URunnableThreadCaller() {}
	~URunnableThreadCaller() {
		
		if (RunningThread)
		{
			RunningThread->WaitForCompletion();
		}
		if (ThreadRunner)
		{
			delete ThreadRunner;
			ThreadRunner = nullptr;
		}
	}

	FRunnableThread * RunningThread;
	FRunnable * ThreadRunner;

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncCallerWithOption")
	void PauseThread();
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncCallerWithOption")
	void ResumeThread();
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncCallerWithOption")
	void EndThread();
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		void ExecuteGameThreadEvent();
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		void ExecuteCriticalEvent(); 
};


UCLASS(Blueprintable)
class UAsyncFuncWorker : public UObject
{
	GENERATED_BODY()
public:
	UAsyncFuncWorker() {
		//promise1 = MakeShared<TPromise<void>, ESPMode::ThreadSafe>();
		//promise2= MakeShared<TPromise<void>, ESPMode::ThreadSafe>();
	}
	UAsyncFuncWorker(const FAsyncFuncOption& opt) : option(opt) {}
	~UAsyncFuncWorker() {
		//if (promise1)
		//{
		//	promise1->GetFuture().Wait();
		//}
		//if (promise2)
		//{
		//	promise2->GetFuture().Wait();
		//}


		if (!WorkingThread)
		{
			delete WorkingThread;
			WorkingThread = nullptr;
		}
	}
public:

	//TSharedPtr<TPromise<void>, ESPMode::ThreadSafe> promise1;
	//TSharedPtr<TPromise<void>, ESPMode::ThreadSafe> promise2;
	FAsyncFuncOption option;
	std::thread* WorkingThread = nullptr;


	void DoThread() {
		AddToRoot();
		std::function<void()> AsyncFunction = [=]() {

			option.AsyncEvent.ExecuteIfBound(option.objs);
			UAsyncFuncWorker * worker = this;
			AsyncTask(ENamedThreads::GameThread, [=] {
				option.FinishEvent.ExecuteIfBound(option.objs);
				worker->RemoveFromRoot();
				//promise2->SetValue();
			});
			//promise1->SetValue();		
		};

		WorkingThread = new std::thread(AsyncFunction);
	}
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		void ExecuteGameThreadEventOnGameThread()
	{
		AsyncTask(ENamedThreads::GameThread, [=] {
			option.GameThreadEvent.ExecuteIfBound(option.objs);
		});
	}

};



UCLASS()
class UAsyncBpLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	static UAsyncFuncWorker* StartAsyncFuncWithOption(const FAsyncFuncOption& _option);

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	static URunnableThreadCaller* StartRunnableThreadWithOption(const FRunnableThreadOption& _option);

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	static int32 GetNowThreadId();

	UFUNCTION(Blueprintpure, Category = "AsyncFuncWithOption")
	static int32 GetHardWareConcurrencyNum();

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		static void MakeCurrentThreadSleep(float seconds);
};
