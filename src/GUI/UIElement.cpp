
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>

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


} // namespace GUI
} // namespace Lumino
