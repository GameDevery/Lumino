
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "Common.h"
#include "Layer.h"

namespace Lumino
{
namespace Scene
{

/**
	@brief		���C���[�̊�{�N���X�ł��B
*/
class Layer
	: public RefObject
{
protected:
	Layer();
	virtual ~Layer();
};

/**
	@brief		�V�[���O���t�̃m�[�h�c���[��`�悷�郌�C���[�ł��B
*/
class DrawingLayer
	: public Layer
{
public:

	/**
		@brief		���̃��C���[���`�悷��m�[�h���w�肵�܂��B
		@details	���̃��C���[�ɂ͎w�肵���m�[�h�ƁA�S�Ă̎q�m�[�h���`�悳��܂��B
	*/
	virtual void SetRenderingRootNode(SceneNode* node) = 0;

	/**
		@brief		���̃��C���[���`�悷��m�[�h���擾���܂��B
	*/
	virtual SceneNode* GetRenderingRootNode() const = 0;

	/**
		@brief		���̃��C���[�̕`��Ɏg�p���鎋�_��ݒ肵�܂��B
	*/
	virtual void SetCamera(Camera* camera) = 0;

	/**
		@brief		���̃��C���[�̕`��Ɏg�p���鎋�_���擾���܂��B
	*/
	virtual Camera* GetCamera() const = 0;

protected:
	DrawingLayer();
	virtual ~DrawingLayer();
	class DrawingLayerImpl;
	DrawingLayerImpl*	m_impl;
};


} // namespace Scene
} // namespace Lumino
