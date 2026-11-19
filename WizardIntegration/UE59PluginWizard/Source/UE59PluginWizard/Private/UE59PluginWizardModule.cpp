#include "Features/IPluginsEditorFeature.h"
#include "IPluginBrowser.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "PluginDescriptor.h"
#include "ModuleDescriptor.h"

#define LOCTEXT_NAMESPACE "FUE59PluginWizardModule"

class FRuntimeEditorTemplateDescription final : public FPluginTemplateDescription
{
public:
    using FPluginTemplateDescription::FPluginTemplateDescription;

    virtual void CustomizeDescriptorBeforeCreation(FPluginDescriptor& Descriptor) override
    {
        if (Descriptor.Modules.Num() > 0)
        {
            const FString RuntimeName = Descriptor.Modules[0].Name.ToString();
            Descriptor.Modules.Add(FModuleDescriptor(
                *FString(RuntimeName + TEXT("Editor")),
                EHostType::Editor,
                ELoadingPhase::Default));
        }
    }
};

class FUE59PluginWizardModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("UE59PluginWizard"));
        if (!Plugin.IsValid())
        {
            return;
        }

        const FString TemplateDirectory = FPaths::Combine(
            Plugin->GetBaseDir(), TEXT("Templates"), TEXT("RuntimeEditor"));

        Template = MakeShared<FRuntimeEditorTemplateDescription>(
            LOCTEXT("Name", "Runtime + Editor Framework"),
            LOCTEXT("Description", "A reusable Unreal Engine 5.9 plugin with separate runtime and editor C++ modules."),
            TemplateDirectory,
            true,
            EHostType::Runtime,
            ELoadingPhase::Default,
            false,
            FString(),
            FString()
        );

        if (FModuleManager::Get().IsModuleLoaded(TEXT("PluginBrowser")))
        {
            IPluginBrowser::Get().RegisterPluginTemplate(Template.ToSharedRef());
            bRegistered = true;
        }
    }

    virtual void ShutdownModule() override
    {
        if (bRegistered && Template.IsValid() &&
            FModuleManager::Get().IsModuleLoaded(TEXT("PluginBrowser")))
        {
            IPluginBrowser::Get().UnregisterPluginTemplate(Template.ToSharedRef());
        }
        bRegistered = false;
        Template.Reset();
    }

private:
    TSharedPtr<FPluginTemplateDescription> Template;
    bool bRegistered = false;
};

IMPLEMENT_MODULE(FUE59PluginWizardModule, UE59PluginWizard)

#undef LOCTEXT_NAMESPACE
