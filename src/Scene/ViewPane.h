
#pragma once
#include <Lumino/Base/BindingSupport.h>
#include <Lumino/Platform/EventArgs.h>
#include <Lumino/Graphics/Texture.h>
#include "Layer.h"

namespace Lumino
{
namespace Scene
{

class LayerList
	: public RefObjectList
{
public:
	LN_REF_OBJECT_LIST_DECL(Layer);
	LayerList() {}
	virtual ~LayerList() {}
};

/// ViewPane
class ViewPane
	: public RefObject
{
public:
	ViewPane(SceneGraphManager* manager);
	virtual ~ViewPane();

public:

	/// ���C���[���X�g�̎擾
	LayerList* GetLayerList() { return &m_layerList; }

	/// �`��
	void Render(Graphics::Texture* renderTarget);

	/// �}�E�X�ړ��C�x���g��ʒm���� (ViewPane �̍���� 0,0 �Ƃ������W���w�肷��)
	bool InjectMouseMove(int x, int y);

	/// �}�E�X�{�^���C�x���g��ʒm����
	bool InjectMouseButtonDown(MouseButton button);

	/// �}�E�X�{�^���C�x���g��ʒm����
	bool InjectMouseButtonUp(MouseButton button);

private:

	/// �}�E�X�{�^���̏��
	struct MouseState
	{
		Point	Position;	///< �Ō�Ƀ{�^���������ꂽ�Ƃ��̃}�E�X�̍��W (�ŏI�I�ɃV�F�[�_�ɓn���̂� -1.0�`1.0 �����A�X�N���[���̃��T�C�Y�ɔ����Č��̍��W�Ŏ����Ă���)
		float	Time;		///< �Ō�Ƀ{�^���������ꂽ�Ƃ��̎��ԁi�b�j
		bool	IsDown;		///< ���݃{�^����������Ă��邩

		void ToVector4(const SizeF& viewSize, Vector4* v) {
			v->X = (2.0f * ((float)Position.X) / viewSize.Width) - 1.0f;
			v->Y = (2.0f * ((float)Position.Y) / viewSize.Height) - 1.0f;
			v->Z = (IsDown) ? 1.0f : 0.0f;
			v->W = Time;
		}
	};

	SceneGraphManager*	m_manager;
	LayerList			m_layerList;

	MouseState			m_leftMouseState;		///< �}�E�X�̍��{�^���̏��
	MouseState			m_rightMouseState;		///< �}�E�X�̉E�{�^���̏��
	MouseState			m_middleMouseState;		///< �}�E�X�̒��{�^���̏��
	Point				m_mousePosition;		///< �}�E�X�̌��݈ʒu
};

} // namespace Scene
} // namespace Lumino
