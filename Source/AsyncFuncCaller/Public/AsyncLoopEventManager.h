#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
//#include "Kismet/BlueprintAsyncActionBase.h"
#include <mutex>
//#include "RunnableThreadWorker.h"
//#include "AsyncBpLibrary.h"

//#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsyncLoopEventManager.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextureLoaderDelegate, UTexture2D*, texture);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTextureLoaderFailDelegate);

//USTRUCT(BlueprintType)
//struct FTextureLoaderInfo
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "如果爱|FileUtility")
//	FString Guid;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "如果爱|FileUtility")
//	FString FilePath;
//
//	FTextureLoaderInfo() {
//		Guid = FString(TEXT(""));
//		FilePath = FString(TEXT(""));
//	}
//	FTextureLoaderInfo(FString _path, FString _guid) {
//		Guid = _guid; 
//		FilePath = _path;
//	}
//};

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
		FGameThreadEvtCallback GameThreadEvent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AsyncFuncCallerWithOption")
		FAsyncFuncCallback FinishEvent;
};

//static TArray<int32> DefaultIntArray = {};
//TArray<int32> DefaultIntArray = {};
//TArray<int32> DefaultIntArray = {};

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
		//void ExecuteGameThreadEvent(const FRunnabelCommonDataType& data);
		void ExecuteGameThreadEvent(const TArray<int32>& intArray, const TArray<float>& floatArray, const TArray<FString>& StringArray);
	UFUNCTION(BlueprintCallable, Category = "AsyncFuncWithOption")
		void ExecuteCriticalEvent();
};


UCLASS(Blueprintable)
class UAsyncLoopEventManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BLueprintCallable, Category = "AsyncLoopEventManager")
	static UAsyncLoopEventManager* CreateAAsyncLoopEventManager();

	std::timed_mutex data_mutex;

	UFUNCTION(BlueprintCallable, Category = "AsyncLoopEventManager")
	URunnableThreadCaller* StartAsyncLoopEvent(const FRunnableThreadOption& _option);

	//UPROPERTY(BlueprintAssignable)
	//FTextureLoaderDelegate OnComplete;
	//UPROPERTY(BlueprintAssignable)
	//FTextureLoaderFailDelegate OnFail;


};