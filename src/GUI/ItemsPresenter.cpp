
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ItemsPresenter.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ItemsPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsPresenter, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenterPtr ItemsPresenter::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::ItemsPresenter(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsPresenter::~ItemsPresenter()
{

}

//-----------------------------------------------------------------------------
// ���̊֐��̓I�[�i�[�� ItemsControl ���u���� Panel �Ń��C�A�E�g����v�ƒʒm���邽�߂̂��́B
// ���� ItemsPresenter �� �I�[�i�[�� ItemsControl �Ɋ��蓖�Ă�ꂽ�Ƃ���A
// �I�[�i�[�� ItemsControl �� HostPanelTemplate ���ς�����Ƃ��ɌĂ΂��B
//-----------------------------------------------------------------------------
void ItemsPresenter::AttachHostPanel(Panel* panel)
{
	// �Â��͎̂�菜��
	if (m_hostPanel != NULL)
	{
		RemoveVisualChild(m_hostPanel);
	}

	// �V�����̂�ǉ�����
	m_hostPanel = panel;
	if (m_hostPanel != NULL)
	{
		AddVisualChild(m_hostPanel);
		m_hostPanel->ApplyTemplate();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsPresenter::MeasureOverride(const SizeF& constraint)
{
	//MeasureElementWithSingleChild

	SizeF size = UIElement::MeasureOverride(constraint);
	if (m_hostPanel != NULL)
	{
		m_hostPanel->MeasureLayout(constraint);
		SizeF childSize = m_hostPanel->GetDesiredSize();
		size.Width = std::max(size.Width, childSize.Width);
		size.Height = std::max(size.Height, childSize.Height);
	}
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsPresenter::ArrangeOverride(const SizeF& finalSize)
{
	SizeF size = UIElement::ArrangeOverride(finalSize);
	if (m_hostPanel != NULL)
	{
		RectF rect(0, 0, size);
		m_hostPanel->ArrangeLayout(rect);
	}
	return size;
}

} // namespace GUI
} // namespace Lumino

