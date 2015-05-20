
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "Common.h"
#include "Layer.h"

namespace Lumino
{
namespace Scene
{

/**
	@brief		�V�[���O���t�̍ŏI�`���ƂȂ郌���_�����O�^�[�Q�b�g�ƁA�V�[���O���t��`�悷�邽�߂̃��C���[���Ǘ����܂��B
	@details	ViewPane �̓V�[���O���t��`�悷�邽�߂̃��[�g�v�f�ł��B
*/
class ViewPane
	: public RefObject
{
public:

	/**
		@brief	ViewPane �̃C���X�^���X���쐬���܂��B
	*/
	ViewPane* Create(SceneGraphManager* manager);

public:

	/**
		@brief	���C���[�̃��X�g���擾���܂��B
	*/
	virtual LayerList* GetLayerList() const = 0;

	/**
		@brief	�w�肵�������_�����O�^�[�Q�b�g�ɁA�S�Ẵ��C���[��`�悵�܂��B
	*/
	virtual void Render(Graphics::Texture* renderTarget) = 0;

public:
	ViewPane();
	virtual ~ViewPane();
};

} // namespace Scene
} // namespace Lumino
