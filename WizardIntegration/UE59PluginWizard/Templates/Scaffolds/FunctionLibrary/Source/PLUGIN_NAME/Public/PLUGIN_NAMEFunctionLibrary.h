#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PLUGIN_NAMEFunctionLibrary.generated.h"

UCLASS()
class PLUGIN_NAME_API UPLUGIN_NAMEFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category="Framework")
    static bool IsFrameworkObjectValid(const UObject* Object);
};
