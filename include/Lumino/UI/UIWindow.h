
#pragma once
#include "UIElement.h"
#include "../Viewport.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIWindow
	: public Object
{
public:


LN_INTERNAL_ACCESS:	// TODO: ���܂͂Ƃ肠���������p�r
	UIWindow();
	virtual ~UIWindow();
	void Initialize();

private:
	Viewport*	m_backBufferViewport;
};

LN_NAMESPACE_END
