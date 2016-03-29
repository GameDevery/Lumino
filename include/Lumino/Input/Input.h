
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
	static bool IsPressed(const StringRef& bindingName);

	/**
		@brief		�{�^�����V���������ꂽ�u�Ԃ𔻒肵�܂��B
	*/
	static bool IsTriggered(const StringRef& bindingName);

	/**
		@brief		�{�^���������ꂽ�u�Ԃ𔻒肵�܂��B
	*/
	static bool IsOffTriggered(const StringRef& bindingName);

	/**
		@brief		�{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肵�܂��B
	*/
	static bool IsRepeated(const StringRef& bindingName);

	/**
		@brief		�w�肵�����̃A�i���O�l���擾���܂��B(0.0�`1.0)
	*/
	static float GetAxisValue(const StringRef& bindingName);

	static VirtualPad* GetController(int index);

private:
};

/**
	@brief	
*/
class InputButtons
{
public:
	static const String Any;
	static const String Left;
	static const String Right;
	static const String Up;
	static const String Down;
	static const String Ok;
	static const String Cancel;
};

LN_NAMESPACE_END
