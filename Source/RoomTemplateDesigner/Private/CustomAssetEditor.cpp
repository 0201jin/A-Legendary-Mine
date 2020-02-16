
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
#include "RoomSizeWidget/SRoomSizeWidget.h"

#include "IDetailsView.h"

#define LOCTEXT_NAMESPACE "RoomTemplateDesigner"

const FName FCustomAssetEditor::ToolkitFName(TEXT("RoomTemplateDesigner"));
const FName FCustomAssetEditor::ViewportTabId(TEXT("Viewport"));
const FName FCustomAssetEditor::ListviewTabId(TEXT("ListView"));
const FName FCustomAssetEditor::RoomSizeId(TEXT("RoomSize"));

void FCustomAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_CustomAssetEditor", "Custom Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnTab_Viewport))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(ListviewTabId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnTab_Listview))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(RoomSizeId, FOnSpawnTab::CreateSP(this, &FCustomAssetEditor::SpawnTab_RoomSize))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FCustomAssetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(RoomSizeId);
	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(ListviewTabId);
}

void FCustomAssetEditor::InitCustomAssetEditorEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UMyCustomAsset* InCustomAsset)
{
	const bool bIsUpdatable = false;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;

	SetCustomAsset(InCustomAsset);

	Viewport = SNew(SRTDViewport)
		.ParentIGCEditor(SharedThis(this))
		.ObjectToEdit(CustomAsset);

	Listview = SNew(SRTDListview)
		.ObjectToEdit(CustomAsset);

	RoomSize = SNew(SRoomSizeWidget)
		.Viewport(Viewport);

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
						->SetSizeCoefficient(0.9f)
						->AddTab(ListviewTabId, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.1f)
						->AddTab(RoomSizeId, ETabState::OpenedTab)
					)
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
}

FCustomAssetEditor::~FCustomAssetEditor()
{
	Viewport.Reset();
	Listview.Reset();
	RoomSize.Reset();
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

TSharedRef<SDockTab> FCustomAssetEditor::SpawnTab_RoomSize(const FSpawnTabArgs & Args)
{
	check(Args.GetTabId() == RoomSizeId);

	return SNew(SDockTab)
		[
			RoomSize.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE