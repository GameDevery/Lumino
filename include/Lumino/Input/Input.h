
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class Input
{
public:

	/**
		@brief		�{�^�������݉�����Ă��邩�𔻒肵�܂��B
	*/
	static bool IsPress(const TCHAR* bindingName);

	/**
		@brief		�{�^�����V���������ꂽ�u�Ԃ𔻒肵�܂��B
	*/
	static bool IsOnTrigger(const TCHAR* bindingName);

	/**
		@brief		�{�^���������ꂽ�u�Ԃ𔻒肵�܂��B
	*/
	static bool IsOffTrigger(const TCHAR* bindingName);

	/**
		@brief		�{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肵�܂��B
	*/
	static bool IsRepeat(const TCHAR* bindingName);

	/**
		@brief		�w�肵�����̃A�i���O�l���擾���܂��B(0.0�`1.0)
	*/
	static float GetAxisValue(const TCHAR* bindingName);

private:
};

/**
	@brief	
*/
class InputButtons
{
public:
	static const String& Left;
	static const String& Right;
	static const String& Up;
	static const String& Down;
};

LN_NAMESPACE_END
