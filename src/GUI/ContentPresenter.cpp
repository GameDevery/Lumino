
#include "../Internal.h"
#include <Lumino/GUI/ContentPresenter.h>

namespace Lumino
{
namespace GUI
{
	
//=============================================================================
// ContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentPresenter, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentPresenter);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter* ContentPresenter::Create(GUIManager* manager)
{
	auto obj = RefPtr<ContentPresenter>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::ContentPresenter(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentPresenter::~ContentPresenter()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::SetContent(UIElement* content)
{
	if (m_content.GetObjectPtr() == content) {
		return;
	}

	// ��ɌÂ��͎̂�菜��
	if (m_content != NULL)
	{
		m_visualChildren.Remove(m_content);
		m_content->SetParent(NULL);
	}

	m_content = content;
	m_visualChildren.Add(m_content);	// m_visualChildren �ɒǉ��������̂� OnEvent �� Render ���Ă΂��悤�ɂȂ�
	m_content->SetParent(this);
}

#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ContentPresenter::ApplyTemplateHierarchy(CombinedLocalResource* parent)
//{
//	UIElement::ApplyTemplateHierarchy(parent);
//	if (m_content != NULL) {
//		m_content->ApplyTemplateHierarchy(m_combinedLocalResource);
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::MeasureOverride(const SizeF& constraint)
{
	if (m_content != NULL)
	{
		m_content->MeasureLayout(constraint);
		return m_content->GetDesiredSize();
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ContentPresenter::ArrangeOverride(const SizeF& finalSize)
{
	if (m_content != NULL) {
		m_content->ArrangeLayout(RectF(0, 0, finalSize));
	}
	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentPresenter::Render()
{
	if (m_content != NULL) {
		m_content->Render();
	}
	UIElement::Render();
}

#endif


} // namespace GUI
} // namespace Lumino
