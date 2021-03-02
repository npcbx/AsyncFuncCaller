#include "AsyncLoopEventManager.h"




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


//void URunnableThreadCaller::ExecuteGameThreadEvent(const FRunnabelCommonDataType& data)
void URunnableThreadCaller::ExecuteGameThreadEvent(const TArray<int32>& intArray, const TArray<float>& floatArray, const TArray<FString>& StringArray)
{
	//UE_LOG(LogTemp, Log, TEXT("URunnableThreadCaller::ExecuteGameThreadEvent,int arr:%d, float arr:%d, string arr:%d, "), intArray.Num(), floatArray.Num(), StringArray.Num());
	FRunnableThreadWorker * threadWorker = (FRunnableThreadWorker*)ThreadRunner;
	if (threadWorker)
	{
		threadWorker->RunEventOnGameThread(intArray, floatArray, StringArray);
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


UAsyncLoopEventManager* UAsyncLoopEventManager::CreateAAsyncLoopEventManager()
{
	UAsyncLoopEventManager* manager = NewObject<UAsyncLoopEventManager>();
	manager->AddToRoot();
	return manager;
}
URunnableThreadCaller* UAsyncLoopEventManager::StartAsyncLoopEvent(const FRunnableThreadOption& _option)
{
	FString ThreadName = FString::Printf(TEXT("AsyncLoopEventThread:"));
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());

	URunnableThreadCaller * caller = NewObject<URunnableThreadCaller>();
	caller->ThreadRunner = new FRunnableThreadWorker(_option, this);
	caller->RunningThread = FRunnableThread::Create(caller->ThreadRunner, *ThreadName);
	return caller;
}


//
//void UTextureLoader::OnFinishCallback(UTexture2D* tex, bool Result, FString FilePath, FString Guid) {
//	if (Result)
//	{
//		if (TextureLoaderSingleton::getInstance()) {
//			TextureLoaderSingleton::getInstance()->TextureInfoMap.Add(tex, FTextureLoaderInfo(FilePath, Guid));
//			TextureLoaderSingleton::getInstance()->PathTextureMap.Add(FilePath, tex);
//		}
//		OnComplete.Broadcast(tex);
//	}
//	else {
//		OnFail.Broadcast();
//	}
//}

