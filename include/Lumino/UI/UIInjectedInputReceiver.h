
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	GUI �V�X�e���ւ̓��̓C�x���g�̓�����ƂȂ�C���^�[�t�F�C�X�ł��B
*/
class IUIInjectedInputReceiver
{
public:
	
	/**
		@brief		�`���̃s�N�Z���T�C�Y�ύX�C�x���g���󂯎��܂��B
		@param[in]	width		: ��
		@param[in]	height		: ����
	*/
	//virtual bool InjectViewportSizeChanged(int width, int height) = 0;

	/**
		@brief		�}�E�X�ړ��C�x���g���󂯎��܂��B
		@param[in]	clientX		: X ���W
		@param[in]	clientY		: Y ���W
	*/
	virtual bool InjectMouseMove(float clientX, float clientY) = 0;

	/**
		@brief		�}�E�X�{�^���������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	button		: ���삳�ꂽ�}�E�X�{�^��
		@param[in]	clientX		: X ���W
		@param[in]	clientY		: Y ���W
	*/
	virtual bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY) = 0;

	/**
		@brief		�}�E�X�{�^���������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	button		: ���삳�ꂽ�}�E�X�{�^��
		@param[in]	clientX		: X ���W
		@param[in]	clientY		: Y ���W
	*/
	virtual bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY) = 0;

	/**
		@brief		�}�E�X�z�C�[�������삳�ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	delta		: �z�C�[���̈ړ���
	*/
	virtual bool InjectMouseWheel(int delta) = 0;

	/**
		@brief		�L�[�������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	keyCode		: �L�[�R�[�h
		@param[in]	modifierKeys: �C���L�[
	*/
	virtual bool InjectKeyDown(Key keyCode, ModifierKeys modifierKeys) = 0;

	/**
		@brief		�L�[�������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	keyCode		: �L�[�R�[�h
		@param[in]	modifierKeys: �C���L�[
	*/
	virtual bool InjectKeyUp(Key keyCode, ModifierKeys modifierKeys) = 0;

	/**
		@brief		���������͂��ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	ch			: ���͂��ꂽ����
	*/
	virtual bool InjectTextInput(TCHAR ch) = 0;

protected:
	virtual ~IUIInjectedInputReceiver() {}
};

LN_NAMESPACE_END
