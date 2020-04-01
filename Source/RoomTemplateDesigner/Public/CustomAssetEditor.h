#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"
#include "ICustomAssetEditor.h"
#include "MyCustomAsset.h"

class IDetailsView;
class SDockableTab;
class UMyCustomAsset;

/**
 * 
 */
class ROOMTEMPLATEDESIGNER_API FCustomAssetEditor : public ICustomAssetEditor
{
public:

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	void InitCustomAssetEditorEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UMyCustomAsset* InCustomAsset);

	virtual ~FCustomAssetEditor();

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual bool IsPrimaryEditor() const override { return true; }

	virtual UMyCustomAsset* GetCustomAsset();
	virtual void SetCustomAsset(UMyCustomAsset* InCustomAsset);

public:
	static const FName ToolkitFName;

private:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Listview(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_ActorListView(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_RoomSize(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_ObjectListView(const FSpawnTabArgs& Args);

private:
	TSharedPtr<class SRoomSizeWidget> RoomSize;
	TSharedPtr<class SRTDViewport> Viewport;
	TSharedPtr<class SRTDListview> Listview;
	TSharedPtr<class SRTDActorListView> ActorListView;
	TSharedPtr<class SObjectListView> ObjectListView;

	static const FName ViewportTabId;
	static const FName ListviewTabId;
	static const FName ActorListViewTabId;
	static const FName RoomSizeId;
	static const FName ObjectListViewId;

	UMyCustomAsset* CustomAsset;
};