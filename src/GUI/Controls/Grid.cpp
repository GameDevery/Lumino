
#include "../../Internal.h"
#include <Lumino/Property.h>
#include <Lumino/GUI/Controls/Grid.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Grid
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Grid);
LN_UI_ELEMENT_SUBCLASS_IMPL(Grid);

const PropertyID	Grid::ColumnDefinitionsProperty(_T("ColumnDefinitions"));
const PropertyID	Grid::RowDefinitionsProperty(_T("RowDefinitions"));

const AttachedProperty*	Grid::ColumnProperty = NULL;
AttachedProperty*	Grid::ColumnSpanProperty = NULL;
AttachedProperty*	Grid::RowProperty = NULL;
AttachedProperty*	Grid::RowSpanProperty = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Grid::Grid(GUIManager* manager)
	: Panel(manager)
{
	// Register attached property
	LN_DEFINE_ATTACHED_PROPERTY(ColumnProperty,		"Column",		int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(ColumnSpanProperty,	"ColumnSpan",	int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(RowProperty,		"Row",			int, Grid, 0);
	LN_DEFINE_ATTACHED_PROPERTY(RowSpanProperty,	"RowSpan",		int, Grid, 0);

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
	for (auto col : *m_columnDefinitionList) { col->m_elementGroup.Clear(); col->m_desiredWidth = 0; }
	for (auto row : *m_rowDefinitionList)	 { row->m_elementGroup.Clear(); row->m_desiredHeight = 0; }

	for (auto child : *m_children)
	{
		// �q�v�f���s�Ɨ񂲂ƂɃO���[�v������
		//		- �͈͊O�̃C���f�b�N�X���w�肳��Ă����� 0 �ɂ���B
		int colIdx = Grid::GetColumn(child);	// TODO: ���̃v���p�e�B�������{�g���l�b�N�ɂȂ邩��
		int rowIdx = Grid::GetRow(child);
		colIdx = m_columnDefinitionList->CheckValidIndex(colIdx) ? colIdx : 0;
		rowIdx = m_rowDefinitionList->CheckValidIndex(rowIdx)    ? rowIdx : 0;

		auto col = m_columnDefinitionList->GetAt(colIdx);
		auto row = m_rowDefinitionList->GetAt(colIdx);

		col->m_elementGroup.Add(child);
		row->m_elementGroup.Add(child);

		// �q�v�f�� DesiredSize (�Œ�T�C�Y) �𑪂�̂́A�Z���̃T�C�Y�w�肪 "Auto" �̎������ł悢�B
		if (col->IsAuto() || row->IsAuto())
		{
			child->MeasureLayout(availableSize);	// TODO: Measuer �͏�ɂ�����ق����ǂ��̂��m�F���Ă������ق����ǂ�����

			if (col->IsAuto() && Grid::GetColumnSpan(child) <= 1) {	// span �� 2 �ȏ�̗v�f�̓T�C�Y�W�v�ɍl�����Ȃ�
				col->m_desiredWidth = std::max(col->m_desiredWidth, child->GetDesiredSize().Width);
			}
			if (row->IsAuto() && Grid::GetRowSpan(child) <= 1) {	// span �� 2 �ȏ�̗v�f�̓T�C�Y�W�v�ɍl�����Ȃ�
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
void Grid::ArrangeLayout(const RectF& finalRect)
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
		(finalRect.Width - totalActual.Width) / starColCount,
		(finalRect.Height - totalActual.Height) / starRowCount);
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
		totalOffset.X += col->m_actualOffsetX;
	}
	for (auto row : *m_rowDefinitionList)
	{
		if (row->IsStar()) {
			row->m_actualHeight = starUnit.Height;
		}

		// �Z��Y���W�m��
		row->m_actualOffsetY = totalOffset.Y;
		totalOffset.Y += row->m_actualOffsetY;
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
		RectF rect(
			m_columnDefinitionList->GetAt(colIdx)->m_actualOffsetX,
			m_rowDefinitionList->GetAt(rowIdx)->m_actualOffsetY,
			0, 0);

		// Span ���l�����ăT�C�Y���m��
		for (int iCol = 0; iCol < colSpan; ++iCol) {
			rect.Width += m_columnDefinitionList->GetAt(colIdx + iCol)->m_actualWidth;
		}
		for (int iRow = 0; iRow < rowSpan; ++iRow) {
			rect.Height += m_rowDefinitionList->GetAt(rowIdx + iRow)->m_actualHeight;
		}

		// Arrange
		child->ArrangeLayout(rect);
	}

	Panel::ArrangeLayout(finalRect);
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

//=============================================================================
// ColumnDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ColumnDefinition);
LN_UI_ELEMENT_SUBCLASS_IMPL(ColumnDefinition);

const PropertyID	ColumnDefinition::WidthProperty(_T("Width"));
const PropertyID	ColumnDefinition::MinWidthProperty(_T("MinWidth"));
const PropertyID	ColumnDefinition::MaxWidthProperty(_T("MaxWidth"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ColumnDefinition::ColumnDefinition(GUIManager* manager)
	: ContentElement(manager)
{
	// Register property
	LN_DEFINE_PROPERTY(ColumnDefinition, float, WidthProperty, &ColumnDefinition::SetWidth, &ColumnDefinition::GetWidth, NAN);
	LN_DEFINE_PROPERTY(ColumnDefinition, float, MinWidthProperty, &ColumnDefinition::SetMinWidth, &ColumnDefinition::GetMinWidth, NAN);
	LN_DEFINE_PROPERTY(ColumnDefinition, float, MaxWidthProperty, &ColumnDefinition::SetMaxWidth, &ColumnDefinition::GetMaxWidth, NAN);
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
	if (IsPixel()) {
		return Math::Clamp(m_width, m_minWidth, m_maxWidth);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

//=============================================================================
// RowDefinition
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RowDefinition);
LN_UI_ELEMENT_SUBCLASS_IMPL(RowDefinition);

const PropertyID	RowDefinition::HeightProperty(_T("Height"));
const PropertyID	RowDefinition::MinHeightProperty(_T("MinHeight"));
const PropertyID	RowDefinition::MaxHeightProperty(_T("MaxHeight"));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RowDefinition::RowDefinition(GUIManager* manager)
	: ContentElement(manager)
{	
	// Register property
	LN_DEFINE_PROPERTY(RowDefinition, float, HeightProperty, &RowDefinition::SetHeight, &RowDefinition::GetHeight, NAN);
	LN_DEFINE_PROPERTY(RowDefinition, float, MinHeightProperty, &RowDefinition::SetMinHeight, &RowDefinition::GetMinHeight, NAN);
	LN_DEFINE_PROPERTY(RowDefinition, float, MaxHeightProperty, &RowDefinition::SetMaxHeight, &RowDefinition::GetMaxHeight, NAN);
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
	if (IsPixel()) {
		return Math::Clamp(m_height, m_minHeight, m_maxHeight);
	}
	else { //if (IsStar()) {
		return 0;
	}
}

} // namespace GUI
} // namespace Lumino

