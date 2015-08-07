
#include "../../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/Grid.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ColumnDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ColumnDefinition, ContentElement);
LN_GUI_CONTENT_ELEMENT_IMPLEMENT(ColumnDefinition);

const float ColumnDefinition::Auto = NAN;
const float ColumnDefinition::Star = std::numeric_limits<float>::infinity();

// Register property
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, WidthProperty, "Width", m_width, ColumnDefinition::Star, NULL);
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, MinWidthProperty, "MinWidth", m_minWidth, 0.0f, NULL);
LN_PROPERTY_IMPLEMENT(ColumnDefinition, float, MaxWidthProperty, "MaxWidth", m_maxWidth, FLT_MAX, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinitionPtr ColumnDefinition::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::ColumnDefinition(GUIManager* manager)
	: ContentElement(manager)
	, m_width(std::numeric_limits<float>::infinity())
	, m_minWidth(0)
	, m_maxWidth(FLT_MAX)
	, m_actualWidth(0)
	, m_actualOffsetX(0)
	, m_elementGroup()
	, m_desiredWidth(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::~ColumnDefinition()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float ColumnDefinition::GetAvailableDesiredWidth() const
{
	if (IsAuto()) {
		return m_desiredWidth;
	}
	else if (IsPixel()) {
		return Math::Clamp(m_width, m_minWidth, m_maxWidth);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

//=============================================================================
// RowDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RowDefinition, ContentElement);
LN_GUI_CONTENT_ELEMENT_IMPLEMENT(RowDefinition);

const float RowDefinition::Auto = NAN;
const float RowDefinition::Star = std::numeric_limits<float>::infinity();

// Register property
LN_PROPERTY_IMPLEMENT(RowDefinition, float, HeightProperty, "Height", m_height, ColumnDefinition::Star, NULL);
LN_PROPERTY_IMPLEMENT(RowDefinition, float, MinHeightProperty, "MinHeight", m_minHeight, 0.0f, NULL);
LN_PROPERTY_IMPLEMENT(RowDefinition, float, MaxHeightProperty, "MaxHeight", m_maxHeight, FLT_MAX, NULL);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinitionPtr RowDefinition::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::RowDefinition(GUIManager* manager)
	: ContentElement(manager)
	, m_height(std::numeric_limits<float>::infinity())
	, m_minHeight(0)
	, m_maxHeight(FLT_MAX)
	, m_actualHeight(0)
	, m_actualOffsetY(0)
	, m_elementGroup()
	, m_desiredHeight(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::~RowDefinition()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float RowDefinition::GetAvailableDesiredHeight() const
{
	if (IsAuto()) {
		return m_desiredHeight;
	}
	else if (IsPixel()) {
		return Math::Clamp(m_height, m_minHeight, m_maxHeight);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

//=============================================================================
// Grid
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Grid, Panel);
LN_UI_ELEMENT_SUBCLASS_IMPL(Grid);

// Register property
LN_PROPERTY_IMPLEMENT(Grid, ColumnDefinitionList*, ColumnDefinitionsProperty, "ColumnDefinitions", m_columnDefinitionList, NULL, NULL);
LN_PROPERTY_IMPLEMENT(Grid, RowDefinitionList*, RowDefinitionsProperty, "RowDefinitions", m_rowDefinitionList, NULL, NULL);

// Register attached property
LN_DEFINE_ATTACHED_PROPERTY(Grid, ColumnProperty, "Column", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, ColumnSpanProperty, "ColumnSpan", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, RowProperty, "Row", 0);
LN_DEFINE_ATTACHED_PROPERTY(Grid, RowSpanProperty, "RowSpan", 0);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GridPtr Grid::Create()
{
	return internalCreateInstance(ApplicationContext::GetGUIManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::Grid(GUIManager* manager)
	: Panel(manager)
{

	m_columnDefinitionList.Attach(LN_NEW ColumnDefinitionList());
	m_rowDefinitionList.Attach(LN_NEW RowDefinitionList());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::~Grid()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::MeasureLayout(const SizeF& availableSize)
{
	//for (ColumnDefinition* col : *m_columnDefinitionList) { col->m_elementGroup.Clear(); col->m_desiredWidth = 0; }
	//for (RowDefinition* row : *m_rowDefinitionList)	 { row->m_elementGroup.Clear(); row->m_desiredHeight = 0; }

	// �܂� Measure �͑S���܂킵�Ă���
	for (auto child : m_visualChildren)
	{
		child->MeasureLayout(availableSize);
	}

	for (auto child : *m_children)
	{
		// �q�v�f���s�Ɨ񂲂ƂɃO���[�v������
		//		- �͈͊O�̃C���f�b�N�X���w�肳��Ă����� 0 �ɂ���B
		int colIdx = Grid::GetColumn(child);	// TODO: ���̃v���p�e�B�������{�g���l�b�N�ɂȂ邩��
		int rowIdx = Grid::GetRow(child);
		colIdx = m_columnDefinitionList->IsOutOfRange(colIdx) ? 0 : colIdx;
		rowIdx = m_rowDefinitionList->IsOutOfRange(rowIdx) ? 0 : rowIdx;

		ColumnDefinition* col = m_columnDefinitionList->IsEmpty() ? NULL : m_columnDefinitionList->GetAt(colIdx);
		RowDefinition*    row = m_rowDefinitionList->IsEmpty() ? NULL : m_rowDefinitionList->GetAt(rowIdx);

		//col->m_elementGroup.Add(child);
		//row->m_elementGroup.Add(child);

		// �q�v�f�� DesiredSize (�Œ�T�C�Y) �𑪂�̂́A�Z���̃T�C�Y�w�肪 "Auto" �̎������ł悢�B
		if ((col == NULL || col->IsAuto()) || (row == NULL || row->IsAuto()))
		{
			//child->MeasureLayout(availableSize);	// TODO: Measuer �͏�ɂ�����ق����ǂ��̂��m�F���Ă������ق����ǂ�����

			if (col != NULL && col->IsAuto() && Grid::GetColumnSpan(child) <= 1) {	// span �� 2 �ȏ�̗v�f�̓T�C�Y�W�v�ɍl�����Ȃ�
				col->m_desiredWidth = std::max(col->m_desiredWidth, child->GetDesiredSize().Width);
			}
			if (row != NULL && row->IsAuto() && Grid::GetRowSpan(child) <= 1) {		// span �� 2 �ȏ�̗v�f�̓T�C�Y�W�v�ɍl�����Ȃ�
				row->m_desiredHeight = std::max(row->m_desiredHeight, child->GetDesiredSize().Height);
			}
		}
	}

	// �e�Z���� DesiredSize ���W�v���āAGrid �S�̂� DesiredSize �����߂�B
	SizeF desiredSize = SizeF::Zero;
	for (auto col : *m_columnDefinitionList) {
		desiredSize.Width += col->GetAvailableDesiredWidth();
	}
	for (auto row : *m_rowDefinitionList) {
		desiredSize.Height += row->GetAvailableDesiredHeight();
	}

	// this �� m_desiredSize ��ݒ肷��
	Panel::MeasureLayout(desiredSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::ArrangeLayout(const RectF& finalLocalRect)
{
	// "Auto" �� "Pixel" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳����
	SizeF totalActual = SizeF::Zero;
	int starColCount = 0;
	int starRowCount = 0;
	for (auto col : *m_columnDefinitionList)
	{
		if (col->IsAuto() || col->IsPixel())
		{
			col->m_actualWidth = col->GetAvailableDesiredWidth();
			totalActual.Width += col->m_actualWidth;
		}
		else {
			starColCount++;
		}
	}
	for (auto row : *m_rowDefinitionList)
	{
		if (row->IsAuto() || row->IsPixel())
		{
			row->m_actualHeight = row->GetAvailableDesiredHeight();
			totalActual.Height += row->m_actualHeight;
		}
		else {
			starRowCount++;
		}
	}

	// "*" �ЂƂ��̃Z���̗̈�
	SizeF starUnit(
		(finalLocalRect.Width - totalActual.Width) / starColCount,
		(finalLocalRect.Height - totalActual.Height) / starRowCount);
	starUnit.Width = std::max(0.0f, starUnit.Width);	// ���l�̓_��
	starUnit.Height = std::max(0.0f, starUnit.Height);	// ���l�̓_��

	// "*" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳���A
	// �S�Z���̃I�t�Z�b�g (�ʒu) ���m�肳����
	PointF totalOffset = PointF::Zero;
	for (auto col : *m_columnDefinitionList)
	{
		if (col->IsStar()) {
			col->m_actualWidth = starUnit.Width;
		}

		// �Z��X���W�m��
		col->m_actualOffsetX = totalOffset.X;
		totalOffset.X += col->m_actualWidth;
	}
	for (auto row : *m_rowDefinitionList)
	{
		if (row->IsStar()) {
			row->m_actualHeight = starUnit.Height;
		}

		// �Z��Y���W�m��
		row->m_actualOffsetY = totalOffset.Y;
		totalOffset.Y += row->m_actualHeight;
	}

	// �q�v�f�̍ŏI�ʒu�E�T�C�Y���m�肳����
	for (auto child : *m_children)
	{
		int colIdx = Grid::GetColumn(child);
		int rowIdx = Grid::GetRow(child);
		int colSpan = Grid::GetColumnSpan(child);
		int rowSpan = Grid::GetRowSpan(child);
		colSpan = std::max(1, colSpan);	// 0 �� 1 �Ƃ���
		rowSpan = std::max(1, rowSpan);	// 0 �� 1 �Ƃ���
		colSpan = std::min(colSpan, colIdx + m_columnDefinitionList->GetCount());	// �ő�l����
		rowSpan = std::min(rowSpan, rowIdx + m_rowDefinitionList->GetCount());		// �ő�l����

		// �z�u��̃Z���ō��W���m��
		//RectF rect(
		//	m_columnDefinitionList->GetAt(colIdx)->m_actualOffsetX,
		//	m_rowDefinitionList->GetAt(rowIdx)->m_actualOffsetY,
		//	0, 0);
		RectF rect = RectF::Zero;

		// Span ���l�����ăT�C�Y���m��
		if (m_columnDefinitionList->IsEmpty())
		{
			rect.Width = finalLocalRect.Width;
		}
		else
		{
			rect.X = m_columnDefinitionList->GetAt(colIdx)->m_actualOffsetX;
			for (int iCol = 0; iCol < colSpan; ++iCol) {
				rect.Width += m_columnDefinitionList->GetAt(colIdx + iCol)->m_actualWidth;
			}
		}
		if (m_rowDefinitionList->IsEmpty())
		{
			rect.Height = finalLocalRect.Height;
		}
		else
		{
			rect.Y = m_rowDefinitionList->GetAt(rowIdx)->m_actualOffsetY;
			for (int iRow = 0; iRow < rowSpan; ++iRow) {
				rect.Height += m_rowDefinitionList->GetAt(rowIdx + iRow)->m_actualHeight;
			}
		}

		// Arrange
		child->ArrangeLayout(rect);
	}

	Panel::ArrangeLayout(finalLocalRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void	Grid::SetColumn(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, ColumnProperty, value); }
int		Grid::GetColumn(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, ColumnProperty, int); }
void	Grid::SetColumnSpan(UIElement* element, int value)	{ LN_SET_ATTACHED_PROPERTY(element, ColumnSpanProperty, value); }
int		Grid::GetColumnSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, ColumnSpanProperty, int); }
void	Grid::SetRow(UIElement* element, int value)			{ LN_SET_ATTACHED_PROPERTY(element, RowProperty, value); }
int		Grid::GetRow(UIElement* element)					{ LN_GET_ATTACHED_PROPERTY(element, RowProperty, int); }
void	Grid::SetRowSpan(UIElement* element, int value)		{ LN_SET_ATTACHED_PROPERTY(element, RowSpanProperty, value); }
int		Grid::GetRowSpan(UIElement* element)				{ LN_GET_ATTACHED_PROPERTY(element, RowSpanProperty, int); }

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Grid::AddColumnDefinition(float width, float minWidth, float maxWidth)
{
	RefPtr<GUI::ColumnDefinition> col(LN_NEW GUI::ColumnDefinition(m_manager));
	m_columnDefinitionList->Add(col);
}
void Grid::AddRowDefinition(float height, float minHeight, float maxHeight)
{
	RefPtr<GUI::RowDefinition> row(LN_NEW GUI::RowDefinition(m_manager));
	m_rowDefinitionList->Add(row);
}

} // namespace GUI
} // namespace Lumino

