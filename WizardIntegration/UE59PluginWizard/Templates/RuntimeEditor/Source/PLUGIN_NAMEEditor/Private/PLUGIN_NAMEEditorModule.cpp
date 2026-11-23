#include "Modules/ModuleManager.h"

class FPLUGIN_NAMEEditorModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FPLUGIN_NAMEEditorModule, PLUGIN_NAMEEditor)
