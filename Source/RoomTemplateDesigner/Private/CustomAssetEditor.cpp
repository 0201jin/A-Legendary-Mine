
#include "CustomAssetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "Editor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "CustomAssetEditorModule.h"

#include "ViewPort/SRTDViewport.h"
#include "PropertyEditorModule.h"
#include "AdvancedPreviewSceneModule.h"

#include "ListView/SRTDListview.h"

#include "IDetailsView.h"

#define LOCTEXT_NAMESPACE "RoomTemplateDesigner"

const FName FCustomAssetEditor::ToolkitFName(TEXT("RoomTemplateDesigner"));
const FName FCustomAssetEditor::PropertiesTabId(TEXT("Properties"));
const FName FCustomAssetEditor::ViewportTabId(TEXT("Viewport"));
const FName FCustomAssetEditor::ListviewTabId(TEXT("ListView"));

void FCustomAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CustomAssetEditor", "Custom Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnPropertiesTab))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnTab_Viewport))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(ListviewTabId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnTab_Listview))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FCustomAssetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(PropertiesTabId);
	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(ListviewTabId);
}

void FCustomAssetEditor::InitCustomAssetEditorEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UMyCustomAsset* InCustomAsset)
{
	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;

	SetCustomAsset(InCustomAsset);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	Viewport = SNew(SRTDViewport)
		.ParentIGCEditor(SharedThis(this))
		.ObjectToEdit(CustomAsset);

	Listview = SNew(SRTDListview)
		.ObjectToEdit(CustomAsset);

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_CustomAssetEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6)
					->AddTab(ViewportTabId, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6f)
						->AddTab(ListviewTabId, ETabState::OpenedTab)
					)
					/*->Split
					(
						FTabManager::NewStack()
						->AddTab(PropertiesTabId, ETabState::OpenedTab)
					)*/
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;

	FAssetEditorToolkit::InitAssetEditor(
		Mode,
		InitToolkitHost,
		CustomAssetEditorAppIdentifier,
		StandaloneDefaultLayout,
		bCreateDefaultStandaloneMenu,
		bCreateDefaultToolbar,
		(UObject*)InCustomAsset);

	// Set the asset we are editing in the details view
	if (DetailsView.IsValid())
	{
		DetailsView->SetObject((UObject*)CustomAsset);
	}
}

FCustomAssetEditor::~FCustomAssetEditor()
{
	DetailsView.Reset();
	Viewport.Reset();
	PropertiesTab.Reset();
	Listview.Reset();
}

FName FCustomAssetEditor::GetToolkitFName() const
{
	return ToolkitFName;
}

FText FCustomAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Asset Editor");
}

FText FCustomAssetEditor::GetToolkitToolTipText() const
{
	return LOCTEXT("ToolTip", "Custom Asset Editor");
}

FString FCustomAssetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "AnimationDatabase ").ToString();
}

FLinearColor FCustomAssetEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Red;
}

UMyCustomAsset* FCustomAssetEditor::GetCustomAsset()
{
	return CustomAsset;
}

void FCustomAssetEditor::SetCustomAsset(UMyCustomAsset* InCustomAsset)
{
	CustomAsset = InCustomAsset;
}

TSharedRef<SDockTab> FCustomAssetEditor::SpawnPropertiesTab(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == PropertiesTabId);

	return SNew(SDockTab)
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FCustomAssetEditor::SpawnTab_Viewport(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == ViewportTabId);

	return SNew(SDockTab)
		[
			Viewport.ToSharedRef()
		];
}

TSharedRef<SDockTab> FCustomAssetEditor::SpawnTab_Listview(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == ListviewTabId);

	return SNew(SDockTab)
		[
			Listview.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE