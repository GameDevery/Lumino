
#include "../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/VisualState.h>	// for button

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElement
//=============================================================================

const String	UIElement::SizeProperty(_T("Size"));

const String	UIElement::MouseMoveEvent(_T("MouseMove"));
const String	UIElement::MouseLeaveEvent(_T("MouseLeave"));
const String	UIElement::MouseEnterEvent(_T("MouseEnter"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
	, m_parent(NULL)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
	, m_rootLogicalParent(NULL)
{
	LN_SAFE_ADDREF(m_manager);

	// �v���p�e�B�̓o�^
	LN_DEFINE_PROPERTY(UIElement, SizeF, SizeProperty, &UIElement::SetSize, &UIElement::GetSize, SizeF());
	//RegisterProperty(SizeProperty, SizeF(NAN, NAN));

	// �C�x���g�̓o�^
	LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent, &UIElement::OnMouseMove, &UIElement::CallMouseMoveEvent);

	// �폜�\��
	//m_eventDataStore.Add(MouseMoveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseLeaveEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
	//m_eventDataStore.Add(MouseEnterEvent, LN_NEW Event02<CoreObject*, MouseEventArgs*>());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::~UIElement()
{
	LN_SAFE_RELEASE(m_localResource);
	LN_SAFE_RELEASE(m_combinedLocalResource);
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate()
{
	CombinedLocalResource* parentResource = NULL;
	if (m_parent != NULL) {
		parentResource = m_parent->m_combinedLocalResource;
	}
	else {
		parentResource = m_manager->GetRootCombinedResource();	// �e�v�f��������� Manager �̃��\�[�X���g��
	}
	ApplyTemplateHierarchy(parentResource);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_finalRect.Contains(globalPt)) {
		return this;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// �e�v�f����q�v�f��z�u�ł���͈�(availableSize)���󂯎��ADesiredSize ���X�V����B
	// �@ Pane �\[measure()   �c ���͈͓̔��Ȃ�z�u�ł����]�� Button
	// �A Pane ��[DesiredSize �c ���Ⴀ���̃T�C�Y�ł��肢���܂�]�\ Button
	// �B Pane �\[arrange()   �c ���̎q�v�f�Ƃ̌��ˍ����ōŏI�T�C�Y�̓R����]�� Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	const SizeF& size = GetSize();
	m_desiredSize.Width = std::min(size.Width, availableSize.Width);
	m_desiredSize.Height = std::min(size.Height, availableSize.Height);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ArrangeLayout(const RectF& finalRect)
{
	// finalRect �͂��̗v�f��z�u�ł���̈�T�C�Y�B
	// �v�f�ɒ��ڐݒ肳��Ă���T�C�Y�����傫�����Ƃ�����B
	// TODO: HorizontalAlignment �����l�����āA�ŏI�I�ȍ��W�ƃT�C�Y�����肷��B
	//		 ���̗v�f�̃T�C�Y���ȗ�����Ă���΁AStretch �Ȃ�T�C�Y�͍ő�ɁA����ȊO�Ȃ�ŏ��ɂȂ�B

	m_finalRect = finalRect;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateBinding(Property* thisProp, const String& srcPropPath, UIElement* rootLogicalParent)
{
	if (m_rootLogicalParent == NULL)
	{
		// TODO: �����́H
		rootLogicalParent->PropertyChanged.AddHandler(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
		m_rootLogicalParent = rootLogicalParent;
	}
	else if (m_rootLogicalParent != rootLogicalParent) {
		// �����Ă͂Ȃ�Ȃ��B
		// VisualTree �v�f���قȂ� ���[�g Logcal �v�f�Ƀo�C���h���悤�Ƃ����B
		LN_THROW(0, InvalidOperationException);
	}

	TemplateBindingInfo info;
	info.ThisProp = thisProp;
	info.SourcePropPath = srcPropPath;
	m_templateBindingInfoList.Add(info);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.Width, size.Height));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	OnRender();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(EventType type, EventArgs* args)
{
	switch (type)
	{
	case Lumino::GUI::EventType_Unknown:
		break;
	case Lumino::GUI::EventType_MouseMove:
		OnMouseMove(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonDown:
		break;
	case Lumino::GUI::EventType_MouseButtonUp:
		break;
	case Lumino::GUI::EventType_MouseWheel:
		break;
	case Lumino::GUI::EventType_KeyDown:
		break;
	case Lumino::GUI::EventType_KeyUp:
		break;
	case Lumino::GUI::EventType_ElapsedTime:
		break;
	case Lumino::GUI::EventType_MouseEnter:
		//RaiseEvent(MouseEnterEvent, this, args);
		break;
	case Lumino::GUI::EventType_MouseLeave:
		//RaiseEvent(MouseLeaveEvent, this, args);
		break;
	default:
		break;
	}

	//if (type == EventType_MouseMove) {
	//	//RaiseEvent<MouseEventArgs>(MouseMoveEvent, this, (MouseEventArgs*)args);
	//}
	return args->Handled;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parent, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parent);
	}
	OnApplyTemplate(m_combinedLocalResource);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::TemplateBindingSource_PropertyChanged(CoreObject* sender, PropertyChangedEventArgs* e)
{
	LN_FOREACH(TemplateBindingInfo& info, m_templateBindingInfoList)
	{
		if (info.SourcePropPath == e->PropertyName) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
	//printf("TemplateBindingSource_PropertyChanged\n");
	//_tprintf(L"%s:%d\n", e->PropertyName.GetCStr(), sender->GetValue(e->PropertyName));
}

//=============================================================================
// Decorator
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(Decorator);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::Decorator(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Decorator::~Decorator()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::SetChild(UIElement* element)
{
	// ���ɕʂ̎q�v�f�ƂȂ��Ă���ꍇ�̓G���[
	if (element != NULL && element->GetParent() != NULL) {
		LN_THROW(0, InvalidOperationException, _T("Specified element is already a child element of another element."));
	}

	m_child = element;

	if (m_child != NULL)
	{
		m_child->SetParent(this);
		m_child->ApplyTemplate();	// �q�v�f�̃e�[�}�𒼂��ɍX�V
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::GetChild() const
{
	return m_child;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* Decorator::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_child != NULL) {	// �q�v�f��D��
		UIElement* e = m_child->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::AddChild(const Variant& value)
{
	UIElement* e = dynamic_cast<UIElement*>(value.GetObject());
	SetChild(e);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Decorator::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);
	if (m_child != NULL) {
		m_child->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void Decorator::AddChildToVisualTree(UIElement* element)
//{
//
//}

//=============================================================================
// ButtonChrome
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonChrome);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonChrome);
const String	ButtonChrome::IsMouseOverProperty(_T("IsMouseOver"));
const String	ButtonChrome::FrameWidthProperty(_T("FrameWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8.0f)
	, m_isMouseOver(false)
{
	// �v���p�e�B�̓o�^
	LN_DEFINE_PROPERTY(ButtonChrome, bool, IsMouseOverProperty, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver, false);

	// TODO
	// �{�^���̃X�^�C���ƃe���v���[�g
	// https://msdn.microsoft.com/ja-jp/library/ms753328%28v=vs.110%29.aspx
	RefPtr<VisualStateManager> vsm(LN_NEW VisualStateManager());

	RefPtr<VisualStateGroup> group1(LN_NEW VisualStateGroup(_T("CommonStates")));


	RefPtr<VisualState> state1(LN_NEW VisualState(_T("Normal")));

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::~ButtonChrome()
{
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::SetValue(const String& propertyName, const Variant& value)
{
	if (propertyName == FrameWidthProperty) { m_frameWidth = value.GetFloat(); return; }
	Decorator::SetValue(propertyName, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant ButtonChrome::GetValue(const String& propertyName) const
{
	if (propertyName == FrameWidthProperty) { return Variant(m_frameWidth); }
	return Decorator::GetValue(propertyName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnApplyTemplate(CombinedLocalResource* localResource)
{
	m_brush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalFrameBrush")));
	m_bgBrush = static_cast<Graphics::TextureBrush*>(localResource->GetItem(_T("ButtonNormalBackgroundBrush")));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ButtonChrome::OnRender()
{
	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO

	RectF bgRect = m_finalRect;
	RectF rect = m_finalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter.SetBrush(m_bgBrush);
		painter.DrawRectangle(bgRect);
	}

	painter.SetBrush(m_brush);
	painter.DrawFrameRectangle(rect, m_frameWidth);
}

//=============================================================================
// ContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentPresenter);

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

//=============================================================================
// ItemsPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ItemsPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ItemsPresenter);

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

//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::Control(GUIManager* manager)
	: UIElement(manager)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Control::~Control()
{

}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternal()
//{
//	return ApplyTemplateInternalMain(ControlTemplateTypeName);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//bool Control::ApplyTemplateInternalMain(const String& typeName)
//{
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(typeName, &t))
//	{
//		t->Apply(this);
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	UIElement::ApplyTemplateHierarchy(parent);

	// ControlTemplate �̓K�p����
	ControlTemplate* t;
	if (m_combinedLocalResource->TryGetControlTemplate(GetTypeID(), &t)) {
		t->Apply(this);
	}
}


//=============================================================================
// ContentControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentControl);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::~ContentControl()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::SetContent(Variant value)
{
	m_content = value;

	// Object �^�ł���� UIElement �ƂƂ��Ď��o���Ă݂�
	if (m_content.GetType() == VariantType_Object) {
		m_childElement = dynamic_cast<UIElement*>(m_content.GetObject());
	}

	if (m_childElement != NULL)
	{
		LN_THROW(m_childElement->GetParent() == NULL, InvalidOperationException);	// ���ɐe�v�f��������
		m_childElement->SetParent(this);

		// �q�v�f�̃e�[�}�𒼂��ɍX�V
		if (m_combinedLocalResource != NULL) {
			m_childElement->ApplyTemplate();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::Render()
{
	OnRender();
	if (m_childElement != NULL) {
		m_childElement->Render();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* ContentControl::CheckMouseHoverElement(const PointF& globalPt)
{
	if (m_childElement != NULL) {	// �q�v�f��D��
		UIElement* e = m_childElement->CheckMouseHoverElement(globalPt);
		if (e != NULL) { return e; }
	}
	return UIElement::CheckMouseHoverElement(globalPt);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::MeasureLayout(const SizeF& availableSize)
{
	if (m_childElement != NULL) {
		m_childElement->MeasureLayout(availableSize);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::MeasureLayout(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ArrangeLayout(const RectF& finalRect)
{
	if (m_childElement != NULL) {
		m_childElement->ArrangeLayout(finalRect);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::ArrangeLayout(finalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::OnRender()
{
	if (m_childElement != NULL) {
		m_childElement->Render();	//TODO: Render ������Ă�ł�E�E�E
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ContentControl::OnEvent(EventType type, EventArgs* args)
{
	if (m_childElement != NULL) {
		if (m_childElement->OnEvent(type, args)) { return true; }
	}
	return Control::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::ApplyTemplateHierarchy(CombinedLocalResource* parent)
{
	Control::ApplyTemplateHierarchy(parent);
	if (m_childElement != NULL) {
		m_childElement->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
	}
}

//=============================================================================
// RootPane
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(RootPane);
LN_UI_ELEMENT_SUBCLASS_IMPL(RootPane);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::RootPane(GUIManager* manager)
	: ContentControl(manager)
{
	//// �f�t�H���g�e�[�}
	//m_combinedLocalResource = LN_NEW CombinedLocalResource();
	//m_combinedLocalResource->Combine(NULL, m_manager->GetDefaultTheme());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane::~RootPane()
{

}

//=============================================================================
// Button
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(Button);
LN_UI_ELEMENT_SUBCLASS_IMPL(Button);
const String	Button::IsMouseOverProperty(_T("IsMouseOver"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManager* manager)
	: ContentControl(manager)
	, m_isMouseOver(false)
{
	// �v���p�e�B�̓o�^
	LN_DEFINE_PROPERTY(Button, bool, IsMouseOverProperty, NULL, &Button::IsMouseOver, false);

	//m_chrome.Attach(LN_NEW ButtonChrome(manager));
	//SetContent(Variant(m_chrome));

	//MouseMove += LN_CreateDelegate(this, &Button::UIElement_MouseMove);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::~Button()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnClick()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Button::OnEvent(EventType type, EventArgs* args)
{

	//

	//if (m_childElement != NULL) {
	//	if (m_childElement->OnEvent(type, args)) { return true; }
	//}
	return ContentControl::OnEvent(type, args);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnRender()
{
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Button::OnMouseMove(MouseEventArgs* e)
{
	m_isMouseOver = true;
	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
	e->Handled = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e)
//{
//	m_isMouseOver = true;
//	OnPropertyChanged(IsMouseOverProperty, m_isMouseOver);
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void Button::Render()
//{
//	printf("Button::Render()\n");
//	ContentControl::Render();
//}

} // namespace GUI
} // namespace Lumino
