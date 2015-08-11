
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Painter.h>
#include <Lumino/GUI/RoutedCommand.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/UIElement.h>
#include "GUIHelper.h"

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElement
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElement, CoreObject);

// Register property
LN_PROPERTY_IMPLEMENT(UIElement, SizeF, SizeProperty, "Size", m_size, SizeF::Zero, NULL);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::HorizontalAlignment, HorizontalAlignmentProperty, "HorizontalAlignment", m_horizontalAlignment, HorizontalAlignment::Stretch, NULL);
LN_PROPERTY_IMPLEMENT(UIElement, GUI::VerticalAlignment, VerticalAlignmentProperty, "VerticalAlignment", m_verticalAlignment, VerticalAlignment::Stretch, NULL);
LN_PROPERTY_IMPLEMENT(UIElement, bool, IsHitTestProperty, "IsHitTest", m_isHitTest, true, NULL);

// Register routed event
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseEnterEvent, "MouseEnter", MouseEnter);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseLeaveEvent, "MouseLeave", MouseLeave);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseMoveEvent, "MouseMove", MouseMove);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseDownEvent, "MouseDown", MouseDown);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, MouseEventArgs, MouseUpEvent, "MouseUp", MouseUp);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, CanExecuteRoutedCommandEventArgs, CanExecuteRoutedCommandEvent, "CanExecuteRoutedCommand", CanExecuteRoutedCommand);
LN_ROUTED_EVENT_IMPLEMENT(UIElement, ExecuteRoutedCommandEventArgs, ExecuteRoutedCommandEvent, "ExecuteRoutedCommand", ExecuteRoutedCommand);


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
int UIElement::GetVisualChildrenCount() const
{
	return m_visualChildren.GetCount();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* UIElement::GetVisualChild(int index) const
{
	/*	GetVisualChildrenCount() �� GetVisualChild() ��g�ݍ��킹�ăr�W���A���ɃA�N�Z�X����̂� WPF �Ɠ�������B
		�{���C�u�����ł͈ȑO�� m_visualChildren �� protected �Ō��J���āA
		�T�u�N���X������ m_visualChildren �ɒǉ�������s���Ă����B
		���������̕��@���ƁAItemsControl �� Panel ���ł��Ȃ�p�ɂ� m_visualChildren �ւ� Add Remove ���s���邱�ƂɂȂ����B
		�܂��AItemsControl �� Panel �������Ă���_���v�f�ƃr�W���A���v�f����ɓ�������K�v������A�R�ꂪ�S�z�B
	*/
	return m_visualChildren.GetAt(index);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::UpdateLayout()
{
	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	// TODO: ��O�̕����ǂ������H
	if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	MeasureLayout(m_size);
	ArrangeLayout(RectF(0, 0, m_size.Width, m_size.Height));
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
void UIElement::AddVisualChild(UIElement* element)
{
	LN_CHECK_ARGS_RETURN(element != NULL);
	LN_THROW(element->GetParent() == NULL, InvalidOperationException);	// ���ɐe�v�f��������

	m_visualChildren.Add(element);		// m_visualChildren �ɒǉ��������̂� OnEvent �� Render ���Ă΂��悤�ɂȂ�
	element->SetParent(this);			// Visual�c���[�ł��A�_���I�Ȑe�̈����͋��ʁB
	element->SetTemplateModified(true);	// �e���v���[�g���č\�z����K�v�����肻��
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::RemoveVisualChild(UIElement* element)
{
	LN_CHECK_ARGS_RETURN(element != NULL);
	LN_THROW(element->GetParent() == this, InvalidOperationException);	// this ���e�v�f�ł���͂�

	m_visualChildren.Remove(element);
	element->SetParent(NULL);
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
SizeF UIElement::ArrangeOverride(const SizeF& finalSize)
{
	return finalSize;
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
		UIElement* e = GetVisualChild(i)->CheckMouseHoverElement(globalPt);
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

	OnLayoutUpdated();
}




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::SetTemplateBinding(const Property* thisProp, const String& srcPropPath/*, UIElement* rootLogicalParent*/)
{
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
		m_rootLogicalParent->PropertyChangedForTemplateBindings -= m_templateBindingHandler;
	}

	// delegate �����쐬�ł���΍�� (����)
	if (m_templateBindingHandler.IsEmpty()) {
		m_templateBindingHandler = LN_CreateDelegate(this, &UIElement::TemplateBindingSource_PropertyChanged);
	}

	// �v�f��ێ�����
	m_rootLogicalParent = logicalParent;

	if (m_rootLogicalParent != NULL)
	{
		// �V�������ɃC�x���g�n���h����o�^����
		m_rootLogicalParent->PropertyChangedForTemplateBindings += m_templateBindingHandler;

		// TemplateBinding ���X�V����
		for (TemplateBindingInfo& info : m_templateBindingInfoList)
		{
			// TODO: TypeInfo::FindProperty(m_rootLogicalParent, name) �̂ق������R�H
			auto* prop = m_rootLogicalParent->GetThisTypeInfo()->FindProperty(info.SourcePropPath);
			if (prop != NULL)
			{
				info.ThisProp->SetValue(this, prop->GetValue(m_rootLogicalParent));
			}
		}
	}

	// �q�v�f�ւ������_���e�v�f���Z�b�g����
	GUIHelper::ForEachVisualChildren(this, [logicalParent](UIElement* child) { child->UpdateTemplateLogicalParentHierarchy(logicalParent); });
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::OnPropertyChanged(PropertyChangedEventArgs* e)
{
	CoreObject::OnPropertyChanged(e);
	
	// TemplateBinding �ɓ`����
	PropertyChangedForTemplateBindings(e);
	
	// �X�^�C���̎��g���K�[�ɓ`����
	if (m_style != NULL) {
		m_style->NortifyTargetObjectPropertyChanged(this, e);
	}
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

	// �q�v�f
	GUIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElement::Render()
{
	// �q�v�f
	GUIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->Render(); });

	// TODO: Panel �Ƃ��A�`���K�v�̖������͓̂��ꂠ�����ɂ��āAPainter ���Ȃ��悤�ɂ�����
	Graphics::Painter painter(m_manager->GetGraphicsManager());
	painter.PushTransform(Matrix::Translation(Vector3(m_finalGlobalRect.X, m_finalGlobalRect.Y, 0)));	// TODO: ���� Transform �� DrawingContext �Ƃ��x�[�X�N���X�ɍ���ĉB�������B
	painter.SetProjection(Size(640, 480), 0, 1000);	// TODO
	OnRender(&painter);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool UIElement::OnEvent(EventType type, RoutedEventArgs* args)
{
	// ��납�烋�[�v����B��̃��m����ɕ`�悳���̂ŁA���̕������R�B
	// TODO: Z�I�[�_�[�͕ʂ̃��X�g�ɂ����ق��������C������E�E�E
	int count = GetVisualChildrenCount();
	for (int i = count - 1; i >= 0; i--)
	{
		if (GetVisualChild(i)->OnEvent(type, args)) { return true; }
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
		GUIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->ApplyTemplate(); });
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
	// �Â��v�f������΂��낢���������
	if (m_templateChild != NULL)
	{
		RemoveVisualChild(m_templateChild);
	}

	if (child != NULL)
	{
		AddVisualChild(child);
		m_templateChild = child;

		// �e���v���[�g�̃��[�g�_���v�f���ς�������Ƃ��Am_templateChild �̎q�v�f�ɒʒm����B
		// ���̒��� TemplateBinding �Ȃǂ̃����N���s����B
		m_templateChild->UpdateTemplateLogicalParentHierarchy(this);
	}
}

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
		if (info.SourcePropPath == e->ChangedProperty->GetName()) {
			info.ThisProp->SetValue(this, e->NewValue);
			//SetValue(info.ThisProp, e);
		}
	}
}


} // namespace GUI
} // namespace Lumino
