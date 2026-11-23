#include "Modules/ModuleManager.h"

class FPLUGIN_NAMEModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override {}
    virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FPLUGIN_NAMEModule, PLUGIN_NAME)
