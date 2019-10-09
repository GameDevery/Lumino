﻿#pragma once
#include <Workspace.hpp>
#include "EditorContext.hpp"

class MainWindow;

class EditorApplication
    : public ln::Application
{
public:
    static Ref<ln::UICommand> NewCommand;
    static Ref<ln::UICommand> OpenCommand;


    static EditorApplication* instance();

    EditorApplication();
    ~EditorApplication();
    ln::Result init();
    void dispose();
    void run();
	void setMainWindow(ln::UIMainWindow* window);	// onInit でのみ可

    MainWindow* mainWindow() const;
    const ln::Ref<lna::Workspace>& workspace() const { return m_workspace; }
    lna::Project* mainProject() const;
	const ln::Ref<lna::EditorContext>& editorContext() const { return m_editorContext; }

    void importFile(const ln::Path& filePath);
    void openAssetFile(const ln::Path& filePath);

protected:
    virtual void onInit() override;

private:
    void openProject(const ln::Path& filePath);
    void closeProject();

    void handleNewProject(ln::UICommandEventArgs* e);
    void handleOpenProject(ln::UICommandEventArgs* e);

    Ref<lna::Workspace> m_workspace;
    Ref<lna::EditorContext> m_editorContext;
};

