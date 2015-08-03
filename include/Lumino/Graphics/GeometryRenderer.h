
#pragma once

#include <Lumino/Base/Rect.h>
#include <Lumino/Math/Matrix.h>

namespace Lumino
{
namespace Graphics
{
class GraphicsManager;
class GeometryRendererCore;

/**
	@brief		��{�I�Ȍ`��̕`��N���X
*/
class GeometryRenderer
	: public RefObject
{
public:
	static GeometryRenderer* Create(GraphicsManager* manager);

public:

	/// �`��p�X�̊J�n (�Ăяo���͔C�ӁB�Ăяo���Ȃ��ꍇ�ADraw�` �n�͂������_�o�b�t�@�� DrawPrimitive ���邾���ɂȂ�)
	void BeginPass();

	/// �`��p�X�̏I��
	void EndPass();

	/// ���W�ϊ��s��̐ݒ�
	void SetTransform(const Matrix& matrix);

	/// ���W�ϊ��s��̐ݒ�
	void SetViewProjTransform(const Matrix& matrix);

	/// �e�N�X�`���̐ݒ� (NULL �Ń_�~�[�e�N�X�`�����g�p����)
	void SetTexture(Device::ITexture* texture);

	/// �����̕`��
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

	/// �l�p�`�̕`��
	void DrawSquare(
		float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
		float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
		float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
		float x4, float y4, float z4, float u4, float v4, const ColorF& c4);

	/// ��`�̕`��     
	void DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color);

protected:
	GeometryRenderer(GraphicsManager* manager);
	virtual ~GeometryRenderer();
	GraphicsManager*		m_manager;
	GeometryRendererCore*	m_internal;
};

} // namespace Graphics
} // namespace Lumino
