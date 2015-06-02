
#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

/**
	@brief	LightNote �̏�������t���[���X�V�A�I���������̃A�v���P�[�V�����S�̂Ɋւ��@�\�ł��B
*/
LN_MODULE(Application)

//==============================================================================
/**
	@brief		LightNote �̏�������X�V���A��I�ȏ������s���N���X�ł��B
*/
LN_STATIC_CLASS(LNApplication)
	
	/**
		@brief		LightNote �����������܂��B
		@details	���C�u�����̂��ׂĂ̋@�\���g�p�ł���悤�ɏ��������s���܂��B
	*/
	LN_STATIC_API
	LN_LIBRARY_INITIALIZER
	LNResult LNApplication_Initialize();
	/*Option
		@override[cpp]
		@override_end
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		�t���[�����X�V���܂��B
		@details	LightNote �̏�Ԃ��X�V���A���Ԃ�1�t���[�����i�߂܂��B
					���̊֐��͕K��1�t���[����1�x�����Ăяo���K�v������܂��B
	*/
	LN_STATIC_API
	LNResult LNApplication_Update();
	/*Option
		@override[hsp]
		@override_end
	Option*/

	/**
		@brief		��ʂ̍X�V�^�C�~���O�����Z�b�g���܂��B
		@details	���Ԃ̂����鏈���̌�ɂ��̃��\�b�h���ĂԂ��ƂŁA
					�ɒ[�ȃt���[���X�L�b�v���������Ȃ��悤�ɂ��邱�Ƃ��ł��܂��B
	*/
	LN_STATIC_API
	LNResult LNApplication_ResetFrameDelay();

	/**
		@brief		�A�v���P�[�V�������I������ׂ������m�F���܂��B
		@param[out]	requested	: �I���v���̗L�����i�[����ϐ�
		@details	�E�B���h�E�̃N���[�Y�{�^���������ꂽ�ꍇ���A
					�A�v���P�[�V�������I������ׂ����ɂ� LN_FALSE ��Ԃ��܂��B
	*/
	LN_STATIC_API
	LNResult LNApplication_IsEndRequested(LNBool* requested);

	/**
		@brief		LightNote �̏I���������s���܂��B
	*/
	LN_STATIC_API
	LN_LIBRARY_TERMINATOR
	void LNApplication_Terminate();


	LN_INTERNAL_API void* LNApplication_GetInternalObject();

LN_CLASS_END

LN_MODULE_END

} // extern "C"
