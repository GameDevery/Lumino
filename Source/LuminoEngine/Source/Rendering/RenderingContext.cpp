﻿
#include "Internal.h"
#include <Lumino/Rendering/RenderingContext.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// RenderingContext
//==============================================================================

//------------------------------------------------------------------------------
RenderingContext::RenderingContext()
	: m_currentRenderView(nullptr)
{
}

//------------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
}

//------------------------------------------------------------------------------
bool RenderingContext::initialize()
{
	return DrawList::initialize(detail::EngineDomain::getGraphicsManager());
}

LN_NAMESPACE_END
