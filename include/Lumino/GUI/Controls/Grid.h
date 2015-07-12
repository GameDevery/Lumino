
#pragma once
#include "../GUIObjectCollection.h"
#include "../ContentElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{
class Grid;

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

	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	void SetWidth(float width) { m_width = width; }
	float GetWidth() const { return m_width; }

	void SetMinWidth(float width) { m_minWidth = width; }
	float GetMinWidth() const { return m_minWidth; }

	void SetMaxWidth(float width) { m_maxWidth = width; }
	float GetMaxWidth() const { return m_maxWidth; }

	/** @} */

public:
	bool IsAuto() const { return Math::IsNaN(m_width); }	// �v�f�ɂ��킹�邩
	bool IsStar() const { return Math::IsInf(m_width); }	// ���C�A�E�g��A�c��̗̈���g����
	bool IsPixel() const { return !IsAuto() && !IsStar(); }

private:
	float	m_width;
	float	m_minWidth;
	float	m_maxWidth;

	friend class Grid;
	float				m_actualWidth;
	float				m_actualOffsetX;
	Array<UIElement*>	m_elementGroup;		///< Measure/Arrange �̍�Ɨp�ϐ�
	float				m_desiredWidth;		///< Measure/Arrange �̍�Ɨp�ϐ�
	float GetAvailableDesiredWidth() const;	///< �ŏ�/�ő���l����������Ԃ�
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

	bool IsAuto() const { return Math::IsNaN(m_height); }
	bool IsStar() const { return Math::IsInf(m_height); }
	bool IsPixel() const { return !IsAuto() && !IsStar(); }

public:
	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	void SetHeight(float height) { m_height = height; }
	float GetHeight() const { return m_height; }

	void SetMinHeight(float height) { m_minHeight = height; }
	float GetMinHeight() const { return m_minHeight; }

	void SetMaxHeight(float height) { m_maxHeight = height; }
	float GetMaxHeight() const { return m_maxHeight; }

	/** @} */

private:
	float	m_height;
	float	m_minHeight;
	float	m_maxHeight;

	friend class Grid;
	float				m_actualHeight;
	float				m_actualOffsetY;
	Array<UIElement*>	m_elementGroup;			///< Measure/Arrange �̍�Ɨp�ϐ�
	float				m_desiredHeight;		///< Measure/Arrange �̍�Ɨp�ϐ�
	float GetAvailableDesiredHeight() const;	///< �ŏ�/�ő���l����������Ԃ�
};

typedef GUIObjectList<ColumnDefinition>	ColumnDefinitionList;
typedef GUIObjectList<RowDefinition>	RowDefinitionList;

/**
	@brief		
*/
class Grid
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Grid);
public:
	static const PropertyID		ColumnDefinitionsProperty;
	static const PropertyID		RowDefinitionsProperty;

	static const AttachedProperty*	ColumnProperty;
	static AttachedProperty*	ColumnSpanProperty;
	static AttachedProperty*	RowProperty;
	static AttachedProperty*	RowSpanProperty;

public:
	Grid(GUIManager* manager);
	virtual ~Grid();

public:
	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	/// ���� Grid �Œ�`����Ă��� ColumnDefinition �̃��X�g���擾���܂��B
	ColumnDefinitionList* GetColumnDefinitions() const { return m_columnDefinitionList; }

	/// ���� Grid �Œ�`����Ă��� RowDefinition �̃��X�g���擾���܂��B
	RowDefinitionList* GetRowDefinitions() const { return m_rowDefinitionList; }

	/** @} */

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
	virtual void ArrangeLayout(const RectF& finalLocalRect);

private:
	RefPtr<ColumnDefinitionList>	m_columnDefinitionList;
	RefPtr<RowDefinitionList>		m_rowDefinitionList;
};

} // namespace GUI
} // namespace Lumino
