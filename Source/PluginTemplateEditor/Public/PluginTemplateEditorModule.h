#pragma once

#include "Modules/ModuleManager.h"

class FPluginTemplateEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
