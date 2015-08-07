/*
	ScrollViewer �̊T�v
	https://msdn.microsoft.com/ja-jp/library/ms750665(v=vs.110).aspx

	ScrollViewer �̊e��v���p�e�B
	http://smart-pda.net/wp7/tips/scrollviewer_tips/

	ScrollViewer �̓��\�b�h�Ăяo���� (��ʓI�ɂ͈�x�R�}���h�L���[�ɗ��ߍ����) 
	ScrollViewer.ScrollInfo �ɒʒm����B
	ScrollViewer ���q�v�f�̃I�t�Z�b�g���v�Z����̂ł͂Ȃ��_�ɒ��ӁB
	
	ScrollContentPresenter �� Child(IScrollInfo) ���ATemplatedParent(ScrollViewer).ScrollInfo �ɃZ�b�g����B
	(HookupScrollingComponents �Ō���)

	IScrollInfo �� StackPanel �� DocumentViewer�ATextBoxView ���Ƃ��Ď��������B
	���ۂɎq�v�f�̃I�t�Z�b�g���v�Z����̂͂����̃N���X�B
	������������邽�߂ɁA�e�N���X ScrollData �Ƃ��������N���X�������Ă���B


	ListBox �� Temlate ��2�p�^�[���B
	ItemsPresenter ���g�����@�ƁA�g��Ȃ����@�B

	�g��Ȃ��ꍇ�� Panel �� IsItemsHost �� true �ɂ���B���̏ꍇ�AVisualTree �� ItemsPresenter �͌���Ȃ��B�����������ꂽ�肵�Ȃ��B
	
	�g���ꍇ�AScrollContentPresenter �̎q�� ItemsPresenter �ɂȂ�B
	���� ItemsPresenter �̎q�́APanel �ɂȂ�B
	���̂Ƃ��A���� Panel �� IScrollInfo ���������Ă���΁A�X�N���[������̑ΏۂɂȂ�B
	���Ȃ݂ɁAItemsPresenter �� IScrollInfo �ł͂Ȃ��B

*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollViewer.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ScrollContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollContentPresenter, ContentPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollContentPresenter);

const float ScrollContentPresenter::m_scrollLineDelta = 16.0f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenterPtr ScrollContentPresenter::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::ScrollContentPresenter(GUIManager* manager)
	: ContentPresenter(manager)
	, m_scrollData()
	, m_scrollInfo(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::~ScrollContentPresenter()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollContentPresenter::OnApplyTemplate(CombinedLocalResource* localResource)
{
	ContentPresenter::OnApplyTemplate(localResource);
	HookupScrollingComponents();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollContentPresenter::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize;
	if (IsScrollClient())
	{
		SizeF childConstraint = constraint;

		// �c�����ꂼ��A�X�N���[���ł���̂ł���΃T�C�Y������݂��Ȃ�
		if (m_scrollData.CanHorizontallyScroll) { childConstraint.Width = std::numeric_limits<float>::infinity(); }
		if (m_scrollData.CanVerticallyScroll) { childConstraint.Height = std::numeric_limits<float>::infinity(); }

		desiredSize = ContentPresenter::MeasureOverride(childConstraint);	// �q�v�f�� Measure �͂��̒��ŁB

		//m_scrollData.Viewport = constraint;
		m_scrollData.Extent = desiredSize;
	}
	else
	{
		// �q�v�f�ɑ΂��ăX�N���[�����삷��ꍇ�́AMeasure ���q�v�f�ɔC����
		desiredSize = ContentPresenter::MeasureOverride(constraint);		// �q�v�f�� Measure �͂��̒��ŁB
	}

	desiredSize.Width = std::min(constraint.Width, desiredSize.Width);
	desiredSize.Height = std::min(constraint.Height, desiredSize.Height);
	return desiredSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollContentPresenter::ArrangeOverride(const SizeF& finalSize)
{
	if (IsScrollClient())
	{
		m_scrollData.Viewport = finalSize;
	}

	if (!m_visualChildren.IsEmpty())
	{
		UIElement* child = m_visualChildren[0];
		if (child != NULL)
		{
			RectF childRect(PointF::Zero, child->GetDesiredSize());

			if (IsScrollClient())
			{
				childRect.X = -GetHorizontalOffset();
				childRect.Y = -GetVerticalOffset();
			}
			childRect.Width = std::max(childRect.Width, finalSize.Width);
			childRect.Height = std::max(childRect.Height, finalSize.Height);

			//printf("%f\n", childRect.X);

			child->ArrangeLayout(childRect);
		}
	}

	return finalSize;
}

//-----------------------------------------------------------------------------
// ���� ScrollContentPresenter ���A�q�v�f�̃I�t�Z�b�g�𒼐ڑ��삷�邩�ǂ����B
// �܂�A�q�v�f�� IScrollInfo �ł͂Ȃ����B
//-----------------------------------------------------------------------------
bool ScrollContentPresenter::IsScrollClient() const
{
	return m_scrollInfo == this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollContentPresenter::HookupScrollingComponents()
{
	ScrollViewer* owner = dynamic_cast<ScrollViewer*>(m_templateParent);
	LN_VERIFY_RETURN(owner != NULL);	// ���肦�Ȃ��񂾂���ǂ��B

	// TODO: ... ���낢��q�v�f��T���� IScrollInfo ��T��
	// TODO: �o�O�B�Ȃ� ItemsPresenter ����������ǉ�����Ă����E�E�E
	IScrollInfo* scrollInfo = NULL;

	// �q�A���� IScrollInfo ��������Ȃ������ꍇ�́Athis ���X�N���[������̑ΏۂƂ���
	if (scrollInfo == NULL)
	{
		scrollInfo = this;
	}

	// �������� IScrollInfo �ƁA�I�[�i�[�� ScrollViewer �����т���
	scrollInfo->SetScrollOwner(owner);
	owner->SetScrollInfo(scrollInfo);

	m_scrollInfo = scrollInfo;
}

//=============================================================================
// ScrollViewer
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollViewer);

const String ScrollViewer::PART_ScrollContentPresenterTemplateName(_T("PART_ScrollContentPresenter"));
const String ScrollViewer::PART_VerticalScrollBarTemplateName(_T("PART_VerticalScrollBar"));
const String ScrollViewer::PART_HorizontalScrollBarTemplateName(_T("PART_HorizontalScrollBar"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewerPtr ScrollViewer::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::ScrollViewer(GUIManager* manager)
	: ContentControl(manager)
{
	LN_REGISTER_ROUTED_EVENT_HANDLER(ScrollViewer, ScrollEventArgs, ScrollBar::ScrollEvent, Handler_ScrollBar_Scroll);

	//LayoutUpdated += LN_CreateDelegate(this, &ScrollViewer::ScrollViewer_LayoutUpdated);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::~ScrollViewer()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetHorizontalOffset(float offset)
{
	if (m_scrollInfo != NULL) {
		m_scrollInfo->SetHorizontalOffset(offset);
		m_horizontalScrollBar->SetValue(offset);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetVerticalOffset(float offset)
{
	if (m_scrollInfo != NULL) {
		m_scrollInfo->SetVerticalOffset(offset);
		m_verticalScrollBar->SetValue(offset);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::OnLayoutUpdated()
{
	if (m_scrollInfo != NULL)
	{
		m_verticalScrollBar->SetTrackExtent(m_scrollInfo->GetExtentHeight() - m_scrollInfo->GetViewportHeight());
		m_verticalScrollBar->SetTrackViewportSize(m_scrollInfo->GetViewportHeight());

		m_horizontalScrollBar->SetTrackExtent(m_scrollInfo->GetExtentWidth() - m_scrollInfo->GetViewportWidth());
		m_horizontalScrollBar->SetTrackViewportSize(m_scrollInfo->GetViewportWidth());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::PollingTemplateChildCreated(UIElement* newElement)
{
	// PART �̃L�[�Ɉ�v���閼�O�����v�f�����������烁���o�ɕێ����Ă���
	if (newElement->GetKeyName() == PART_ScrollContentPresenterTemplateName) {
		m_scrollContentPresenter = dynamic_cast<ScrollContentPresenter*>(newElement);
	}
	else if (newElement->GetKeyName() == PART_VerticalScrollBarTemplateName) {
		m_verticalScrollBar = dynamic_cast<ScrollBar*>(newElement);
		m_verticalScrollBar->m_isStandalone = false;	// ScrollViewer �Ŗʓ|������t���O
	}
	else if (newElement->GetKeyName() == PART_HorizontalScrollBarTemplateName) {
		m_horizontalScrollBar = dynamic_cast<ScrollBar*>(newElement);
		m_horizontalScrollBar->m_isStandalone = false;	// ScrollViewer �Ŗʓ|������t���O
	}

	ContentControl::PollingTemplateChildCreated(newElement);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollViewer::MeasureOverride(const SizeF& constraint)
{
	// �X�N���[���o�[�����\���̏����͂����ł���

	return ContentControl::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF ScrollViewer::ArrangeOverride(const SizeF& finalSize)
{
	return ContentControl::ArrangeOverride(finalSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::SetScrollInfo(IScrollInfo* scrollInfo)
{
	m_scrollInfo = scrollInfo;
	// TODO:
	//m_scrollInfo->SetCanVerticallyScroll();
	//m_scrollInfo->SetCanHorizontallyScroll();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ScrollViewer::Handler_ScrollBar_Scroll(ScrollEventArgs* e)
{
	if (m_scrollInfo != NULL)
	{
		switch (e->Type)
		{
		case ScrollEventType::ThumbTrack:
			// �{���͈�x�R�}���h���X�g�����āA�x���ɔ�����ׂ�
			if (e->Sender == m_verticalScrollBar) {
				SetVerticalOffset(e->NewValue);
				//m_scrollInfo->SetVerticalOffset(e->NewValue);
				//printf("%f\n", e->NewValue);
			}
			else if (e->Sender == m_horizontalScrollBar) {
				SetHorizontalOffset(e->NewValue);
				//m_scrollInfo->SetHorizontalOffset(e->NewValue);
			}
			break;
		default:
			break;
		}
	}
}

} // namespace GUI
} // namespace Lumino

