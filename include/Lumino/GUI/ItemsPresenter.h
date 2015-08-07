
#pragma once
#include "UIElement.h"
#include "Controls/Panel.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief	ItemsControl �̃R���e���c��\�����܂��B
*/
class ItemsPresenter
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ItemsPresenter);
public:

public:
	static ItemsPresenterPtr Create();

	ItemsPresenter(GUIManager* manager);
	virtual ~ItemsPresenter();

public:	// internal
	void AttachHostPanel(Panel* panel);

protected:
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);
	//virtual void AddVisualChild(UIElement* child) { LN_THROW(0, InvalidOperationException); }	// ItemsPresenter �͘_���I�Ȏq�v�f�̔z�u����}�[�N���郁�^�f�[�^�̂悤�Ȃ��̂Ȃ̂ŁA�q�v�f�͎����Ȃ��B
	
private:
	RefPtr<Panel>	m_hostPanel;
};

} // namespace GUI
} // namespace Lumino
