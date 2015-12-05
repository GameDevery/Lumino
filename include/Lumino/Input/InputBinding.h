
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class InputBinding
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		InputBinding �I�u�W�F�N�g���쐬���܂��B
		@details	�쐬���ꂽ�I�u�W�F�N�g�͎g���I������ Release() ���Ăяo���ĎQ�Ƃ��������K�v������܂��B
	*/
	static InputBinding* Create(const String& bindingName, Key key, int joyNumber);

protected:
	InputBinding();
	virtual ~InputBinding();

private:
	String						m_bindingName;
	detail::DeviceInputSource	m_source;
};

LN_NAMESPACE_END
