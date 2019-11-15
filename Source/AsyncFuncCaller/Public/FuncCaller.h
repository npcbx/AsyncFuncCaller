#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "FuncCaller.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFuncCallSuccessDelegate, UObject*, obj);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFuncCallFailDelegate);



UCLASS(Blueprintable)
class ASYNCFUNCCALLER_API UFuncCaller : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BLueprintCallable, Category = "如果爱|FileUtility", meta = (BlueprintInternalUseOnly = "true"))
	static UFuncCaller* StartAsyncFunc(FString FuncName, UObject* Target);

	UPROPERTY(BlueprintAssignable)
	FFuncCallSuccessDelegate OnComplete;
	UPROPERTY(BlueprintAssignable)
	FFuncCallFailDelegate OnFail;

	void OnFinishCallback(UObject* obj, bool Result);
};