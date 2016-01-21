
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class RenderingContext
{
public:
	static const int MaxMultiRenderTargets = 4;

	/**
		@brief	�����_�����O�X�e�[�g��ݒ肵�܂��B
	*/
	void SetRenderState(const RenderState& state);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�X�e�[�g���擾���܂��B
	*/
	const RenderState& GetRenderState() const;

	/**
		@brief	�[�x�e�X�g�y�уX�e���V���e�X�g�X�e�[�g��ݒ肵�܂��B
	*/
	void SetDepthStencilState(const DepthStencilState& state);
	
	/**
		@brief	�[�x�e�X�g�y�уX�e���V���e�X�g�X�e�[�g���擾���܂��B
	*/
	const DepthStencilState& GetDepthStencilState() const;

	/**
		@brief	�����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	*/
	void SetRenderTarget(int index, Texture* texture);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g���擾���܂��B
	*/
	Texture* GetRenderTarget(int index) const;

	/**
		@brief	�[�x�o�b�t�@��ݒ肵�܂��B
	*/
	void SetDepthBuffer(Texture* depthBuffer);

	/**
		@brief	���ݐݒ肳��Ă���[�x�o�b�t�@���擾���܂��B
	*/
	Texture* GetDepthBuffer() const;

	/**
		@brief	�r���[�|�[�g��ݒ肵�܂��B
	*/
	void SetViewport(const Rect& rect);

	/**
		@brief	���ݐݒ肳��Ă���r���[�|�[�g���擾���܂��B
	*/
	const Rect& GetViewport() const;

	/**
		@brief	���_�o�b�t�@��ݒ肵�܂��B
	*/
	void SetVertexBuffer(VertexBuffer* vertexBuffer);

	/**
		@brief	�C���f�b�N�X�o�b�t�@��ݒ肵�܂��B
	*/
	void SetIndexBuffer(IndexBuffer* indexBuffer);

	/**
		@brief	���ݐݒ肳��Ă��郌���_�����O�^�[�Q�b�g�A�[�x�o�b�t�@���N���A���܂��B
	*/
	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	���ݐݒ肳��Ă��钸�_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ăv���~�e�B�u�������_�����O���܂��B
	*/
	void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

};

LN_NAMESPACE_END
