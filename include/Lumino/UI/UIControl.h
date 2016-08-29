
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@note		�R���g���[���� VisualTree �̃��[�g�������Ƃ��ł���B
*/
class UIControl
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:
	LN_TR_PROPERTY(VerticalAlignment,	VerticalContentAlignmentProperty);		/**< VerticalContentAlignment �v���p�e�B�̎��ʎq */
	LN_TR_PROPERTY(HorizontalAlignment, HorizontalContentAlignmentProperty);	/**< HorizontalContentAlignment �v���p�e�B�̎��ʎq */
	
public:

	void SetVerticalContentAlignment(VerticalAlignment value) { tr::Property::SetPropertyValueDirect<VerticalAlignment>(this, VerticalContentAlignmentProperty, value); }
	VerticalAlignment GetVerticalContentAlignment() const { return tr::Property::GetPropertyValueDirect<VerticalAlignment>(this, VerticalContentAlignmentProperty); }
	
	void SetHorizontalContentAlignment(HorizontalAlignment value) { tr::Property::SetPropertyValueDirect<HorizontalAlignment>(this, HorizontalContentAlignmentProperty, value); }
	HorizontalAlignment GetHorizontalContentAlignment() const { return tr::Property::GetPropertyValueDirect<HorizontalAlignment>(this, HorizontalContentAlignmentProperty); }

protected:
	UIControl();
	virtual ~UIControl();
	void Initialize(detail::UIManager* manager);

	// UIElement interface
	virtual int GetVisualChildrenCount() const override;
	virtual UIElement* GetVisualChildOrderd(int index) const override;
	virtual SizeF MeasureOverride(const SizeF& constraint) override;
	virtual SizeF ArrangeOverride(const SizeF& finalSize) override;
	virtual VerticalAlignment* GetPriorityContentVerticalAlignment() override;
	virtual HorizontalAlignment* GetPriorityContentHorizontalAlignment() override;

LN_INTERNAL_ACCESS:
	void SetVisualTreeRoot(UIElement* element);
	UIElement* GetVisualTreeRoot() { return m_visualTreeRoot; }

private:
	VerticalAlignment	m_verticalContentAlignment;
	HorizontalAlignment	m_horizontalContentAlignment;

	UIElement*	m_visualTreeRoot;
};

LN_NAMESPACE_END
