
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		1 �̃R���e���c�����R���g���[����\���܂��B
*/
class ContentControl
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentControl);
public:
	ContentControl(GUIManager* manager);
	virtual ~ContentControl();


	/// ���̃R���g���[���̃R���e���c��ݒ肵�܂��B
	void SetContent(Variant value);

	virtual void Render();

protected:
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
	virtual void OnRender();
	virtual bool OnEvent(EventType type, EventArgs* args);

	// IAddChild
	virtual void AddChild(const Variant& value) { SetContent(value); }
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }

protected:
	// override UIElement
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);

private:
	Variant		m_content;		
	UIElement*	m_childElement;	///< m_content �� UIElement �ł���΂�����w��
};

/**
	@brief		���[�g�v�f�B
*/
class RootPane
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RootPane);
public:
	RootPane(GUIManager* manager);
	virtual ~RootPane();

	//virtual void Render();

protected:
	//virtual void OnRender() {}

	friend class GUIManager;

private:
};


} // namespace GUI
} // namespace Lumino
