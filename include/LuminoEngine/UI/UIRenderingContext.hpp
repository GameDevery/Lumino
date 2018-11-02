﻿#pragma once
#include <LuminoEngine/Rendering/RenderingContext.hpp>

namespace ln {

class UIRenderingContext
	: public RenderingContext
{
public:
    UIRenderingContext();


public: // TODO: inernal
	void reset();

	Ref<detail::DrawElementList> m_elementList;
};

} // namespace ln

