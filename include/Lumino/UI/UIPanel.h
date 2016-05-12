
#pragma once
#include "UIElement.h"
#include "UIElementCollection.h"

LN_NAMESPACE_BEGIN

/**
	@brief		1�ȏ�̎q UIElement ������ UI �v�f�̃x�[�X�N���X�ł��B
*/
class UIPanel
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	void AddChild(UIElement* element);
	void RemoveChild(UIElement* element);

LN_PROTECTED_INTERNAL_ACCESS:
	UIPanel();
	virtual ~UIPanel();
	void Initialize(detail::UIManager* manager);

	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;

LN_INTERNAL_ACCESS:
	void OnChildElementAdd(UIElement* element);
	void OnChildElementRemove(UIElement* element);

private:
	RefPtr<UIElementCollection>	m_children;
};

LN_NAMESPACE_END