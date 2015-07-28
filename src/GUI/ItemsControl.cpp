
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ItemsControl.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ItemsControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsControl, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsControl); 

LN_DEFINE_PROPERTY_2(ItemsControl, ControlTemplate*, ItemsPanelTemplateProperty, "ItemsPanelTemplate", NULL, &ItemsControl::SetItemsPanelTemplate, &ItemsControl::GetItemsPanelTemplate);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl* ItemsControl::Create(GUIManager* manager)
{
	auto obj = RefPtr<ItemsControl>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::ItemsControl(GUIManager* manager)
	: Control(manager)
	, m_visualItemsPresenter(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ItemsControl::~ItemsControl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::OnApplyTemplate(CombinedLocalResource* localResource)
{
	Control::OnApplyTemplate(localResource);

	// m_itemsPanelTemplate �� m_visualItemsPresenter �͕K�������Ȃ���΂Ȃ�Ȃ�
	LN_THROW(m_itemsPanelTemplate != NULL, InvalidOperationException);
	LN_THROW(m_visualItemsPresenter != NULL, InvalidOperationException);

	// �A�C�e����ێ����� Panel �����
	// TODO: Template �Ɋ֐��|�C���^���悤�ɂ��Ă����Ώ��������ł���
	RefPtr<CoreObject> obj(m_manager->CreateObject(m_itemsPanelTemplate->GetTargetType()));
	auto panel = dynamic_cast<Panel*>(obj.GetObjectPtr());
	LN_THROW(panel != NULL, InvalidOperationException);		// Panel �̃T�u�N���X�łȂ���΂Ȃ�Ȃ�
	m_hostPanel = panel;

	m_visualItemsPresenter->AttachHostPanel(m_hostPanel);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ItemsControl::PollingTemplateChildCreated(UIElement* newElement)
{
	auto ip = dynamic_cast<ItemsPresenter*>(newElement);
	if (ip != NULL) {
		m_visualItemsPresenter = ip;
	}
	Control::PollingTemplateChildCreated(newElement);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsControl::MeasureOverride(const SizeF& constraint)
{
	// TODO ������ƓK��
	if (!m_visualChildren.IsEmpty()) {
		m_visualChildren[0]->MeasureLayout(constraint);
	}
	return Control::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ItemsControl::ArrangeOverride(const SizeF& finalSize)
{
	// TODO ������ƓK��
	if (!m_visualChildren.IsEmpty()) {
		m_visualChildren[0]->ArrangeOverride(finalSize);
	}
	return Control::ArrangeOverride(finalSize);
}

} // namespace GUI
} // namespace Lumino

