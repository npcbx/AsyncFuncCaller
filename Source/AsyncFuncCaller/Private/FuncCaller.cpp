
#include "FuncCaller.h"
#include "FuncCallerWorker.h"

UFuncCaller* UFuncCaller::StartAsyncFunc(FString FuncName, UObject* Target) {
	UFuncCaller * loader = NewObject<UFuncCaller>();

	FString ThreadName = FString::Printf(TEXT("FuncCallerThreadWorkder Thread:"));
	ThreadName.AppendInt(FAsyncThreadIndex::GetNext());

	FuncCallFinishDelegate LoadFinishHandler;
	LoadFinishHandler.BindUObject(loader, &UFuncCaller::OnFinishCallback);


	FRunnableThread::Create(new FFuncCallerThreadWorker(FuncName, Target, LoadFinishHandler), *ThreadName);

	return loader;
}

void UFuncCaller::OnFinishCallback(UObject* obj, bool Result) {
	if (Result)
	{
		OnComplete.Broadcast(obj);
	}
	else {
		OnFail.Broadcast();
	}
}
