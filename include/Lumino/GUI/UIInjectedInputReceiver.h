
#pragma once
#include "../Common.h"
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief	GUI �V�X�e���ւ̓��̓C�x���g�̓�����ƂȂ�C���^�[�t�F�C�X�ł��B
*/
class IUIInjectedInputReceiver
{
public:

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
		@param[in]	clientX		: X ���W
		@param[in]	clientY		: Y ���W
	*/
	virtual bool InjectMouseWheel(int delta, float clientX, float clientY) = 0;

	/**
		@brief		�L�[�������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	keyCode		: �L�[�R�[�h
		@param[in]	isAlt		: Alt �L�[��������Ă���ꍇ�� true
		@param[in]	isShift		: Shift �L�[��������Ă���ꍇ�� true
		@param[in]	isControl	: Ctrl �L�[��������Ă���ꍇ�� true
	*/
	virtual bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl) = 0;

	/**
		@brief		�L�[�������ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	keyCode		: �L�[�R�[�h
		@param[in]	isAlt		: Alt �L�[��������Ă���ꍇ�� true
		@param[in]	isShift		: Shift �L�[��������Ă���ꍇ�� true
		@param[in]	isControl	: Ctrl �L�[��������Ă���ꍇ�� true
	*/
	virtual bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl) = 0;

	/**
		@brief		���������͂��ꂽ���̃C�x���g���󂯎��܂��B
		@param[in]	ch			: ���͂��ꂽ����
	*/
	virtual bool InjectTextInput(TCHAR ch) = 0;

	/**
		@brief		���Ԍo�߂��ʒm����܂��B
		@param[in]	elapsedTime	: �o�ߎ��� (�b)
	*/
	virtual void InjectElapsedTime(float elapsedTime) = 0;

protected:
	virtual ~IUIInjectedInputReceiver() {}
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
