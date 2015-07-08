
#pragma once
#include "../ContentElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief		
*/
class Grid
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Grid);
public:
	static AttachedProperty*	ColumnProperty;
	static AttachedProperty*	ColumnSpanProperty;
	static AttachedProperty*	RowProperty;
	static AttachedProperty*	RowSpanProperty;

public:
	Grid(GUIManager* manager);
	virtual ~Grid();

public:
	//-------------------------------------------------------------------------
	/** @name Attached properties *//** @{ */

	/// ColumnProperty �̒l���w�肵�� UIElement �ɐݒ肵�܂��B
	static void SetColumn(UIElement* element, int value);

	/// ColumnProperty �̒l���w�肵�� UIElement ����擾���܂��B
	static int GetColumn(UIElement* element);

	/// ColumnSpanProperty �̒l���w�肵�� UIElement �ɐݒ肵�܂��B
	static void SetColumnSpan(UIElement* element, int value);

	/// ColumnSpanProperty �̒l���w�肵�� UIElement ����擾���܂��B
	static int GetColumnSpan(UIElement* element);

	/// RowProperty �̒l���w�肵�� UIElement �ɐݒ肵�܂��B
	static void SetRow(UIElement* element, int value);

	/// RowProperty �̒l���w�肵�� UIElement ����擾���܂��B
	static int GetRow(UIElement* element);

	/// RowSpanProperty �̒l���w�肵�� UIElement �ɐݒ肵�܂��B
	static void SetRowSpan(UIElement* element, int value);

	/// RowSpanProperty �̒l���w�肵�� UIElement ����擾���܂��B
	static int GetRowSpan(UIElement* element);

	/** @} */

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalRect);
};

/**
	@brief		
*/
class ColumnDefinition
	: public ContentElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ColumnDefinition);	// TODO �}�N�����ς��������ǂ��C������
public:
	static const PropertyID	WidthProperty;
	static const PropertyID	MinWidthProperty;
	static const PropertyID	MaxWidthProperty;

public:
	ColumnDefinition(GUIManager* manager);
	virtual ~ColumnDefinition();

	void SetWidth(float width) { m_width = width; }
	float GetWidth() const { return m_width; }

	void SetMinWidth(float width) { m_minWidth = width; }
	float GetMinWidth() const { return m_minWidth; }

	void SetMaxWidth(float width) { m_maxWidth = width; }
	float GetMaxWidth() const { return m_maxWidth; }

private:
	float	m_width;
	float	m_minWidth;
	float	m_maxWidth;
};

/**
	@brief		
*/
class RowDefinition
	: public ContentElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RowDefinition);	// TODO �}�N�����ς��������ǂ��C������
public:
	static const PropertyID	HeightProperty;
	static const PropertyID	MaxHeightProperty;
	static const PropertyID	MinHeightProperty;

public:
	RowDefinition(GUIManager* manager);
	virtual ~RowDefinition();

	void SetHeight(float height) { m_height = height; }
	float GetHeight() const { return m_height; }

	void SetMinHeight(float height) { m_minHeight = height; }
	float GetMinHeight() const { return m_minHeight; }

	void SetMaxHeight(float height) { m_maxHeight = height; }
	float GetMaxHeight() const { return m_maxHeight; }

private:
	float	m_height;
	float	m_minHeight;
	float	m_maxHeight;
};

} // namespace GUI
} // namespace Lumino
