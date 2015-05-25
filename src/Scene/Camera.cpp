
#include "../Internal.h"
#include "Camera.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// Camera
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Camera::Camera(SceneGraphManager* manager, CameraProjection proj)
	: m_projectionMode(proj)
	, m_fovY(Math::PI * 0.125f)
	, m_nearClip(1.0f)
	, m_farClip(1000.0f)
{
	m_transform.Translation.Set(0, 0, -20.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Camera::~Camera()
{
}

/// �s�N�Z���P�ʂ�2D�`��Ɏg���ˉe�s��̍쐬
static void Perspective2DLH(float width, float height, float nearClip, float farClip, Matrix* outMatrix)
{
	outMatrix->Set(
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		-1.0f, 1.0f, nearClip / (nearClip - farClip), 1.0f);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Camera::UpdateMatrices(const SizeF& viewSize)
{
	// 2D ���[�h
	if (m_projectionMode == CameraProjection_2D)
	{
		// ���ʕ���
		Vector3 direction = Vector3::TransformCoord(Vector3(0, 0, 1), m_combinedGlobalMatrix);
		m_direction = Vector4(direction, 0.0f);

		m_viewMatrix = m_combinedGlobalMatrix;
		Perspective2DLH(viewSize.Width, viewSize.Height, m_nearClip, m_farClip, &m_projMatrix);
		m_viewProjMatrix = m_viewMatrix * m_projMatrix;
	}
	// 3D ���[�h
	else
	{
		// �����_
		Vector3 lookAt = Vector3::TransformCoord(Vector3(0, 0, 1), m_combinedGlobalMatrix);

		// �r���[�s��
		Vector3 up = Vector3(0, 1, 0);
		m_viewMatrix = Matrix::LookAtLH(m_combinedGlobalMatrix.GetPosition(), lookAt, up);

		// �v���W�F�N�V�����s��̍X�V
		// TODO: ����p��near,far
		// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
		m_projMatrix = Matrix::PerspectiveFovLH(Math::PI / 4.0f, viewSize.Width / viewSize.Height, m_nearClip, m_farClip);

		m_viewProjMatrix = m_viewMatrix * m_projMatrix;

		// ���ʕ���
		m_direction = Vector4(lookAt - m_combinedGlobalMatrix.GetPosition(), 0.0f);
	}

	m_viewMatrixI = Matrix::Inverse(m_viewMatrix);
	m_projMatrixI = Matrix::Inverse(m_projMatrix);
	m_viewProjMatrixI = Matrix::Inverse(m_viewProjMatrix);
	m_viewMatrixT = Matrix::Transpose(m_viewMatrix);
	m_projMatrixT = Matrix::Transpose(m_projMatrix);
	m_viewProjMatrixT = Matrix::Transpose(m_viewProjMatrix);
	m_viewMatrixIT = Matrix::Transpose(m_viewMatrixI);
	m_projMatrixIT = Matrix::Transpose(m_projMatrixI);
	m_viewProjMatrixIT = Matrix::Transpose(m_viewProjMatrixI);
}

} // namespace Scene
} // namespace Lumino
