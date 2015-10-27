
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	�O���t�B�b�N�X���W���[���̊�{�I�ȃ��\�[�X�I�u�W�F�N�g�̃x�[�X�N���X�ł��B�f�o�C�X�̏�ԕω���ʒm����@�\���������܂��B
*/
class GraphicsResourceObject
	: public RefObject
{
protected:
	friend class GraphicsManager;

	// �f�o�C�X���ύX�����ꍇ�A�܂��� NULL ���n����ČĂ΂��B���̂Ƃ��A�K�v�ȃ��\�[�X��ۑ�����B
	// ���ɐV�����f�o�C�X���n����ČĂ΂��B���̂Ƃ��A�ۑ��������\�[�X���f�o�C�X�I�u�W�F�N�g�Ƀ��[�h����B
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device) = 0;
	
	virtual ~GraphicsResourceObject() {}
};

LN_NAMESPACE_END
