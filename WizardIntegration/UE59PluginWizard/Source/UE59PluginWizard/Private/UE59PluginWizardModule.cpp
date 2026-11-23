#include "Features/IPluginsEditorFeature.h"
#include "IPluginBrowser.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "PluginDescriptor.h"
#include "ModuleDescriptor.h"
#include "IPluginWizardDefinition.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

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
                FName(*(RuntimeName + TEXT("Editor"))),
                EHostType::Editor,
                ELoadingPhase::Default));
        }
    }
};

// Each invocation owns its own selection. PluginBrowser copies all selected
// folders before compiling, so generated reflected classes are present for UHT.
class FConfigurableWizardDefinition final : public IPluginWizardDefinition
{
public:
    FConfigurableWizardDefinition(const FString& InTemplateRoot,
        TSharedRef<FPluginTemplateDescription> Base)
        : TemplateRoot(InTemplateRoot)
    {
        Templates.Add(Base);
        Options.Add({TEXT("ActorComponent"), LOCTEXT("Component", "Blueprint Actor Component"), false});
        Options.Add({TEXT("Interface"), LOCTEXT("Interface", "Blueprint Interface"), false});
        Options.Add({TEXT("DataAsset"), LOCTEXT("DataAsset", "Blueprint Data Asset"), false});
        Options.Add({TEXT("FunctionLibrary"), LOCTEXT("Library", "Blueprint Function Library"), false});
    }

    const TArray<TSharedRef<FPluginTemplateDescription>>& GetTemplatesSource() const override { return Templates; }
    void OnTemplateSelectionChanged(TSharedPtr<FPluginTemplateDescription> Item, ESelectInfo::Type) override { Selected = Item; }
    bool HasValidTemplateSelection() const override { return Selected.IsValid(); }
    TSharedPtr<FPluginTemplateDescription> GetSelectedTemplate() const override { return Selected; }
    void ClearTemplateSelection() override { Selected.Reset(); }
    bool CanShowOnStartup() const override { return false; }
    bool HasModules() const override { return Selected.IsValid(); }
    bool IsMod() const override { return false; }
    void OnShowOnStartupCheckboxChanged(ECheckBoxState) override {}
    ECheckBoxState GetShowOnStartupCheckBoxState() const override { return ECheckBoxState::Unchecked; }

    TSharedPtr<SWidget> GetCustomHeaderWidget() override
    {
        TSharedRef<SVerticalBox> Box = SNew(SVerticalBox);
        for (int32 Index = 0; Index < Options.Num(); ++Index)
        {
            Box->AddSlot().AutoHeight().Padding(2)
            [
                SNew(SCheckBox)
                .IsChecked_Lambda([this, Index]() { return Options[Index].bEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
                .OnCheckStateChanged_Lambda([this, Index](ECheckBoxState State) { Options[Index].bEnabled = State == ECheckBoxState::Checked; })
                [ SNew(STextBlock).Text(Options[Index].Label) ]
            ];
        }
        return Box;
    }

    FText GetInstructions() const override
    {
        return LOCTEXT("Instructions", "Select the framework template, choose any optional C++ scaffolds, then enter the plugin name and create. All options are off by default.");
    }
    bool GetPluginIconPath(FString& OutIconPath) const override
    {
        return GetTemplateIconPath(Templates[0], OutIconPath);
    }
    bool GetTemplateIconPath(TSharedRef<FPluginTemplateDescription>, FString& OutIconPath) const override
    {
        OutIconPath = IPluginManager::Get().FindPlugin(TEXT("PluginBrowser"))->GetBaseDir() / TEXT("Resources/DefaultIcon128.png");
        return true;
    }
    EHostType::Type GetPluginModuleDescriptor() const override { return EHostType::Runtime; }
    ELoadingPhase::Type GetPluginLoadingPhase() const override { return ELoadingPhase::Default; }
    FString GetPluginFolderPath() const override { return Templates[0]->OnDiskPath; }
    TArray<FString> GetFoldersForSelection() const override
    {
        TArray<FString> Folders;
        if (Selected.IsValid())
        {
            Folders.Add(Selected->OnDiskPath);
            for (const FOption& Option : Options)
            {
                if (Option.bEnabled)
                {
                    Folders.Add(TemplateRoot / TEXT("Scaffolds") / Option.Folder);
                }
            }
        }
        return Folders;
    }
    void PluginCreated(const FString&, bool) const override {}

private:
    struct FOption { FString Folder; FText Label; bool bEnabled; };
    FString TemplateRoot;
    TArray<FOption> Options;
    TArray<TSharedRef<FPluginTemplateDescription>> Templates;
    TSharedPtr<FPluginTemplateDescription> Selected;
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

        TemplateRoot = Plugin->GetBaseDir() / TEXT("Templates");
        const FString TemplateDirectory = TemplateRoot / TEXT("RuntimeEditor");

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

        if (!IsRunningCommandlet())
        {
            FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TabName,
                FOnSpawnTab::CreateRaw(this, &FUE59PluginWizardModule::SpawnConfigurableWizard))
                .SetDisplayName(LOCTEXT("ConfigurableWizard", "Configurable Plugin Wizard"))
                .SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());
            bTabRegistered = true;
        }
    }

    virtual void ShutdownModule() override
    {
        if (bTabRegistered)
        {
            if (TSharedPtr<SDockTab> Tab = OpenTab.Pin())
            {
                Tab->RequestCloseTab();
            }
            FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
        }
        if (bRegistered && Template.IsValid() &&
            FModuleManager::Get().IsModuleLoaded(TEXT("PluginBrowser")))
        {
            IPluginBrowser::Get().UnregisterPluginTemplate(Template.ToSharedRef());
        }
        bRegistered = false;
        Template.Reset();
    }

private:
    TSharedRef<SDockTab> SpawnConfigurableWizard(const FSpawnTabArgs& Args)
    {
        TSharedRef<FPluginTemplateDescription> Configurable = MakeShared<FRuntimeEditorTemplateDescription>(
            Template->Name, Template->Description, Template->OnDiskPath, true,
            EHostType::Runtime, ELoadingPhase::Default, false);
        Configurable->bCanBePlacedInEngine = false;
        TSharedRef<SDockTab> Tab = IPluginBrowser::Get().SpawnPluginCreatorTab(Args,
            MakeShared<FConfigurableWizardDefinition>(TemplateRoot, Configurable));
        OpenTab = Tab;
        return Tab;
    }

    const FName TabName = TEXT("UE59ConfigurablePluginWizard");
    FString TemplateRoot;
    TWeakPtr<SDockTab> OpenTab;
    bool bTabRegistered = false;
    TSharedPtr<FPluginTemplateDescription> Template;
    bool bRegistered = false;
};

IMPLEMENT_MODULE(FUE59PluginWizardModule, UE59PluginWizard)

#undef LOCTEXT_NAMESPACE
