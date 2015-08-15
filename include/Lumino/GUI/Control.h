
#pragma once
#include "UIElement.h"
#include "VisualState.h"

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
	LN_PROPERTY(Graphics::BrushPtr,		BackgroundProperty);			/**< Background �v���p�e�B�̎��ʎq */
	LN_PROPERTY(ControlTemplate*,		TemplateProperty);
	LN_PROPERTY(VisualStateGroupList*,	VisualStateGroupsProperty);

public:
	Control(GUIManager* manager);
	virtual ~Control();

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** �R���g���[���̔w�i�̕`��Ɏg�p����u���V��ݒ肵�܂��B*/
	void SetBackground(Graphics::BrushPtr value) { SetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty, value); }

	/** �R���g���[���̔w�i�̕`��Ɏg�p����u���V���擾���܂��B*/
	Graphics::BrushPtr GetBackground() const { return GetTypedPropertyValue<Graphics::BrushPtr>(BackgroundProperty); }



	

	void SetTemplate(ControlTemplate* controlTemplate);	// TODO: OnTemplatePropertyChanged
	ControlTemplate* GetTemplate() const { return m_controlTemplate; }


	/** ���� Control �Ɋ��蓖�Ă��Ă��� VisualStateGroup �̃��X�g���擾���܂��B*/
	VisualStateGroupList* GetVisualStateGroups() const { return m_visualStateGroupList; }

	/** @} */

protected:
	// override UIElement
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void OnRender(Graphics::Painter* painter);

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
	Graphics::BrushPtr	m_background;
	RefPtr<ControlTemplate>	m_controlTemplate;
	RefPtr<VisualStateGroupList>	m_visualStateGroupList;
};


} // namespace GUI
} // namespace Lumino
