#pragma once

#include "UObject/Interface.h"
#include "PLUGIN_NAMEInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class PLUGIN_NAME_API UPLUGIN_NAMEInterface : public UInterface
{
    GENERATED_BODY()
};

class PLUGIN_NAME_API IPLUGIN_NAMEInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Framework")
    void OnFrameworkEvent();
};
