﻿
#include "AssetBrowserNavigator.hpp"
#include "StandardPluginModule.hpp"

namespace lna {


//==============================================================================
// StandardTextureImporterExtension

int StandardTextureImporterExtension::matchFilePath(const ln::Path& filePath)
{
    // TODO: static メソッドにしたい
    auto importer = ln::makeObject<ln::TextureImporter>();
    if (importer->testSupportedExtensions(filePath)) {
        return BasePriority;
    }
    return 0;
}

Ref<ln::AssetImporter> StandardTextureImporterExtension::createImporter(const ln::Char* assetSourceFilePath)
{
    auto importer = ln::makeObject<ln::TextureImporter>();
    if (!importer->testSupportedExtensions(assetSourceFilePath)) {
        return nullptr;
    }
    return importer;
}


//==============================================================================
// StandardPluginModule

StandardPluginModule::StandardPluginModule()
{
    {
        auto ext = ln::makeObject<StandardTextureImporterExtension>();
        m_editorExtensionInstances.add(ext);
        m_editorExtensions.add(ext);
    }
    {
        auto ext = ln::makeObject<AssetBrowserNavigatorExtension>();
        m_editorExtensionInstances.add(ext);
        m_editorExtensions.add(ext);
    }
    //{
    //    auto ext = ln::makeObject<SceneNavigatorExtension>();
    //    m_editorExtensionInstances.add(ext);
    //    m_editorExtensions.add(ext);
    //}
    //{
    //    auto ext = ln::makeObject<TilemapSceneEditorExtension>();
    //    m_editorExtensionInstances.add(ext);
    //    m_editorExtensions.add(ext);
    //}
    //{
    //    auto ext = ln::makeObject<TilesetNavigatorExtension>();
    //    m_editorExtensionInstances.add(ext);
    //    m_editorExtensions.add(ext);
    //}
    //{
    //    auto ext = ln::makeObject<TilesetEditorExtension>();
    //    m_editorExtensionInstances.add(ext);
    //    m_editorExtensions.add(ext);
    //}

}

} // namespace lna