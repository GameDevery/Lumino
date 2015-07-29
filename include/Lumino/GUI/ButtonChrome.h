
#pragma once
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
	static const Property*	IsMouseOverProperty;	///< IsMouseOver �v���p�e�B�̎��ʎq
	static const Property*	FrameWidthProperty;		///< FrameWidth �v���p�e�B�̎��ʎq

public:
	static ButtonChrome* Create(GUIManager* manager);

	ButtonChrome(GUIManager* manager);
	virtual ~ButtonChrome();

public:
	// Property
	void SetMouseOver(bool value) { m_isMouseOver = value; }
	bool IsMouseOver() const { return m_isMouseOver; }

	/// �{�^���C���[�W�̊O�g�̕���ݒ肵�܂��B
	void SetFrameWidth(float width) { m_frameWidth = width; }

	/// �{�^���C���[�W�̊O�g�̕����擾���܂��B
	float GetFrameWidth() const { return m_frameWidth; }

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
