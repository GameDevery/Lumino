#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {
class TilemapSceneEditorModel;

class TilemapSceneModePane
    : public ln::EditorPane
{
public:
	static const int DisplayTilesX = 8;	// ���� 8 �� 1 �s�Ƃ���̂� View ���̓s���BModel ���Ƃ��ẮA���e�N�X�`���͂��������t�H�[�}�b�g��������Ȃ����ATileset �͒P�� id �󂯎���� tile �Ԃ������ŁA���̐��͋C�ɂ��Ȃ�
	static const float DisplayTileSize;

    void init(TilemapSceneEditorModel* model);
    void setTileset(ln::Tileset* tileset);

protected:
	virtual void onSourcePropertyChanged(ln::UINotifyPropertyChangedEventArgs* e) override;
    virtual void onRender(ln::UIRenderingContext* context) override;
	virtual void onRoutedEvent(ln::UIEventArgs* e) override;

private:
	TilemapSceneEditorModel* m_model;
    Ref<ln::Tileset> m_tileset;
    Ref<ln::Material> m_material;
	ln::Size m_tileSize;
	ln::Vector2 m_displayScale;
};

} // namespace lna
