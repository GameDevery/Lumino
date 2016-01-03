
#pragma once

#include "ApplicationSettings.h"

LN_NAMESPACE_BEGIN

/**
	@brief		�A�v���P�[�V�����S�̂ɂ�����鏈�����s���܂��B
*/
class Engine
{
public:

	/**
		@brief		�G���W���̏������������s���܂��B
		@params[in]	settings	: �������ݒ�
	*/
	static void Initialize(const ApplicationSettings& settings);
	
	/**
		@brief		�G���W���̏I���������s���܂��B
	*/
	static void Finalize();

	/**
		@brief		1�t���[�����̍X�V�������s���܂��B
		@return		�A�v���P�[�V�����̏I�����v������Ă���ꍇ�� false ��Ԃ��܂��B
		@details	���̊֐��̓O���t�B�b�N�X�Ɠ��͂��X�V���A�w�肳�ꂽ�t���[�����[�g�ɂȂ�悤�ɑҋ@���܂��B
	*/
	static bool UpdateFrame();


	static bool BeginRendering();
	static void EndRendering();
	static void Render();
};

LN_NAMESPACE_END
