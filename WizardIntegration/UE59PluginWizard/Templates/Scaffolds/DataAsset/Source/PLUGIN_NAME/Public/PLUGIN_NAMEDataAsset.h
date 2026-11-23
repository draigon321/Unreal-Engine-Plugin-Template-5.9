#pragma once

#include "Engine/DataAsset.h"
#include "PLUGIN_NAMEDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PLUGIN_NAME_API UPLUGIN_NAMEDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Framework")
    FText DisplayName;
};
