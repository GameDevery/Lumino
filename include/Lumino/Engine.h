
#pragma once
#include "Foundation/Application.h"
#include "EngineSettings.h"

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
	static void Initialize(const EngineSettings& settings);
	
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

	static Application* GetApplication();
	static bool BeginRendering();
	static void EndRendering();
	static void Render();
	static void Exit();

	
	/**
		@brief		�x�������Z�b�g���܂��B
		@details	���\�[�X�̃��[�h���Ŏ��Ԃ������蒷�����ԍX�V�������s���Ȃ������ꍇ�A
					UpdateFrame() �͖{������ׂ����Ԃɒǂ������Ƃ��Ă��΂炭�̊ԃm�[�E�F�C�g�Ńt���[���X�V���s���܂��B
					���̊Ԃ̓A�v���P�[�V���������ɍ����ɓ��삵�Ă���悤�Ɍ����Ă��܂��܂��B
					�����������邽�߁A���Ԃ̂����鏈���̒���ł��̊֐����ĂԂ��ƂŁAFPS ����ɒx�����������Ă��Ȃ����Ƃ�`���܂��B
	*/
	static void ResetFrameDelay();
};

LN_NAMESPACE_END
