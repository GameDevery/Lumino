
#include "Internal.h"
#include <Lumino/UI/UIContext.h>		// TODO: ����Ȃ�����
#include <Lumino/UI/UILayoutView.h>		// TODO: ����Ȃ�����
#include <Lumino/UI/UIPanel.h>
#include <Lumino/UI/UIContentControl.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIContentControl
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIContentControl, UIElement);

//------------------------------------------------------------------------------
UIContentControl::UIContentControl()
	: m_contentHost(nullptr)
	, m_contentElement(nullptr)
	, m_contentElementModified(false)
{
}

//------------------------------------------------------------------------------
UIContentControl::~UIContentControl()
{
	LN_SAFE_RELEASE(m_contentElement);
	LN_SAFE_RELEASE(m_contentHost);
}

//------------------------------------------------------------------------------
void UIContentControl::Initialize(detail::UIManager* manager)
{
	UIControl::Initialize(manager);

	SetVerticalContentAlignment(VerticalAlignment::Stretch);
	SetHorizontalContentAlignment(HorizontalAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UIContentControl::SetContent(UIElement* element)
{
	// ���� content �������Ă���Ύ�菜���Ă���
	if (m_contentElement != nullptr && m_contentHost != nullptr)
	{
		m_contentHost->RemoveChild(m_contentElement);
		LN_SAFE_RELEASE(m_contentElement);
	}

	// �V���� m_contentElement ��ێ�����
	LN_REFOBJ_SET(m_contentElement, element);
	if (m_contentElement != nullptr)
	{
		m_contentElementModified = true;
	}
}

//------------------------------------------------------------------------------
void UIContentControl::OnUpdatingLayout()
{
	// VisualTree ��������ΒP�Ȃ� Panel ������� m_contentHost �Ƃ��Ďg���B
	// ���̎��_�� VisualTree �������Ƃ������Ƃ́AStyle �������ł��Ȃ��������A
	// Style �̎��� Template �� Host �ƂȂ�� Panel ���܂܂�Ă��Ȃ������Ƃ������ƁB
	if (GetVisualTreeRoot() == nullptr)
	{
		auto panel = RefPtr<UIPanel>::MakeRef();
		panel->Initialize(GetManager());

		// ContentAlignment �ݒ�
		panel->SetVerticalAlignment(GetVerticalContentAlignment());
		panel->SetHorizontalAlignment(GetHorizontalContentAlignment());

		m_contentHost = panel.DetachMove();
		SetVisualTreeRoot(m_contentHost);
	}

	// m_contentHost �� m_contentElement ��ǉ�����
	if (m_contentElementModified)
	{
		m_contentHost->AddChild(m_contentElement);

		// �v�f�c���[�ɓ������̂ŃX�^�C�����X�V����
		m_contentHost->ApplyTemplateHierarchy(GetOwnerLayoutView()->GetOwnerContext()->GetRootStyleTable(), GetLocalStyle());
		m_contentElementModified = false;
	}
}

LN_NAMESPACE_END
