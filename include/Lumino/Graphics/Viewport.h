
#pragma once
#include "Texture.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class Viewport
	: public Object
{
public:


LN_INTERNAL_ACCESS:	// TODO: ���܂͂Ƃ肠���������p�r
	Viewport();
	virtual ~Viewport();
	void Initialize(RenderTarget* renderTarget);

private:
	RenderTarget*	m_renderTarget;
};

LN_NAMESPACE_END
