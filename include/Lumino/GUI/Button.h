
#pragma once
#include "ContentControl.h"
#include "ButtonChrome.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
*/
class Button
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Button);
public:
	static const String	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq


	static const String ControlTemplateTypeName;	///< "Button"


	static ButtonPtr Create();

	Button(GUIManager* manager);
	virtual ~Button();

public:
	bool IsMouseOver() const { return m_isMouseOver; }

	//virtual void Render();

protected:
	virtual void OnClick();
	virtual bool OnEvent(EventType type, RoutedEventArgs* args);	// TODO: �Ӗ��I�ɂ� RoutedEvent �ł͂Ȃ�
	virtual void OnRender(Graphics::Painter* painter);
	//virtual void Render();


	virtual void OnMouseMove(MouseEventArgs* e);
	//void UIElement_MouseMove(CoreObject* sender, MouseEventArgs* e);

protected:
	// override UIElement

	//virtual bool ApplyTemplateInternal()
	//{
	//	if (ApplyTemplateInternalMain(ControlTemplateTypeName)) { return true; }
	//	return ContentControl::ApplyTemplateInternal();	// base
	//}

private:
	RefPtr<ButtonChrome>	m_chrome;	// TODO: ���B�����ƃe�[�}����ǂ݂���
	bool m_isMouseOver;
};

} // namespace GUI
} // namespace Lumino
