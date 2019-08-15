﻿
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>

namespace ln {
    
//==============================================================================
// UIPopup

UIPopup::UIPopup()
    : m_opend(false)
{
}

void UIPopup::init()
{
	UIElement::init();
    specialElementFlags().set(detail::UISpecialElementFlags::VisualLeaf);
}

void UIPopup::setContent(UIElement* content)
{
    m_content = content;
    if (m_content) {
        addVisualChild(m_content);
    }
}

void UIPopup::setPlacementTarget(UIElement* target)
{
    m_placementTarget = target;
}

void UIPopup::setPlacementMode(PlacementMode mode)
{
    m_placementMode = mode;
}

void UIPopup::open()
{
    if (!m_content) return;
    if (!m_placementTarget) return;

    if (!m_opend)
    {
        if (!m_adorner) {
            m_adorner = makeObject<UIAdorner>(m_placementTarget);
            m_adorner->setContent(m_content);
        }

        UIRenderView* renderView = getRenderView();
        if (renderView) {
            renderView->adornerLayer()->add(m_adorner);
        }
        m_opend = true;
    }
}

void UIPopup::close()
{
    if (m_opend)
    {
        if (m_adorner)
        {
            UIRenderView* renderView = getRenderView();
            if (renderView) {
                renderView->adornerLayer()->remove(m_adorner);
            }
            m_adorner = nullptr;
        }
        m_opend = false;
    }
}

} // namespace ln

