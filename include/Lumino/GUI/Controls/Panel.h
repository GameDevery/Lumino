
#pragma once
#include "../UIElement.h"
#include "../UIElementCollection.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		Panel �v�f�̊�{�N���X�ł��B
*/
class Panel
	: public UIElement
{
	// [ContentProperty("Children")]
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Panel);
public:
	Panel(GUIManager* manager);
	virtual ~Panel();

	UIElementCollection* GetChildren() { return m_children; }


public:
#if 0
	// �q�v�f�����v�f�� override
	virtual UIElement* CheckMouseHoverElement(const PointF& globalPt);
	virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);
	virtual void Render();

	//virtual void ApplyTemplate(CombinedLocalResource* parent);
	//virtual void MeasureLayout(const SizeF& availableSize);
	//virtual void ArrangeLayout(const RectF& finalLocalRect);
#endif

	// IAddChild
	virtual void AddChild(const Variant& value);
	virtual void AddText(const String& text) { LN_THROW(0, InvalidOperationException); }


private:
	void Children_ItemAdded(UIElement* item);
	void Children_ItemRemoved(UIElement* item);

protected:
	RefPtr<UIElementCollection>		m_children;
};

class PilePanel
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(PilePanel);
public:
	/**
		@brief	PilePanel ���쐬���܂��B
		@return	�쐬���ꂽ�C���X�^���X�̃|�C���^�B�g���I������ Release() ���Ăяo���āA�Q�Ƃ�������Ă��������B	
	*/
	PilePanel* Create();

protected:
	PilePanel(GUIManager* manager);
	virtual ~PilePanel();
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
};

} // namespace GUI
} // namespace Lumino
