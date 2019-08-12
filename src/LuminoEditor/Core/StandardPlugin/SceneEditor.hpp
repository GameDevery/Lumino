#pragma once
#include <LuminoEditor/Plugin.hpp>

/*
    �^�C���}�b�v���V�[���G�f�B�^�Ƃ��������ɋ������ĕҏW���������ǂǂ�����H
    ----------
    Sub-Scene �̍l�����ōs���Ă݂悤�B
    Scene �̓v���n�u���g��Ȃ��ꍇ�A����ȃI�u�W�F�N�g�c���[�̃f�[�^�ƂȂ�ƂȂ�B
    
    �v���n�u���g���ꍇ�͂��̈ꕔ����ʂ� .lnasset �t�@�C���ɒǂ��o���A�ʂ� EditorView �ŕҏW�ł���B
    
    �������A�v���n�u�����Ȃ��ꍇ�� View ��ҏW���� SceneEditor �̎q Editor �Ƃ��Ĉ���Ȃ����΂Ȃ�Ȃ��B
    �Ȃ��Ȃ�AScene ��\�� AssetObject �� Tilemap ��\�� AssetObject �𕪂��邱�Ƃ͂ł��Ȃ�����B
    �i�v���n�u������΂����񂾂��ǁc�j
    �v���n�u������ƁAScene �p�� Tilemap �p�� 2 �̃^�u���J���K�v������A������Ɣς킵���B
    �f�[�^�̕\���Ƃ��Ă͐������񂾂낤���ǁA�����Ɨ������Ȃ��ƍŏ��Ƃ����ɂ����B
    
    �Ȃ̂ŁASceneEditor �Ƃ����ЂƂ� View �̒��ɁA�ʂ� View �� Eclipse �̃p�[�X�y�N�e�B�u�؂�ւ��݂����ɓ���ł���悤�ɂ������B
    �� WorldObject �̕ҏW�͊�{�I�ɉE���̃C���X�y�N�^�ōs�����ATilemap �̂悤�ɃC���X�y�N�^�ł͑���Ȃ����͕̂� View ���J���A�݂����ȁB
*/
class SceneEditor : public ln::AssetEditorViewModel
{
public:
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
};

class SceneEditorPlugin : public ln::IAssetEditorExtension
{
public:
    SceneEditorPlugin();
    virtual const ln::Char* id() const { return u"7B7D6551-3105-4877-99C4-D4EE8EA06482"; }
    virtual const ln::Char* displayName() const { return u"SceneEditorPlugin"; }
    virtual const ln::Char* typeKeyword() const { return u"Scene"; }
    virtual ln::Ref<ln::AssetEditorViewModel> createEditor() override;

private:
};
