/*

[2019/8/13] Panes
----------

### ���[�hPane
UE4�� mode pane �Ɠ����C���[�W�B
�����A�v���n�u�̃u���E�U�����˂�B

���[�h�͂��܂̂Ƃ���
�E�^�C���}�b�v�ҏW���[�h
�E�I�u�W�F�N�g���[�h

�^�C���}�b�v�ҏW���[�h�ł́A�����Ƀ^�C���Z�b�g���\�������B

�I�u�W�F�N�g���[�h�ł́A�e��I�u�W�F�N�g��v���n�u��I�����A�V�[���ɔz�u�ł���BD&D�ł������ȁB�܂��A�ׂ������Ƃ͂��ƂŁB


### ���C���[Pane
Scene�̃��C���[��\���BTkoolXP�̎��̂悤�ȁA�^�C���}�b�v�̃��C���[�ł͂Ȃ��B
���ۂɂ̓��C���[�̊ԂɃI�u�W�F�N�g���񂾂�A�^�C���̒P�Ȃ�3�����z��̃G�f�B�^�ɂ͂��Ȃ��ق��������Ǝv���B
�� �ł�������Ղ炮����Ƃ�������Ƃ��͂��̎d�g�݂��������ق����悩�����񂾂�ȁE�E�B
�� �悭�l���������ς�K�v�����B���Ƀ^�C�����u�����v���邽�߂ɕʂ̃^�C����u�������ꍇ�B
    �����A�V�[���̃��C���[�ł͂Ȃ� tilemap �̓������C���[�Ƃ�������Ȉʒu�Â��ŁB

���C���[���̂̓V�[���̃��[�g�I�u�W�F�N�g�B
Tilemap ���C���[���I�����ꂽ��A���̂܂܂��� Tilemap �̕ҏW���[�h�ɓ���B
Tilemap �̃I�u�W�F�N�g���[�h�ɖ߂����肷��ƁATilemap ���̂�Transform �Ƃ��������B




[2019/8/13] ���C���[�ɂ���
----------
�����������C���[�Ƃ����f�[�^�\���͕K�v�H�����������́H�Q�[���G���W���ɂ����ă��C���[�̓O���[�s���O�̈Ӗ��̕��������B
�y�C���g�\�t�g�ł͉��ݒ肵���艺�w�̃C���[�W�Ƀt�B���^�t������Ɏg�����ǁH
�E�ҏW�͈͂̐���E�E�E�^�C���}�b�v���C���[:Tilemap�����ώ킷�郂�[�h   �I�u�W�F�N�g���C���[:�I�u�W�F�N�g�̔z�u�����郂�[�h
�E�O��֌W�̐���E�E�E�`�揇�ł͂Ȃ��O��֌W�B�Ȃ̂ŁAUE4 �� Office �̃O���[�v���̈Ӗ��������B

�^�C���}�b�v�̉��Ɏq�I�u�W�F�N�g��u�����肵�����Ȃ��H
�c�����������B

��������ƁA�A�E�g���C�i�őI�񂾃I�u�W�F�N�g�ɂ���ĕҏW���[�h�ς���ق��������H
�E�E�E����ATilemap ���̂ɂ��A�I�u�W�F�N�g���[�h�ƕҏW���[�h�̔C�Ӑ؂�ւ����~�����E�E�E�B

### ��̓I�ɂǂ�ȃX�g�[���[�ŕҏW�������́H
Tiled ���ȁB
�����Ȃ�ƁATilemap ���ƂɕҏW���[�h�ؑցA�Ƃ������A�V�[���S�̂Ƃ��ẮA2D�ҏW���[�h������ς�ق����B

�����āA�����w��Tilemap���������Ƃ͓���Ȃ̂ŁA�ҏW�ΏۂƂȂ�I�u�W�F�N�g�̑I���Ƃ��ăO���[�v�����g�������B����Ƀ��C���[�I�ȍl������ꂽ���B
�� UE4 �ł́A���̖�ڂ́u���C���[�vhttps://api.unrealengine.com/JPN/Engine/UI/LevelEditor/Layers/index.html


### Scene�G�f�B�^�̊�{�@�\�Ƃ��āA�r���I�ȕҏW���b�N������Ƃ����̂��ȁH
����I�u�W�F�N�g�̎q�c���[�����ҏW�ł���悤�Ȏd�g�݁B
�����z�肵�Ă����ҏW���C���[�̑I���Ƃ͂܂�A���̋@�\�ŖړI�̃c���[�ȊO�̕ҏW���b�N��������Ƃ������ƁB
����ɍ��킹�āA���b�N���ꂽ�I�u�W�F�N�g�͔������Ƃ��ɂ��Ă����������B

###�u�A�E�g���C�����C���[�v�݂����Ȗ��O�ɂ��Ă�������
����̎��Ԃ̓q�G�����L�[�ŏ�ʂ̃m�[�h�ł���B

### UE4 �̃��[�h�ؑւ̃C���[�W���߂��H
�ł��A���������h�X�P�[�v�������Ƃ��A���C���[�݂����� 2D �I�ȑO��֌W���A�E�g���C�i�Œ�������K�v������̂ŁA
�����C���[�W���Ă������C���[�I�Ȏg�����Ƃ͂�����ƈႤ�c�B

### �A�v���[�`�̘������Ђǂ��̂Ō�����c�̂܂��ɁA3D�G���W��������̃A�v���[�`�������Ɛ������Ă݂悤
�E�E�E�ƌ����Ă��AUE4 �Ɠ����悤�ɂȂ邩�ȁB
�ł��A�Q�w��tilemap�̊Ԃɋ��܂ꂽ�I�u�W�F�N�g��I���������Ƃ��ɁA�V�[���r���[�ォ��͑I���ł��Ȃ��̂����Ȃ�X�g���X�����B�i����������mouseHitTest���Ȃ��A�ł��������ǁj

��͂�A�V�[���̍\���͋��ʂ����ǃA�v���[�`�̈قȂ�2�̃V�[���G�f�B�^�͂������ق��������C������B


[2019/8/13] �^�C���}�b�v�ҏW���[�h�ƁA�C�x���g(�I�u�W�F�N�g) �ҏW���[�h�𕪂���H
----------
�������ق��������Ǝv���B

PGMMV �ł͓�������Ă����ǁA�傫�ȃI�u�W�F�N�g������Ƃ��ɁA���̌��̃^�C����I���ł��Ȃ��B
����Ō������Ă݂���A�ҏW���[�h�����̂�����������Â炢�����������c�B




*/

#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include <PluginManager.hpp>
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilemapSceneEditor.hpp"
#include "TilemapSceneModePane.hpp"


namespace lna {
    
//==============================================================================
// TilemapSceneListPane

void TilemapSceneListPane::init()
{
    UIControl::init();

    auto layout1 = ln::makeObject<ln::UIBoxLayout3>();
    layout1->setOrientation(ln::Orientation::Vertical);
    addElement(layout1);

    auto layout2 = ln::makeObject<ln::UIHBoxLayout2>();
    layout1->addChild(layout2);
    {
        auto caption = ln::UITextBlock::create(u"Scene");
        caption->setMargin(ln::Thickness(8, 0));    // TODO: theme ����Ƃ肽��
        caption->setVerticalAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilemapSceneListPane::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilemapSceneListPane::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->project();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Scenes");

    m_model = ln::makeObject<TilemapSceneListModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilemapSceneListPane::addButton_onClick(ln::UIEventArgs* e)
{
    auto Scene = ln::makeObject<ln::Scene>();
    auto asset = ln::makeObject<ln::AssetModel>(Scene);

    auto project = lna::Workspace::instance()->project();

    auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Scene-", ln::AssetModel::AssetFileExtension.c_str());

    asset->saveInternal(path);

    m_model->refresh();
}

void TilemapSceneListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// TilemapSceneEditor

ln::Result TilemapSceneEditor::init()
{
    AssetEditor::init();
    m_modePane = ln::makeObject<TilemapSceneModePane>();
    m_inspectorPane = ln::makeObject<ln::EditorPane>();
    m_inspectorPane->setBackgroundColor(ln::Color::Green);

    m_modePanes = ln::makeList<Ref<ln::EditorPane>>({ ln::static_pointer_cast<ln::EditorPane>(m_modePane) });
    m_inspectorPanes = ln::makeList<Ref<ln::EditorPane>>({ m_inspectorPane });
    m_toolPanes = ln::makeList<Ref<ln::EditorPane>>();
    return true;
}

void TilemapSceneEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    auto m_mainViewport = ln::makeObject<ln::UIViewport>();
    frame->addElement(m_mainViewport);
    //m_mainViewport->setBackgroundColor(ln::Color::Blue);// ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));


    auto m_mainWorld = ln::makeObject<ln::World>();
    auto m_mainWorldAsset = ln::AssetModel::create(m_mainWorld);

    auto m_mainCamera = ln::makeObject<ln::Camera>();
    auto m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
    m_mainWorldRenderView->setTargetWorld(m_mainWorld);
    m_mainWorldRenderView->setCamera(m_mainCamera);
    m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
    m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
    m_mainViewport->addRenderView(m_mainWorldRenderView);

    m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());

    // TODO: test
    {
        auto sprite = ln::Sprite::create(ln::Texture2D::create(u"D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4, 4);
        sprite->setSourceRect(0, 0, 16, 16);
        sprite->setPosition(0, 2, 0);
        m_mainWorld->addObject(sprite);


        auto tilemap = ln::makeObject<ln::Tilemap>();
        auto tilemapAsset = ln::AssetModel::create(tilemap);
        tilemap->setShadingModel(ln::ShadingModel::UnLighting);
        m_mainWorld->addObject(tilemap);
        m_mainWorldAsset->addChild(tilemapAsset);

        auto tilesetMaterial = ln::makeObject<ln::Material>();
        tilesetMaterial->setMainTexture(ln::Texture2D::create((u"D:/Proj/LN/PrivateProjects/HC0/Assets/Tilesets/BaseChip_pipo.png")));

        auto tileset = ln::makeObject<ln::Tileset>();
        tileset->reset(tilesetMaterial, 16, 16);

        auto layer = ln::makeObject<ln::TilemapLayer>();
        layer->resize(5, 5);
        layer->setTileSize(ln::Size(1, 1));
        layer->setOrientation(ln::TilemapOrientation::UpFlow);
        layer->setTileId(0, 0, 1);
        layer->setTileId(1, 1, 1);
        layer->setTileId(0, 4, 1);
        layer->setTileId(1, 4, 1);
        layer->setTileId(2, 4, 1);
        layer->setTileId(3, 4, 1);
        layer->setTileId(4, 4, 1);
        auto tilemapModel = ln::makeObject<ln::TilemapModel>();
        tilemapModel->addTileset(tileset);
        tilemapModel->addLayer(layer);
        tilemap->setTilemapModel(tilemapModel);

        m_modePane->setTileset(tileset);
    }

    editorContext()->mainProject()->assetDatabase()->createAsset(m_mainWorld, u"D:/Proj/LN/PrivateProjects/HC0/Assets/Scenes/test.lnasset");
}

void TilemapSceneEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> TilemapSceneEditor::getEditorPanes(ln::EditorPaneKind kind)
{
    switch (kind)
    {
    case ln::EditorPaneKind::Mode:
        return m_modePanes;
    case ln::EditorPaneKind::Inspector:
        return m_inspectorPanes;
    case ln::EditorPaneKind::Tool:
        return m_toolPanes;
    }
    return nullptr;
}

//==============================================================================
// TilemapSceneEditorPloxy

Ref<ln::AssetEditor> TilemapSceneEditorPloxy::createEditor()
{
    return ln::makeObject<TilemapSceneEditor>();
}

//==============================================================================
// TilemapSceneNavigator

void TilemapSceneNavigator::init()
{
    m_navigationBarItem = ln::makeObject<ln::UIIcon>();
    m_navigationBarItem->setIconName(u"globe");
    m_navigationBarItem->setHorizontalAlignment(ln::HAlignment::Center);
    m_navigationBarItem->setVerticalAlignment(ln::VAlignment::Center);
    m_navigationBarItem->setFontSize(24);
    
    m_sceneListPane = ln::makeObject<TilemapSceneListPane>();
}

ln::UIElement* TilemapSceneNavigator::getNavigationMenuItem()
{
    return m_navigationBarItem;
}

ln::UIElement* TilemapSceneNavigator::getNavigationPane()
{
    return m_sceneListPane;
}

//==============================================================================
// TilemapSceneEditorExtensionModule

void TilemapSceneEditorExtensionModule::onActivate(lna::EditorContext* context)
{
    m_navigator = ln::makeObject<TilemapSceneNavigator>();
    context->mainWindow()->navigatorManager()->addNavigator(m_navigator);

    m_editorPloxy = ln::makeObject<TilemapSceneEditorPloxy>();
    context->application()->mainProject()->pluginManager()->addAssetEditorPloxy(m_editorPloxy);
}

void TilemapSceneEditorExtensionModule::onDeactivate(lna::EditorContext* context)
{
    context->application()->mainProject()->pluginManager()->removeAssetEditorPloxy(m_editorPloxy);
}

} // namespace lna


#if 0

//==============================================================================
// TilemapSceneListPane

void TilemapSceneListPane::init()
{
    UIControl::init();

    auto layout1 = ln::makeObject<ln::UIBoxLayout3>();
    layout1->setOrientation(ln::Orientation::Vertical);
    addElement(layout1);

    auto layout2 = ln::makeObject<ln::UIHBoxLayout2>();
    layout1->addChild(layout2);
    {
        auto caption = ln::UITextBlock::create(u"Scene");
        caption->setMargin(ln::Thickness(8, 0));    // TODO: theme ����Ƃ肽��
        caption->setVerticalAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilemapSceneListPane::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilemapSceneListPane::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->project();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Scenes");

    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilemapSceneListPane::addButton_onClick(ln::UIEventArgs* e)
{
    auto Scene = ln::makeObject<ln::Scene>();
    auto asset = ln::makeObject<ln::AssetModel>(Scene);

    auto project = lna::Workspace::instance()->project();

    auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Scene-", ln::AssetModel::AssetFileExtension.c_str());

    asset->saveInternal(path);

    m_model->refresh();
}

void TilemapSceneListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// SceneNavigatorExtension

void SceneNavigatorExtension::onAttached()
{
    m_item = ln::makeObject<ln::NavigationMenuItem>();
    m_item->setIconName(u"globe");  // map, image

    m_list = ln::makeObject<TilemapSceneListPane>();
}

ln::NavigationMenuItem* SceneNavigatorExtension::getNavigationMenuItem()
{
    return m_item;
}

ln::UIElement* SceneNavigatorExtension::getNavigationPane()
{
    return m_list;
}

//==============================================================================
// TilemapSceneEditor

ln::Result TilemapSceneEditor::init()
{
    AssetEditor::init();
    m_modePane = ln::makeObject<ln::EditorPane>();
    m_modePane->setBackgroundColor(ln::Color::Red);
    m_inspectorPane = ln::makeObject<ln::EditorPane>();
    m_inspectorPane->setBackgroundColor(ln::Color::Green);

    m_modePanes = ln::makeList<Ref<ln::EditorPane>>({ m_modePane });
    m_inspectorPanes = ln::makeList<Ref<ln::EditorPane>>({ m_inspectorPane });
    m_toolPanes = ln::makeList<Ref<ln::EditorPane>>();
    return true;
}

void TilemapSceneEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    auto m_mainViewport = ln::makeObject<ln::UIViewport>();
    frame->addElement(m_mainViewport);
    //m_mainViewport->setBackgroundColor(ln::Color::Blue);// ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));


    auto m_mainWorld = ln::makeObject<ln::World>();
    auto m_mainCamera = ln::makeObject<ln::Camera>();
    auto m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
    m_mainWorldRenderView->setTargetWorld(m_mainWorld);
    m_mainWorldRenderView->setCamera(m_mainCamera);
    m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
    m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
    m_mainViewport->addRenderView(m_mainWorldRenderView);

    m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());

    auto sprite = ln::Sprite::create(ln::Texture2D::create(u"D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4,4);
    sprite->setSourceRect(0, 0, 16, 16);
    sprite->setPosition(0, 2, 0);
    m_mainWorld->addObject(sprite);
}

void TilemapSceneEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> TilemapSceneEditor::getEditorPanes(ln::EditorPaneKind kind)
{
    switch (kind)
    {
    case ln::EditorPaneKind::Mode:
        return m_modePanes;
    case ln::EditorPaneKind::Inspector:
        return m_inspectorPanes;
    case ln::EditorPaneKind::Tool:
        return m_toolPanes;
    }
    return nullptr;
}

//==============================================================================
// TilemapSceneEditorExtension

TilemapSceneEditorExtension::TilemapSceneEditorExtension()
{

}

ln::Ref<ln::AssetEditor> TilemapSceneEditorExtension::createEditor()
{
    return ln::makeObject<TilemapSceneEditor>();
}

#endif
