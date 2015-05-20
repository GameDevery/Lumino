
#pragma once

#include "SceneNode.h"

namespace Lumino
{
namespace Scene
{

/// Light
class Light
	: public SceneNode
{
public:
	Light(SceneGraphManager* manager, LightType type);
	virtual ~Light();

public:

	/// ���C�g�̎�ނ̎擾
	LightType GetType() const { return m_type; }

	/// ���C�g�̗L���A������ݒ肷��
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	/// ���C�g�̗L���A�������m�F����
	bool IsEnabled() const { return m_enabled; }

	/// �f�B�t���[�Y�J���[�̐ݒ�
	void SetDiffuseColor(const Graphics::ColorF& color) { m_diffuse = color; }

	/// �f�B�t���[�Y�J���[�̎擾
	const Graphics::ColorF& GetDiffuseColor() const { return m_diffuse; }

	/// �A���r�G���g�J���[�̐ݒ�
	void SetAmbientColor(const Graphics::ColorF& color) { m_ambient = color; }

	/// �A���r�G���g�J���[�̎擾
	const Graphics::ColorF& GetAmbientColor() const { return m_ambient; }

	/// �X�y�L�����J���[�̐ݒ�
	void SetSpecularColor(const Graphics::ColorF& color) { m_specular = color; }

	/// �X�y�L�����J���[�̎擾
	const Graphics::ColorF& GetSpecularColor() const { return m_specular; }

	/// �X�|�b�g���C�g�̃R�[���p�x�̐ݒ� (���W�A���P��)
	void SetSpotAngle(float angle) { m_spotAngle = angle; }

	/// �X�|�b�g���C�g�̃R�[���p�x�̎擾 (���W�A���P��)
	float GetSpotAngle() const { return m_spotAngle; }

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

protected:
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList);

private:
	friend class VisualNode;

	LightType			m_type;				///< ���C�g�̎��
	Graphics::ColorF	m_diffuse;			///< �f�B�t���[�Y�J���[
	Graphics::ColorF	m_ambient;			///< �A���r�G���g�J���[
	Graphics::ColorF	m_specular;			///< �X�y�L�����J���[
	bool				m_enabled;			///< �L�����
	float				m_spotAngle;		///< �R�[���p�x (���W�A���P��)

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

	float				m_tmpDistance;		///< �m�[�h�ɋ߂����C�g����������Ƃ��̍�Ɨp�ϐ�
};

} // namespace Scene
} // namespace Lumino
