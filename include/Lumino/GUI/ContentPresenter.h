
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief	ContentControl �̃R���e���c��\�����܂��B
*/
class ContentPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ContentPresenter);
public:

public:
	static ContentPresenter* Create(GUIManager* manager);

	ContentPresenter(GUIManager* manager);
	virtual ~ContentPresenter();

	void SetContent(UIElement* content);

protected:
#if 1
	//virtual void ApplyTemplateHierarchy(CombinedLocalResource* parent);
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	virtual void Render();
#endif

	// TODO: InvalidateArrange/Measure

private:
	RefPtr<UIElement>	m_content;
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ContentPresenter �͘_���I�Ȏq�v�f�̔z�u����}�[�N���郁�^�f�[�^�̂悤�Ȃ��̂Ȃ̂ŁA�q�v�f�͎����Ȃ��B
};

} // namespace GUI
} // namespace Lumino