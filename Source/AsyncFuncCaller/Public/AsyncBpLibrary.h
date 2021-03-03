// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <thread>
#include "HAL/Runnable.h"
//#include <function>
#include "AsyncBpLibrary.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FAsyncFuncCallback, const TArray<UObject*>&, objs);
//DECLARE_DYNAMIC_DELEGATE(FAsyncFuncCallback);

//USTRUCT(BlueprintType)
//struct FRunnabelCommonDataType
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
//		TArray<int32> ints = {};
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
//		TArray<int32> floats = {};
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncWithOption")
//		TArray<FString> strings = {};
//};

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGameThreadEvtCallback, const TArray<int32>&, intArray, const TArray<float>&, floatArray, const TArray<FString>&, StringArray);
//DECLARE_DYNAMIC_DELEGATE(FGameThreadEvtCallback);



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


		if (WorkingThread)
		{
			//std::pthread_cancel(WorkingThread.native_handle());
			//WorkingThread.join();

			delete WorkingThread;
			WorkingThread = nullptr;
		}
	}
public:

	//TSharedPtr<TPromise<void>, ESPMode::ThreadSafe> promise1;
	//TSharedPtr<TPromise<void>, ESPMode::ThreadSafe> promise2;
	FAsyncFuncOption option;

	std::thread* WorkingThread = nullptr;
	//TSharedPtr<std::thread> WorkingThread;

	void DoThread() {
		AddToRoot();
		UAsyncFuncWorker* worker = this;

		auto AsyncFunction = [=]() {
			option.AsyncEvent.ExecuteIfBound(option.objs);
			if (worker && worker->IsValidLowLevel())
			{
				AsyncTask(ENamedThreads::GameThread, [=] {
					if (worker && worker->IsValidLowLevel())
					{
						option.FinishEvent.ExecuteIfBound(option.objs);
						worker->RemoveFromRoot();
					}
					//promise2->SetValue();
				});
			}
			//promise1->SetValue();		
		};

		//WorkingThread = MakeShareable(new std::thread(AsyncFunction));
		WorkingThread = new std::thread(AsyncFunction);
		WorkingThread->detach();
	}
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		void ExecuteGameThreadEventOnGameThread()
	{
		UAsyncFuncWorker* worker = this;
		if (worker && worker->IsValidLowLevel())
		{
			AsyncTask(ENamedThreads::GameThread, [=] {
				option.GameThreadEvent.ExecuteIfBound(option.objs);
			});
		}
	}

};



UCLASS()
class UAsyncBpLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	static UAsyncFuncWorker* StartAsyncFuncWithOption(const FAsyncFuncOption& _option);

	//UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	//static URunnableThreadCaller* StartRunnableThreadWithOption(const FRunnableThreadOption& _option);

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
	static int32 GetNowThreadId();

	UFUNCTION(Blueprintpure, Category = "AsyncFuncWithOption")
	static int32 GetHardWareConcurrencyNum();

	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		static void MakeCurrentThreadSleep(float seconds);

	UFUNCTION(BlueprintCallable, Category = "MeshExporter")
		static void HoldUObject(UObject* obj);
	UFUNCTION(BlueprintCallable, Category = "MeshExporter")
		static void ReleaseUObject(UObject* obj);
};
