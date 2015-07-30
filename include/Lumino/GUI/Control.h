
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{
class ContentPresenter;

/**
	@brief		���[�U�[����ɂ���ē���̓�����s�� UI �v�f�̊�{�N���X�ł��B
*/
class Control
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Control);
public:
	LN_PROPERTY(ControlTemplate*, TemplateProperty);

public:
	Control(GUIManager* manager);
	virtual ~Control();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	void SetTemplate(ControlTemplate* controlTemplate);	// TODO: OnTemplatePropertyChanged
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }

	/** @} */

protected:
	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

	//friend class UIElementFactory;

	//friend class ControlTemplate;
	//friend class DataTemplate;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// Contro ���͎̂q�v�f�������Ȃ��B�T�u�N���X�ł���ɃI�[�o�[���C�h�����B

	//virtual void ApplyTemplate(CombinedLocalResource* parent);

	//	virtual bool ApplyTemplateInternal();
	//	bool ApplyTemplateInternalMain(const String& typeName);
	//
	//
	//protected:
	//	// override UIElement
	//	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
private:
	RefPtr<ControlTemplate>	m_controlTemplate;
};


} // namespace GUI
} // namespace Lumino
