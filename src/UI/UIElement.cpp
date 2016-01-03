
#include "Internal.h"
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/UI/UIElement.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UILayoutView.h>
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// UIElement
//=============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIElement, tr::ReflectionObject)

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseEnterEvent, "MouseEnter", MouseEnter);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseLeaveEvent, "MouseLeave", MouseLeave);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIMouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyDownEvent, "KeyDown", KeyDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, KeyUpEvent, "KeyUp", KeyUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, UIKeyEventArgs, TextInputEvent, "TextInput", TextInput);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement()
	: m_ownerLayoutView(nullptr)
	, m_parent(nullptr)
	, m_opacity(1.0f)
	, m_combinedOpacity(0.0f)
	, m_isEnabled(nullptr)
	, m_isMouseOver(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::~UIElement()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Focus()
{
	if (IsFocusable())
	{
		if (m_parent != nullptr) { m_parent->ActivateInternal(this); }
		m_ownerLayoutView->GetOwnerContext()->SetFocusElement(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::CaptureMouse()
{
	m_ownerLayoutView->CaptureMouse(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	m_ownerLayoutView->ReleaseMouseCapture(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int UIElement::GetVisualChildrenCount() const
{
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::GetVisualChild(int index) const
{
	LN_THROW(0, InvalidOperationException);
	return nullptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::GetVisualChildOrderd(int index) const
{
	LN_THROW(0, InvalidOperationException);
	return nullptr;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::RaiseEvent(const UIEventInfo* ev, UIElement* sender, UIEventArgs* e)
{
	e->sender = sender;
	RaiseEventInternal(ev, e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// �������t���O�����̗v�f�ɓ`�d������
	if (m_parent != nullptr)
	{
		// �t�H���g�� MeasureOverride() �̒��ōX�V����
		m_invalidateFlags |= (m_parent->m_invalidateFlags & detail::InvalidateFlags::Font);
	}

	// �e�v�f����q�v�f��z�u�ł���͈�(availableSize)���󂯎��ADesiredSize ���X�V����B
	// �@ Pane �\[measure()   �c ���͈͓̔��Ȃ�z�u�ł����]�� Button
	// �A Pane ��[DesiredSize �c ���Ⴀ���̃T�C�Y�ł��肢���܂�]�\ Button		�����̎��_�� inf ��Ԃ����Ƃ����蓾��B
	// �B Pane �\[arrange()   �c ���̎q�v�f�Ƃ̌��ˍ����ōŏI�T�C�Y�̓R����]�� Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin ���l������
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	SizeF localAvailableSize(
		std::max(availableSize.Width - marginWidth, 0.0f),
		std::max(availableSize.Height - marginHeight, 0.0f));

	m_desiredSize = MeasureOverride(localAvailableSize);

	// Margin ���l������
	m_desiredSize.Width += marginWidth;
	m_desiredSize.Height += marginHeight;

	// �t�H���g�̖����t���O�𗎂Ƃ�
	// TODO: UITextElement �ֈړ����������ǂ������H
	m_invalidateFlags &= ~detail::InvalidateFlags::Font;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect �͂��̗v�f��z�u�ł���̈�T�C�Y�B�ƁA�e�v�f���ł̃I�t�Z�b�g�B
	// �v�f�ɒ��ڐݒ肳��Ă���T�C�Y�����傫�����Ƃ�����B
	// TODO: HorizontalAlignment �����l�����āA�ŏI�I�ȍ��W�ƃT�C�Y�����肷��B
	//		 ���̗v�f�̃T�C�Y���ȗ�����Ă���΁AStretch �Ȃ�T�C�Y�͍ő�ɁA����ȊO�Ȃ�ŏ��ɂȂ�B

	SizeF arrangeSize;

	// ���̗v�f�̃T�C�Y�������I�Ɏw�肳��Ă���ꍇ�͂������D�悷��
	arrangeSize.Width = Math::IsNaNOrInf(m_size.Width) ? finalLocalRect.Width : m_size.Width;
	arrangeSize.Height = Math::IsNaNOrInf(m_size.Height) ? finalLocalRect.Height : m_size.Height;

	const SizeF& ds = GetDesiredSize();
	RectF arrangeRect;
	UIHelper::AdjustHorizontalAlignment(arrangeSize, ds, m_horizontalAlignment, &arrangeRect);
	UIHelper::AdjustVerticalAlignment(arrangeSize, ds, m_verticalAlignment, &arrangeRect);

	// Margin ���l������ (0 �ȉ��ɂ͏o���Ȃ�)
	float marginWidth = m_margin.Left + m_margin.Right;
	float marginHeight = m_margin.Top + m_margin.Bottom;
	arrangeRect.Width = std::max(arrangeRect.Width - marginWidth, 0.0f);
	arrangeRect.Height = std::max(arrangeRect.Height - marginHeight, 0.0f);


	SizeF renderSize = ArrangeOverride(arrangeRect.GetSize());
	m_finalLocalRect.X = finalLocalRect.X + m_margin.Left + arrangeRect.X;
	m_finalLocalRect.Y = finalLocalRect.Y + m_margin.Top + arrangeRect.Y;
	m_finalLocalRect.Width = renderSize.Width;
	m_finalLocalRect.Height = renderSize.Height;

	OnLayoutUpdated();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UIElement::MeasureOverride(const SizeF& constraint)
{
	// �߂�l�́Aconstraint �̐����̒��ŁA�q�v�f�����C�A�E�g���邽�߂ɕK�v�ȍŏ��T�C�Y�B
	// ���[�U�[�w��̃T�C�Y������ꍇ�͂����Ԃ��B
	// �������Aconstraint �𒴂��邱�Ƃ͂ł��Ȃ��B

	SizeF size;
	// NaN �̏ꍇ�A���̗v�f�Ƃ��ĕK�v�ȍŏ��T�C�Y�� 0 �ƂȂ�B
	size.Width = Math::IsNaNOrInf(m_size.Width) ? 0.0f : m_size.Width;
	size.Height = Math::IsNaNOrInf(m_size.Height) ? 0.0f : m_size.Height;
	size.Width = std::min(size.Width, constraint.Width);
	size.Height = std::min(size.Height, constraint.Height);

	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF UIElement::ArrangeOverride(const SizeF& finalSize)
{
	return finalSize;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnLayoutUpdated()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnMouseMove(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseMoveEvent, this, e); } }
void UIElement::OnMouseDown(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseDownEvent, this, e); } }
void UIElement::OnMouseUp(UIMouseEventArgs* e) { if (!e->handled) { RaiseEvent(MouseUpEvent, this, e); } }
void UIElement::OnKeyDown(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(KeyDownEvent, this, e); } }
void UIElement::OnKeyUp(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(KeyUpEvent, this, e); } }
void UIElement::OnTextInput(UIKeyEventArgs* e) { if (!e->handled) { RaiseEvent(TextInputEvent, this, e); } }

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnMouseEnter(UIMouseEventArgs* e)
{
	// �e�ɂ��}�E�X���͂��߂ď�����̂ł���ΐe�ɂ��ʒm����
	if (m_parent != NULL && !m_parent->m_isMouseOver) {
		m_parent->OnMouseEnter(e);
	}

	m_isMouseOver = true;

	if (!e->handled) { RaiseEvent(MouseEnterEvent, this, e); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnMouseLeave(UIMouseEventArgs* e)
{
	// �e�ɂ��}�E�X����������ƂɂȂ��Ă���΁A�q�b�g�e�X�g��������Œʒm����
	if (m_parent != NULL && m_parent->m_isMouseOver)
	{
		if (!m_parent->m_finalGlobalRect.Contains(PointF(e->x, e->y))) {
			m_parent->OnMouseLeave(e);
		}
	}

	m_isMouseOver = false;

	if (!e->handled) { RaiseEvent(MouseLeaveEvent, this, e); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// ��납�烋�[�v����B��̃��m����ɕ`�悳���̂ŁA���̕������R�B
	// TODO: Z�I�[�_�[�͕ʂ̃��X�g�ɂ����ق��������C������E�E�E
	int count = GetVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		UIElement* e = GetVisualChildOrderd(i)->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}

	if (m_finalGlobalRect.Contains(globalPt)) {
		return this;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ActivateInternal(UIElement* child)
{
	if (m_parent != NULL) { m_parent->ActivateInternal(this); }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(detail::UIInternalEventType type, UIEventArgs* args)
{
	/* ���̂Ƃ���A�C�x���g���ċA�Œʒm���Ă����K�v�͂Ȃ��B
	�}�E�X�C�x���g�� Hover ���Ă�����̂� Manager �����ڑ��荞�ށB
	�L�[�C�x���g�̓t�H�[�J�X�������Ă�����̂� Manager �����ڑ��荞�ށB

	�c�Ƃ������A�ċA�Œʒm���Ă͂Ȃ�Ȃ��B�}�E�X�C�x���g�Ƃ��͍ċA���Ă��܂��ƁA
	�}�E�X�J�[�\��������Ă��Ȃ��v�f���C�x���g���󂯎���Ă��܂����ƂɂȂ�B
	*/
	// ��납�烋�[�v����B��̃��m����ɕ`�悳���̂ŁA���̕������R�B
	// TODO: Z�I�[�_�[�͕ʂ̃��X�g�ɂ����ق��������C������E�E�E
	//int count = GetVisualChildrenCount();
	//for (int i = count - 1; i >= 0; i--)
	//{
	//	if (GetVisualChild(i)->OnEvent(type, args)) { return true; }
	//}


	switch (type)
	{
	case detail::UIInternalEventType::Unknown:
		break;
	case detail::UIInternalEventType::MouseMove:
		if (m_isEnabled) { OnMouseMove(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseButtonDown:
		if (m_isEnabled) { OnMouseDown(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseButtonUp:
		if (m_isEnabled) { OnMouseUp(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseWheel:
		break;
	case detail::UIInternalEventType::KeyDown:
		if (m_isEnabled) { OnKeyDown(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::KeyUp:
		if (m_isEnabled) { OnKeyUp(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::TextInput:
		if (m_isEnabled) { OnTextInput(static_cast<UIKeyEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::ElapsedTime:
		break;
	case detail::UIInternalEventType::MouseEnter:	// TODO: �e�̈�
		if (m_isEnabled) { OnMouseEnter(static_cast<UIMouseEventArgs*>(args)); }
		break;
	case detail::UIInternalEventType::MouseLeave:	// TODO: �e�̈�
		if (m_isEnabled) { OnMouseLeave(static_cast<UIMouseEventArgs*>(args)); }
		break;
	default:
		break;
	}

	//if (type == EventType_MouseMove) {
	//	//RaiseEvent<MouseEventArgs>(MouseMoveEvent, this, (MouseEventArgs*)args);
	//}
	return args->handled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate()
{
	/// ���݂̃e���v���[�g����r�W���A���c���[���č\�z���܂��B
	/// ���̊֐��͕K�v�ȃ^�C�~���O�Ń��C�A�E�g�V�X�e������Ăяo����܂��B�ʏ�A�����I�ɌĂяo���K�v�͂���܂���B
	///		�Ƃ������A�Ăяo������_���B�K�����[�g����ċA�I�ɍX�V���Ȃ��ƁA�����e���܂� ApplyTemplate() ���ĂȂ���Ԃł�����ĂԂ�
	///		���[�J�����\�[�X���������X�V����Ȃ��B
	///		TODO: ������������ASetParent �����u�ԂɃ��[�J�����\�[�X���X�V�����ق����ǂ������H
	///		��������΂��� ApplyTemplate() ���Ăяo���Ă��ǂ����c ���v�͖������B

	ApplyTemplateHierarchy();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplateHierarchy()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size(
		Math::IsNaNOrInf(m_size.Width) ? m_ownerLayoutView->GetViewPixelSize().Width : m_size.Width,
		Math::IsNaNOrInf(m_size.Height) ? m_ownerLayoutView->GetViewPixelSize().Height : m_size.Height);

	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	// TODO: ��O�̕����ǂ������H
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateTransformHierarchy()
{
	if (m_parent != NULL)
	{
		m_finalGlobalRect.X = m_parent->m_finalGlobalRect.X + m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_parent->m_finalGlobalRect.Y + m_finalLocalRect.Y;
		m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// �s�����x���R�R�ō����Ă��܂�
	}
	else
	{
		m_finalGlobalRect.X = m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_finalLocalRect.Y;
		m_combinedOpacity = m_opacity;
	}
	m_finalGlobalRect.Width = m_finalLocalRect.Width;
	m_finalGlobalRect.Height = m_finalLocalRect.Height;

	// �q�v�f
	UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::RaiseEventInternal(const UIEventInfo* ev, UIEventArgs* e)
{
	LN_VERIFY_RETURN(ev != NULL);
	LN_VERIFY_RETURN(e != NULL);
	tr::TypeInfo* thisType = tr::TypeInfo::GetTypeInfo(this);

	// this �� AddHandler ����Ă���C�x���g�n���h�����Ăяo���B
	thisType->InvokeReflectionEvent(this, ev, e);

	// bubble
	if (!e->handled && m_parent != NULL)
	{
		m_parent->RaiseEventInternal(ev, e);
	}
}

LN_NAMESPACE_END
