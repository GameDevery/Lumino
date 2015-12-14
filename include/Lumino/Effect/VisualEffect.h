
#pragma once
#include <list>
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		���o���ʃI�u�W�F�N�g�𑀍삷��@�\��񋟂��܂��B
*/
class VisualEffect
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		�w�肵���t�@�C������ VisualEffect �̃C���X�^���X���쐬���܂��B
		@param[in]	filePath	: �G�t�F�N�g�t�@�C��
		@details	�Ή����Ă���G�t�F�N�g�t�@�C���� Effekseer �̃t�@�C�� (.efk) �ł��B
	*/
	static VisualEffectPtr Create(const StringRef& filePath);

public:

	/**
		@brief		�G�t�F�N�g�̍Đ����J�n���܂��B
		@param[in]	overlap	: true�̏ꍇ�͍Đ����̃G�t�F�N�g������Β�~�����ɐV�����G�t�F�N�g�̍Đ����J�n���Afalse �̏ꍇ�͒�~���Ă���J�n����B
		@return		�J�n���ꂽ�G�t�F�N�g�������N���X
	*/
	VisualEffectInstance* Play(bool overlap);

	/**
		@brief		���̃I�u�W�F�N�g����Đ����ꂽ�G�t�F�N�g���~���܂��B
	*/
	void Stop();

	/**
		@brief		���̃I�u�W�F�N�g����Đ����ꂽ�G�t�F�N�g�̂����ꂩ���Đ��������m�F���܂��B
	*/
	bool IsPlaying() const;
	
	/**
		@brief		�Đ����̃G�t�F�N�g�̎p�� (���[���h�ϊ��s��) �����̃I�u�W�F�N�g�Ɠ������邩��ݒ肵�܂��B
		@param[in]	enabled	: true �̏ꍇ�A�������� (�f�t�H���g true)
	*/
	void SetSyncEffects(bool enabled);

	/**
		@brief		���̃I�u�W�F�N�g�̎p�� (���[���h�ϊ��s��) ��ݒ肵�܂��B
	*/
	void SetWorldMatrix(const Matrix& matrix);

protected:
	VisualEffect() = default;
	virtual ~VisualEffect() = default;
	void Initialize(detail::EffectCore* core);

private:
	detail::EffectCore*					m_core = nullptr;
	std::list<VisualEffectInstance*>	m_instanceList;
	Matrix								m_worldMatrix;
	bool								m_syncEffects = true;
};


LN_NAMESPACE_END
