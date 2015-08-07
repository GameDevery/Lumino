
#pragma once
#include "Common.h"
#include "Decorator.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		Button �R���g���[���̊O�ςł��B
*/
class ButtonChrome
	: public Decorator
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonChrome);
public:
	LN_PROPERTY(bool,	IsMouseOverProperty);
	LN_PROPERTY(float,	FrameWidthProperty);

public:
	static ButtonChromePtr Create();

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

public:
	// Property
	void SetMouseOver(bool value) { SetTypedPropertyValue<bool>(IsMouseOverProperty, value); }
	bool IsMouseOver() const { return GetTypedPropertyValue<bool>(IsMouseOverProperty); }

	/// �{�^���C���[�W�̊O�g�̕���ݒ肵�܂��B
	void SetFrameWidth(float value) { SetTypedPropertyValue<float>(FrameWidthProperty, value); }

	/// �{�^���C���[�W�̊O�g�̕����擾���܂��B
	float GetFrameWidth() const { return GetTypedPropertyValue<float>(FrameWidthProperty); }

public:
	// override CoreObject
	//virtual void SetPropertyValue(const Property* prop, const Variant& value);
	//virtual Variant GetPropertyValue(const Property* prop) const;

protected:
	virtual void OnApplyTemplate(CombinedLocalResource* localResource);
	virtual void OnRender(Graphics::Painter* painter);

private:
	RefPtr<Graphics::TextureBrush>	m_brush;
	RefPtr<Graphics::TextureBrush>	m_bgBrush;
	float							m_frameWidth;	///< �g�̕� (�`��Ŏg�p���邽�߃p�t�H�[�}���X���l�����AGetValue() �ł͂Ȃ������o�Ɏ����Ă���)
	bool							m_isMouseOver;
};

} // namespace GUI
} // namespace Lumino
