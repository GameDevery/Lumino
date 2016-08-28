
#pragma once
#include "UIControl.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
	@details	VerticalContentAlignment �� HorizontalContentAlignment �̏����l�͂��ꂼ�� Stretch �ł��B
*/
class UIContentControl
	: public UIControl
{
	LN_UI_TYPEINFO_DECLARE();
public:

	void SetContent(UIElement* element);

protected:
	UIContentControl();
	virtual ~UIContentControl();
	void Initialize(detail::UIManager* manager);
	//virtual void OnUpdatingLayout() override;


private:
	UIElement*	m_contentElement;
	bool		m_contentElementModified;
};

LN_NAMESPACE_END
