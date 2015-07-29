
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
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
LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElement, CoreObject);


LN_PROPERTY_IMPLEMENT(UIElement, SizeF, Size, m_size, SizeF::Zero);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::HorizontalAlignment, HorizontalAlignment, m_horizontalAlignment, HorizontalAlignment::Stretch);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::VerticalAlignment, VerticalAlignment, m_verticalAlignment, VerticalAlignment::Stretch);
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsHitTest, m_isHitTest, true);

////const PropertyID	UIElement::SizeProperty(_T("Size"));
//LN_DEFINE_PROPERTY_2(UIElement, SizeF, SizeProperty, "Size", SizeF::Zero, &UIElement::SetSize, &UIElement::GetSize);
//LN_DEFINE_PROPERTY_ENUM_2(UIElement, HorizontalAlignment, HorizontalAlignmentProperty, "HorizontalAlignment", HorizontalAlignment::Stretch, &UIElement::SetHorizontalAlignment, &UIElement::GetHorizontalAlignment);
//LN_DEFINE_PROPERTY_ENUM_2(UIElement, VerticalAlignment, VerticalAlignmentProperty, "VerticalAlignment", VerticalAlignment::Stretch, &UIElement::SetVerticalAlignment, &UIElement::GetVerticalAlignment);

//const PropertyID	UIElement::HorizontalAlignmentProperty(_T("HorizontalAlignment"));
//const PropertyID	UIElement::VerticalAlignmentProperty(_T("VerticalAlignment"));

//const RoutedEvent*	UIElement::MouseMoveEvent(_T("MouseMove"));
//const RoutedEvent*	UIElement::MouseLeaveEvent(_T("MouseLeave"));
//const RoutedEvent*	UIElement::MouseEnterEvent(_T("MouseEnter"));
//const RoutedEvent*	UIElement::MouseDownEvent(_T("MouseDown"));
//const RoutedEvent*	UIElement::MouseUpEvent(_T("MouseUp"));
//
//const RoutedEvent*	UIElement::CanExecuteRoutedCommandEvent(_T("CanExecuteRoutedCommandEvent"));
//const RoutedEvent*	UIElement::ExecuteRoutedCommandEvent(_T("ExecuteRoutedCommandEvent"));
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);

LN_DEFINE_ROUTED_EVENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent, "CanExecuteRoutedCommand", Handler_CanExecuteRoutedCommandEvent);
LN_DEFINE_ROUTED_EVENT(UIElement, ExecuteRoutedCommandEventArgs, ExecuteRoutedCommandEvent, "ExecuteRoutedCommand", Handler_ExecuteRoutedCommandEvent);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement::UIElement(GUIManager* manager)
	: m_manager(manager)
	, m_parent(NULL)
	, m_localResource(NULL)
	, m_combinedLocalResource(NULL)
	, m_horizontalAlignment(HorizontalAlignment::Stretch)
	, m_verticalAlignment(VerticalAlignment::Stretch)
	, m_rootLogicalParent(NULL)
	, m_templateParent(NULL)
{
	LN_SAFE_ADDREF(m_manager);

	// �C�x���g�̓o�^
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseMoveEvent, [](UIElement* target, MouseEventArgs* e) { target->MouseMove(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseDownEvent,	[](UIElement* target, MouseEventArgs* e) { target->MouseDown(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, MouseEventArgs, MouseUpEvent,		[](UIElement* target, MouseEventArgs* e) { target->MouseUp(e); });
	//
	//LN_DEFINE_ROUTED_EVENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent,	[](UIElement* target, CanExecuteRoutedCommandEventArgs* e) { target->Handler_CanExecuteRoutedCommandEvent(e); });
	//LN_DEFINE_ROUTED_EVENT(UIElement, ExecuteRoutedCommandEventArgs,	ExecuteRoutedCommandEvent,		[](UIElement* target, ExecuteRoutedCommandEventArgs* e) { target->Handler_ExecuteRoutedCommandEvent(e); });

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
void UIElement::InitializeComponent()
{
	// ���̎��_�ł͉����̎q�v�f�ɂ͂Ȃ��Ă��Ȃ��͂��Ȃ̂ŁA
	// m_combinedLocalResource �̓O���[�o�����\�[�X�ƂȂ�B
	UpdateLocalResource();

	// �C���X�^���X�������N���X�^���� Style ����������B
	// ���������ꍇ�̓x�[�X�N���X�� Style ����������c�Ƃ������Ƃ͂��Ȃ��B(WPF �̎d�l)
	m_style = m_combinedLocalResource->FindStyle(GetThisTypeInfo());
	if (m_style != NULL) {
		m_style->Apply(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::CaptureMouse() 
{
	m_manager->CaptureMouse(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ReleaseMouseCapture()
{
	m_manager->ReleaseMouseCapture(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::ApplyTemplate()
{
	//CombinedLocalResource* parentResource = NULL;
	//if (m_parent != NULL) {
	//	parentResource = m_parent->m_combinedLocalResource;
	//}
	//else {
	//	parentResource = m_manager->GetRootCombinedResource();	// �e�v�f��������� Manager �̃��\�[�X���g��
	//}
	ApplyTemplateHierarchy(/*parentResource*/);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLocalResource()
{
	// �e�́H
	CombinedLocalResource* parentResource = NULL;
	if (m_parent != NULL) {
		parentResource = m_parent->m_combinedLocalResource;
	}
	else {
		parentResource = m_manager->GetRootCombinedResource();	// �e�v�f��������� Manager �̃��\�[�X���g��
	}

	if (m_combinedLocalResource != NULL && parentResource != m_combinedLocalResource) {
		m_combinedLocalResource->Combine(parentResource, m_localResource);
	}
	else {
		LN_REFOBJ_SET(m_combinedLocalResource, parentResource);
	}

	// �����̃e���v���[�g���X�V����K�v������ꍇ
	//if (m_templateModified)
	//{
	//	// �e�́H
	//	CombinedLocalResource* parentResource = NULL;
	//	if (m_parent != NULL) {
	//		parentResource = m_parent->m_combinedLocalResource;
	//	}
	//	else {
	//		parentResource = m_manager->GetRootCombinedResource();	// �e�v�f��������� Manager �̃��\�[�X���g��
	//	}

	//	// ����
	//	if (m_combinedLocalResource != NULL && parentResource != m_combinedLocalResource) {
	//		m_combinedLocalResource->Combine(parentResource, m_localResource);
	//	}
	//	else {
	//		LN_REFOBJ_SET(m_combinedLocalResource, parentResource);
	//	}
	//	// ControlTemplate �� DataTemplate �͂��̃I�[�o�[���C�h�� Apply �����
	//	OnApplyTemplate(m_combinedLocalResource);

	//	m_templateModified = false;			// �����̃e���v���[�g���X�V����
	//	m_childTemplateModified = true;		// �������X�V������A�q���X�V����K�v������
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::CheckMouseHoverElement(const PointF& globalPt)
{
	// �q�v�f��D��
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	UIElement* e = child->CheckMouseHoverElement(globalPt);
	//	if (e != NULL) { return e; }
	//}
	//if (m_templateChild != NULL)
	//{
	//	UIElement* e = m_templateChild->CheckMouseHoverElement(globalPt);
	//	if (e != NULL) { return e; }
	//	// �q�_���v�f�� UIElement �̒S���ł͂Ȃ��B
	//	// ContetntControl ���ł��̊֐����I�[�o�[���C�h���A������Ɏ�������B
	//}
	// ��납�烋�[�v����B��̃��m����ɕ`�悳���̂ŁA���̕������R�B
	for (int i = m_visualChildren.GetCount() - 1; i >= 0; i--)
	{
		UIElement* e = m_visualChildren[i]->CheckMouseHoverElement(globalPt);
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
void UIElement::MeasureLayout(const SizeF& availableSize)
{
	// �e�v�f����q�v�f��z�u�ł���͈�(availableSize)���󂯎��ADesiredSize ���X�V����B
	// �@ Pane �\[measure()   �c ���͈͓̔��Ȃ�z�u�ł����]�� Button
	// �A Pane ��[DesiredSize �c ���Ⴀ���̃T�C�Y�ł��肢���܂�]�\ Button		�����̎��_�� inf ��Ԃ����Ƃ����蓾��B
	// �B Pane �\[arrange()   �c ���̎q�v�f�Ƃ̌��ˍ����ōŏI�T�C�Y�̓R����]�� Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm


	m_desiredSize = MeasureOverride(availableSize);

	//const SizeF& size = GetSize();
	//m_desiredSize.Width = std::min(size.Width, m_desiredSize.Width);
	//m_desiredSize.Height = std::min(size.Height, m_desiredSize.Height);

	// �q�v�f
	//if (m_templateChild != NULL) {
	//	m_templateChild->MeasureLayout(m_desiredSize);
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->MeasureLayout(m_desiredSize);
	//}
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

	SizeF renderSize = ArrangeOverride(finalLocalRect.GetSize());
	m_finalLocalRect.X = finalLocalRect.X;
	m_finalLocalRect.Y = finalLocalRect.Y;
	m_finalLocalRect.Width = renderSize.Width;
	m_finalLocalRect.Height = renderSize.Height;

	// �q�v�f (������������� m_finalLocalRect �̗̈�ɏd�˂���悤�ɔz�u�����)
	//if (m_templateChild != NULL) {
	//	RectF rect(0, 0, m_finalLocalRect.Width, m_finalLocalRect.Height);
	//	m_templateChild->ArrangeLayout(rect);
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	child->ArrangeLayout(m_finalLocalRect;
	//}

	LayoutUpdated(NULL);
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
	size.Width = Math::IsNaNOrInf(m_size.Width) ? 0.0f : size.Width;
	size.Height = Math::IsNaNOrInf(m_size.Height) ? 0.0f : size.Height;
	size.Width = std::min(size.Width, constraint.Width);
	size.Height = std::min(size.Height, constraint.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/)
{
	//if (m_rootLogicalParent == NULL)
	//{
	//	// TODO: �����́H
	//	rootLogicalParent->PropertyChanged.AddHandler(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
	//	m_rootLogicalParent = rootLogicalParent;
	//}
	//else if (m_rootLogicalParent != rootLogicalParent) {
	//	// �����Ă͂Ȃ�Ȃ��B
	//	// VisualTree �v�f���قȂ� ���[�g Logcal �v�f�Ƀo�C���h���悤�Ƃ����B
	//	LN_THROW(0, InvalidOperationException);
	//}

	TemplateBindingInfo info;
	info.ThisProp = thisProp;
	info.SourcePropPath = srcPropPath;
	m_templateBindingInfoList.Add(info);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateTemplateLogicalParentHierarchy(UIElement* logicalParent)
{
	if (m_rootLogicalParent == logicalParent) { return; }

	// �Â��ق�����C�x���g�n���h������������
	// (m_rootLogicalParent �ɉ����Z�b�g����Ă���̂�����Am_templateBindingHandler �� NULL �ł͂Ȃ��͂�)
	if (m_rootLogicalParent != NULL) {
		m_rootLogicalParent->PropertyChanged -= m_templateBindingHandler;
	}

	// delegate �����쐬�ł���΍�� (����)
	if (m_templateBindingHandler.IsEmpty()) {
		m_templateBindingHandler = LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged);
	}

	// �v�f��ێ�����
	m_rootLogicalParent = logicalParent;

	// �V�������ɃC�x���g�n���h����o�^����
	if (m_rootLogicalParent) {
		m_rootLogicalParent->PropertyChanged += m_templateBindingHandler;
	}

	// �q�v�f�ւ������_���e�v�f���Z�b�g����
	for (UIElement* child : m_visualChildren) {
		child->UpdateTemplateLogicalParentHierarchy(logicalParent);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnPropertyChanged(const String& name, const Variant& newValue)
{
	CoreObject::OnPropertyChanged(name, newValue);
	if (m_style != NULL)
	{
		RefPtr<PropertyChangedEventArgs> args(m_manager->GetEventArgsPool()->Create<PropertyChangedEventArgs>(name, newValue));
		m_style->InvoleTriggers(CoreObject::PropertyChangedEvent, args, this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	SizeF size = GetSize();

	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	// TODO: ��O�̕����ǂ������H
	if (Math::IsNaNOrInf(size.Width) || Math::IsNaNOrInf(size.Height)) { return; }

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
	}
	else
	{
		m_finalGlobalRect.X = m_finalLocalRect.X;
		m_finalGlobalRect.Y = m_finalLocalRect.Y;
	}
	m_finalGlobalRect.Width = m_finalLocalRect.Width;
	m_finalGlobalRect.Height = m_finalLocalRect.Height;

	for(UIElement* child : m_visualChildren) {
		child->UpdateTransformHierarchy();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	// �q�v�f
	//if (m_templateChild != NULL) {
	//	m_templateChild->Render();
	//}
	LN_FOREACH(UIElement* child, m_visualChildren) {
		child->Render();
	}

	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO
	OnRender(&painter);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(EventType type, RoutedEventArgs* args)
{
	// �q�v�f
	//if (m_templateChild != NULL) {
	//	if (m_templateChild->OnEvent(type, args)) { return true; }
	//}
	//LN_FOREACH(UIElement* child, m_visualChildren) {
	//	if (child->OnEvent(type, args)) { return true; }
	//}
	// ��납�烋�[�v����B��̃��m����ɕ`�悳���̂ŁA���̕������R�B
	for (int i = m_visualChildren.GetCount() - 1; i >= 0; i--)
	{
		if (m_visualChildren[i]->OnEvent(type, args)) { return true; }
	}

	switch (type)
	{
	case Lumino::GUI::EventType_Unknown:
		break;
	case Lumino::GUI::EventType_MouseMove:
		OnMouseMove(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonDown:
		OnMouseDown(static_cast<MouseEventArgs*>(args));
		break;
	case Lumino::GUI::EventType_MouseButtonUp:
		OnMouseUp(static_cast<MouseEventArgs*>(args));
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
void UIElement::ApplyTemplateHierarchy(/*CombinedLocalResource* parent*/)
{
	UpdateLocalResource();

	// �����̃e���v���[�g���X�V����K�v������ꍇ
	if (m_templateModified)
	{
		//if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
		//	m_combinedLocalResource->Combine(parent, m_localResource);
		//}
		//else {
		//	LN_REFOBJ_SET(m_combinedLocalResource, parent);
		//}
		// ControlTemplate �� DataTemplate �͂��̃I�[�o�[���C�h�� Apply �����
		OnApplyTemplate(m_combinedLocalResource);

		m_templateModified = false;			// �����̃e���v���[�g���X�V����
		m_childTemplateModified = true;		// �������X�V������A�q���X�V����K�v������
	}

	// �q�v�f�̃e���v���[�g���X�V����K�v������ꍇ (�p�t�H�[�}���X�̂���)
	if (m_childTemplateModified)
	{
		LN_FOREACH(UIElement* child, m_visualChildren) {
			child->ApplyTemplate();
			//child->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
		}
		m_childTemplateModified = false;	// �q�v�f�̃e���v���[�g���X�V����
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIElement::UpdateTemplateHierarchy()
//{
//	//// �����̃e���v���[�g�X�V���K�v
//	//if (m_templateModified)
//	//{
//	//	ApplyTemplate();
//	//}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateChild(UIElement* child)
{
	//LN_VERIFY_RETURN(child != NULL);

	// �Â��v�f������΂��낢���������
	if (m_templateChild != NULL)
	{
		m_visualChildren.Remove(m_templateChild);
		// TODO: templateBinding �p�� PropertyChanged ���Ђ悤��������Ȃ��B
		//PropertyChanged.Clear();
		//rootLogicalParent->PropertyChanged(LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged));
	}
	if (child != NULL)
	{
		RefPtr<UIElement> t(child, true);
		m_templateChild = child;
		m_visualChildren.Add(child);
		child->m_parent = this;
		child->m_templateParent = this;
		child->UpdateTemplateLogicalParentHierarchy(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void UIElement::AddVisualChild(UIElement* child)
//{
//	LN_VERIFY_RETURN(child != NULL);
//	LN_VERIFY_RETURN(child->m_parent == NULL);
//
//	RefPtr<UIElement> t(child, true);
//	m_visualChildren.Add(t);
//	child->m_parent = this;
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_CanExecuteRoutedCommandEvent(CanExecuteRoutedCommandEventArgs* e)
{
	// TODO: ���o�[�X�C�e���[�^�łȂ��ƁA�R�}���h�̃I�[�o�[���C�h�ɑΉ��ł��Ȃ�
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			e->CanExecute = command->CanExecute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled �� false �Ȃ�e UIElement �� Handler_CanExecuteRoutedCommandEvent �֗����B
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Handler_ExecuteRoutedCommandEvent(ExecuteRoutedCommandEventArgs* e)
{
	for (auto context : m_routedCommandTypeContextList)
	{
		for (auto command : context->RoutedCommandList)
		{
			command->Execute(e->Parameter);
			e->Handled = true;
		}
	}
	// e->Handled �� false �Ȃ�e UIElement �� Handler_ExecuteRoutedCommandEvent �֗����B
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::TemplateBindingSource_PropertyChanged(/*CoreObject* sender, */PropertyChangedEventArgs* e)
{
	LN_FOREACH(TemplateBindingInfo& info, m_templateBindingInfoList)
	{
		if (info.SourcePropPath == e->PropertyName) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
}

//=============================================================================
// Decorator
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Decorator, UIElement);
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
		m_child->SetTemplateModified(true);
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
void Decorator::AddChild(const Variant& value)
{
	UIElement* e = dynamic_cast<UIElement*>(value.GetObject());
	if (e == NULL) {
		LN_THROW(0, NotImplementedException);
	}
	SetChild(e);
}

#if 0
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
//void Decorator::AddVisualChild(UIElement* child)
//{
//	// Decorator �� LogicalTree = VisualTree �Ȃ͂��Ȃ̂ł���ł����͂��E�E�E
//	SetChild(child);
//}

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
#endif

//=============================================================================
// ButtonChrome
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonChrome, Decorator);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonChrome);

// Register property
LN_DEFINE_PROPERTY_2(ButtonChrome, bool, IsMouseOverProperty, "IsMouseOver", 0.0f, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver);
LN_DEFINE_PROPERTY_2(ButtonChrome, float, FrameWidthProperty, "FrameWidth", 8.0f, &ButtonChrome::SetFrameWidth, &ButtonChrome::GetFrameWidth);

//const String	ButtonChrome::IsMouseOverProperty(_T("IsMouseOver"));
//const String	ButtonChrome::FrameWidthProperty(_T("FrameWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome* ButtonChrome::Create(GUIManager* manager)
{
	auto obj = RefPtr<ButtonChrome>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonChrome::ButtonChrome(GUIManager* manager)
	: Decorator(manager)
	, m_frameWidth(8.0f)
	, m_isMouseOver(false)
{
	// �v���p�e�B�̓o�^
	//LN_DEFINE_PROPERTY(ButtonChrome, bool, IsMouseOverProperty, &ButtonChrome::SetMouseOver, &ButtonChrome::IsMouseOver, false);

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
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void ButtonChrome::SetPropertyValue(const Property* prop, const Variant& value)
//{
//	if (prop == FrameWidthProperty) { m_frameWidth = value.GetFloat(); return; }
//	Decorator::SetPropertyValue(prop, value);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//Variant ButtonChrome::GetPropertyValue(const Property* prop) const
//{
//	if (prop == FrameWidthProperty) { return Variant(m_frameWidth); }
//	return Decorator::GetPropertyValue(prop);
//}

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
void ButtonChrome::OnRender(Graphics::Painter* painter)
{
	RectF bgRect = m_finalGlobalRect;
	RectF rect = m_finalGlobalRect;

	if (!m_isMouseOver)
	{
		bgRect.Inflate(-m_frameWidth, -m_frameWidth);
		painter->SetBrush(m_bgBrush);
		painter->DrawRectangle(bgRect);
	}

	painter->SetBrush(m_brush);
	painter->DrawFrameRectangle(rect, m_frameWidth);
}

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


//=============================================================================
// Control
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Control, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(Control);

LN_DEFINE_PROPERTY_2(Control, ControlTemplate*, TemplateProperty, "Template", NULL, &Control::SetTemplate, &Control::GetTemplate);

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Control::SetTemplate(ControlTemplate* controlTemplate)
{
	m_controlTemplate = controlTemplate;
	SetTemplateModified(true);
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
void Control::OnApplyTemplate(CombinedLocalResource* localResource)
{
	// ControlTemplate �̓K�p���� (WPF �ł� ApplyTemplate �ōs���Ă���)
	if (m_controlTemplate != NULL) {
		m_controlTemplate->Apply(this);
	}

	//ControlTemplate* t;
	//if (localResource->TryGetControlTemplate(GetTypeID(), &t)) {
	//	t->Apply(this);
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::MeasureOverride(const SizeF& constraint)
{
	if (m_templateChild != NULL) {
		m_templateChild->MeasureLayout(constraint);
		return m_templateChild->GetDesiredSize();
	}
	return UIElement::MeasureOverride(constraint);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF Control::ArrangeOverride(const SizeF& finalSize)
{
	if (m_templateChild != NULL) {
		m_templateChild->ArrangeLayout(RectF(PointF::Zero, finalSize));
	}
	return UIElement::ArrangeOverride(finalSize);
}

//void Control::ApplyTemplateHierarchy(CombinedLocalResource* parent)
//{
//	// ���[�J�����\�[�X���X�V����
//	if (m_combinedLocalResource != NULL && parent != m_combinedLocalResource) {
//		m_combinedLocalResource->Combine(parent, m_localResource);
//	}
//	else {
//		LN_REFOBJ_SET(m_combinedLocalResource, parent);
//	}
//
//	// ControlTemplate �̓K�p����
//	ControlTemplate* t;
//	if (m_combinedLocalResource->TryGetControlTemplate(GetTypeID(), &t)) {
//		t->Apply(this);
//	}
//
//	OnApplyTemplate(m_combinedLocalResource);
//
//	// �q�v�f
//	if (m_templateChild != NULL) {
//		m_templateChild->ApplyTemplateHierarchy(m_combinedLocalResource);	// �ċA�I�ɍX�V����
//	}
//}
//

//=============================================================================
// ContentControl
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ContentControl, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(ContentControl);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContentControl::ContentControl(GUIManager* manager)
	: Control(manager)
	, m_childElement(NULL)
	, m_contentPresenter(NULL)
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
	if (m_content.GetType() == VariantType_Object)
	{
		UIElement* element = dynamic_cast<UIElement*>(m_content.GetObject());
		if (element != NULL) {
			m_childElement = element;
		}
		LN_THROW(m_childElement->GetParent() == NULL, InvalidOperationException);	// ���ɐe�v�f��������
	}

	//m_content.SetFloat(0);

	// m_contentPresenter ������΂��̎��_�ŃZ�b�g���Ă��܂�
	if (m_childElement != NULL &&
		m_contentPresenter != NULL)
	{
		m_contentPresenter->SetContent(m_childElement);
		m_childElement->SetTemplateModified(true);
	}
	else {
		// m_contentPresenter �� NULL �ł���Ƃ������Ƃ́A�܂� VisualTree �̍\�z���I����Ă��Ȃ��Ƃ������ƁB
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContentControl::PollingTemplateChildCreated(UIElement* newElement)
{
	ContentPresenter* presenter = dynamic_cast<ContentPresenter*>(newElement);
	if (presenter != NULL)
	{
		m_contentPresenter = presenter;

		// m_childElement ������΂��̎��_�ŃZ�b�g���Ă��܂�
		if (m_childElement != NULL &&
			m_contentPresenter != NULL)
		{
			m_contentPresenter->SetContent(m_childElement);
		}
	}

}

#if 0
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
void ContentControl::ArrangeLayout(const RectF& finalLocalRect)
{
	if (m_childElement != NULL) {
		m_childElement->ArrangeLayout(finalLocalRect);	// ���ɘg�Ƃ��Ȃ��̂ł��̂܂܂̃T�C�Y��n����
	}
	Control::ArrangeLayout(finalLocalRect);
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
bool ContentControl::OnEvent(EventType type, RoutedEventArgs* args)
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
#endif

//=============================================================================
// RootPane
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(RootPane, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(RootPane);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RootPane* RootPane::Create(GUIManager* manager)
{
	auto obj = RefPtr<RootPane>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

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

LN_CORE_OBJECT_TYPE_INFO_IMPL(Button, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(Button);
const String	Button::IsMouseOverProperty(_T("IsMouseOver"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button* Button::Create(GUIManager* manager)
{
	auto obj = RefPtr<Button>::Create(manager);
	obj->InitializeComponent();
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Button::Button(GUIManager* manager)
	: ContentControl(manager)
	, m_isMouseOver(false)
{
	// �v���p�e�B�̓o�^
	//LN_DEFINE_PROPERTY(Button, bool, IsMouseOverProperty, NULL, &Button::IsMouseOver, false);

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
bool Button::OnEvent(EventType type, RoutedEventArgs* args)
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
void Button::OnRender(Graphics::Painter* painter)
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
