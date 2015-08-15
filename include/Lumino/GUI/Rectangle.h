
#pragma once
#include "UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		Shape �v�f�̊�{�N���X�ł��B
*/
class Shape
	: public UIElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();	// TODO: LN_UI_ELEMENT_SUBCLASS_DECL �ɂ܂Ƃ߂���
	LN_UI_ELEMENT_SUBCLASS_DECL(Shape);
public:
	LN_PROPERTY(Graphics::Brush*, FillBrushProperty);	/**< FillBrush �v���p�e�B�̎��ʎq */

public:

	//-------------------------------------------------------------------------
	/** @name Properties */
	/** @{ */

	/** �}�`�����̓h��Ԃ��Ɏg�p����u���V��ݒ肵�܂��B*/
	void SetFillBrush(Graphics::Brush* size) { SetTypedPropertyValue<Graphics::Brush*>(SizeProperty, size); }

	/** �}�`�����̓h��Ԃ��Ɏg�p����u���V���擾���܂��B*/
	Graphics::Brush* GetFillBrush() const { return GetTypedPropertyValue<Graphics::Brush*>(SizeProperty); }

	/** @} */

protected:
	Shape(GUIManager* manager);
	virtual ~Shape();

protected:
	RefPtr<Graphics::Brush>	m_fillBrush;
};

/**
	@brief		�l�p�`��`�悷�� GUI �v�f�ł��B
*/
class Rectangle
	: public Shape
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Rectangle);

public:
	/**
		@brief	Rectangle ���쐬���܂��B
		@return	�쐬���ꂽ�C���X�^���X�̃|�C���^�B�g���I������ Release() ���Ăяo���āA�Q�Ƃ�������Ă��������B	
	*/
	Rectangle* Create();

protected:
	Rectangle(GUIManager* manager);
	virtual ~Rectangle();
	virtual void OnRender(Graphics::Painter* painter);

public:
};

} // namespace GUI
} // namespace Lumino
