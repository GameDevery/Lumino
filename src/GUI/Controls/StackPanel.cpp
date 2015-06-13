
#include "../../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/StackPanel.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// StackPanel
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(StackPanel);
LN_UI_ELEMENT_SUBCLASS_IMPL(StackPanel);
const String	StackPanel::OrientationProperty(_T("Orientation"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StackPanel::StackPanel(GUIManager* manager)
	: Panel(manager)
{
	// �v���p�e�B�̓o�^
	LN_DEFINE_PROPERTY_ENUM(StackPanel, Orientation, OrientationProperty, &StackPanel::SetOrientation, &StackPanel::GetOrientation, Orientation::Horizontal);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
StackPanel::~StackPanel()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void StackPanel::MeasureLayout(const SizeF& availableSize)
{
	// TODO: �}�[�W���Ƃ�����΍l������
	SizeF size = availableSize;

	if (m_orientation == Orientation::Horizontal)
	{
		// ���ɕ��ׂ�ꍇ�A���̐�����݂��Ȃ�
		size.Width = std::numeric_limits<float>::infinity();
	}
	else
	{
		// �c�ɕ��ׂ�ꍇ�A�����̐�����݂��Ȃ�
		size.Height = std::numeric_limits<float>::infinity();
	}

	SizeF desiredSize;
	LN_FOREACH(UIElement* child, *m_children) 
	{
		child->MeasureLayout(size);

		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			desiredSize.Width += childDesiredSize.Width;
			desiredSize.Height = std::max(desiredSize.Height, childDesiredSize.Height);
		}
		else
		{
			desiredSize.Width = std::max(desiredSize.Width, childDesiredSize.Width);
			desiredSize.Height += child->GetDesiredSize().Height;
		}
	}

	// this �� m_desiredSize ��ݒ肷��
	Panel::MeasureLayout(desiredSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void StackPanel::ArrangeLayout(const RectF& finalRect)
{
	float prevChildSize = 0;
	RectF childRect;
	LN_FOREACH(UIElement* child, *m_children)
	{
		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			childRect.X += prevChildSize;
			prevChildSize = childDesiredSize.Width;
			childRect.Width = prevChildSize;
			childRect.Height = std::max(finalRect.Height, childDesiredSize.Height);
		}
		else
		{
			childRect.Y += prevChildSize;
			prevChildSize = childDesiredSize.Height;
			childRect.Height = prevChildSize;
			childRect.Width = std::max(finalRect.Width, childDesiredSize.Width);
		}

		child->ArrangeLayout(childRect);
	}

	Panel::ArrangeLayout(finalRect);
}

} // namespace GUI
} // namespace Lumino

