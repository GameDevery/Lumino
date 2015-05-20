
#pragma once

#include "SceneNode.h"

namespace Lumino
{
namespace Scene
{

/// Camera
class Camera
	: public SceneNode
{
public:
	Camera(SceneGraphManager* manager, CameraProjection proj);
	virtual ~Camera();

public:

	/// Y ��������p�̐ݒ�
	void SetFovY(float fov_y) { m_fovY = fov_y; }

	/// Y ��������p�̎擾
	float GetFovY() const { return m_fovY; }

	/// �ł��߂��r���[�v���[���ʒu�̐ݒ�
	void SetNearClip(float nearClip) { m_nearClip = nearClip; }

	/// �ł��߂��r���[�v���[���ʒu�̎擾
	float GetNearClip() const { return m_nearClip; }

	/// �ł������r���[�v���[���ʒu�̐ݒ�
	void SetFarClip(float farClip) { m_farClip = farClip; }

	/// �ł������r���[�v���[���ʒu�̎擾
	float GetFarClip() const { return m_farClip; }

public:	// internal

	/// �e�s����X�V���� (SceneNode::UpdateFrameHierarchy() �̌�ŌĂяo������)
	void UpdateMatrices(const SizeF& viewSize);

	// �����̎擾 (�V�F�[�_�ݒ�p�BUpdateMatrices() �̌�ŌĂяo������)
	const Vector4& GetDirectionInternal() const { return m_direction; }

	// �s��̎擾 (�V�F�[�_�ݒ�p�BUpdateMatrices() �̌�ŌĂяo������)
	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix& GetProjectionMatrix() const { return m_projMatrix; }
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjMatrix; }
	const Matrix& GetViewMatrixI() const { return m_viewMatrixI; }
	const Matrix& GetProjectionMatrixI() const { return m_projMatrixI; }
	const Matrix& GetViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	const Matrix& GetViewMatrixT() const { return m_viewMatrixT; }
	const Matrix& GetProjectionMatrixT() const { return m_projMatrixT; }
	const Matrix& GetViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	const Matrix& GetViewMatrixIT() const { return m_viewMatrixIT; }
	const Matrix& GetProjectionMatrixIT() const { return m_projMatrixIT; }
	const Matrix& GetViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

private:

	CameraProjection	m_projectionMode;
	float				m_fovY;
	float				m_nearClip;
	float				m_farClip;

	Matrix				m_viewMatrix;		///< �r���[�s��
	Matrix				m_projMatrix;		///< �v���W�F�N�V�����s��
	Matrix				m_viewProjMatrix;	///< �r���[�s��ƃv���W�F�N�V�����s��̐�
	Vector4				m_direction;		///< ����

	// �ȉ��̓V�F�[�_�ϐ��ւ̐ݒ�p�B���C�g�͌X�̃m�[�h�ɔ�ׂĎQ�Ƃ����񐔂������̂�
	// �K�v�ɂȂ邽�тɌv�Z����̂ł͂Ȃ��A���炩���ߌv�Z���Ă����B
	Matrix				m_viewMatrixI;		///< �r���[�s�� (Inverse)
	Matrix				m_projMatrixI;		///< �v���W�F�N�V�����s�� (Inverse)
	Matrix				m_viewProjMatrixI;	///< �r���[�s��ƃv���W�F�N�V�����s��̐� (Inverse)
	Matrix				m_viewMatrixT;		///< �r���[�s�� (Transpose)
	Matrix				m_projMatrixT;		///< �v���W�F�N�V�����s�� (Transpose)
	Matrix				m_viewProjMatrixT;	///< �r���[�s��ƃv���W�F�N�V�����s��̐� (Transpose)
	Matrix				m_viewMatrixIT;		///< �r���[�s�� (Inverse * Transpose)
	Matrix				m_projMatrixIT;		///< �v���W�F�N�V�����s�� (Inverse * Transpose)
	Matrix				m_viewProjMatrixIT;	///< �r���[�s��ƃv���W�F�N�V�����s��̐� (Inverse * Transpose)
};

} // namespace Scene
} // namespace Lumino
