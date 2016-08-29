
#include "Internal.h"
#include <Lumino/UI/UIControl.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIControl
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIControl, UIElement);

LN_TR_PROPERTY_IMPLEMENT(UIControl, VerticalAlignment, VerticalContentAlignmentProperty, "VerticalContentAlignment", m_verticalContentAlignment, tr::PropertyMetadata());
LN_TR_PROPERTY_IMPLEMENT(UIControl, HorizontalAlignment, HorizontalContentAlignmentProperty, "HorizontalContentAlignment", m_horizontalContentAlignment, tr::PropertyMetadata());

//------------------------------------------------------------------------------
UIControl::UIControl()
	: m_visualTreeRoot(nullptr)
{
}

//------------------------------------------------------------------------------
UIControl::~UIControl()
{
	LN_SAFE_RELEASE(m_visualTreeRoot);
}

//------------------------------------------------------------------------------
void UIControl::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
}

//------------------------------------------------------------------------------
int UIControl::GetVisualChildrenCount() const
{
	return (m_visualTreeRoot != nullptr) ? 1 : 0;
}

//------------------------------------------------------------------------------
UIElement* UIControl::GetVisualChildOrderd(int index) const
{
	LN_THROW(0 <= index && index < GetVisualChildrenCount(), OutOfRangeException);
	return m_visualTreeRoot;
}

//------------------------------------------------------------------------------
SizeF UIControl::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize = UIElement::MeasureOverride(constraint);
	if (m_visualTreeRoot != nullptr)
	{
		m_visualTreeRoot->MeasureLayout(constraint);
		const SizeF& childDesiredSize = m_visualTreeRoot->GetDesiredSize();

		desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	}
	return desiredSize;
}

//------------------------------------------------------------------------------
SizeF UIControl::ArrangeOverride(const SizeF& finalSize)
{
	RectF childFinal(0, 0, finalSize);
	if (m_visualTreeRoot != nullptr)
	{
		SizeF childDesiredSize = m_visualTreeRoot->GetDesiredSize();
		childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
		childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
		m_visualTreeRoot->ArrangeLayout(RectF(0, 0, childDesiredSize));
	}
	return finalSize;
}

//------------------------------------------------------------------------------
VerticalAlignment* UIControl::GetPriorityContentVerticalAlignment()
{
	if (m_verticalContentAlignment == VerticalAlignment::Stretch) return nullptr;
	return &m_verticalContentAlignment;
}

//------------------------------------------------------------------------------
HorizontalAlignment* UIControl::GetPriorityContentHorizontalAlignment()
{
	if (m_horizontalContentAlignment == VerticalAlignment::Stretch) return nullptr;
	return &m_horizontalContentAlignment;
}

//------------------------------------------------------------------------------
void UIControl::SetVisualTreeRoot(UIElement* element)
{
	if (m_visualTreeRoot != nullptr)
	{
		m_visualTreeRoot->SetParent(nullptr);
	}

	LN_REFOBJ_SET(m_visualTreeRoot, element);

	if (m_visualTreeRoot != nullptr)
	{
		m_visualTreeRoot->SetParent(this);
	}
}

LN_NAMESPACE_END
