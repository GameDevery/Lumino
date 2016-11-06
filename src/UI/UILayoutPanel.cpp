
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UILayoutPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UILayoutPanel, UIElement)

//------------------------------------------------------------------------------
UILayoutPanel::UILayoutPanel()
{
}

//------------------------------------------------------------------------------
UILayoutPanel::~UILayoutPanel()
{
}

//------------------------------------------------------------------------------
void UILayoutPanel::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
	m_children = RefPtr<UIElementCollection>::MakeRef(this);

	// Panel �n�̃f�t�H���g�� Stretch
	SetHAlignment(HAlignment::Stretch);
	SetVAlignment(VAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UILayoutPanel::AddChild(UIElement* element)
{
	m_children->Add(element);
}

//------------------------------------------------------------------------------
void UILayoutPanel::RemoveChild(UIElement* element)
{
	m_children->Remove(element);
}

//------------------------------------------------------------------------------
int UILayoutPanel::GetVisualChildrenCount() const
{
	return m_children->GetCount();
}

//------------------------------------------------------------------------------
UIElement* UILayoutPanel::GetVisualChildOrderd(int index) const
{
	return m_children->GetAt(index);
}

//------------------------------------------------------------------------------
Size UILayoutPanel::MeasureOverride(const Size& constraint)
{
	Size desiredSize = UIElement::MeasureOverride(constraint);
	for (UIElement* child : *m_children)
	{
		child->MeasureLayout(constraint);
		const Size& childDesiredSize = child->GetDesiredSize();

		desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	}
	return desiredSize;
}

//------------------------------------------------------------------------------
Size UILayoutPanel::ArrangeOverride(const Size& finalSize)
{
	for (UIElement* child : *m_children)
	{
		Size childDesiredSize = child->GetDesiredSize();
		childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
		childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
		child->ArrangeLayout(RectF(0, 0, childDesiredSize));
	}
	return finalSize;
}

//------------------------------------------------------------------------------
void UILayoutPanel::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
{
	// �V�����ǉ����ꂽ���̂���
	for (UIElement* element : e.newItems)
	{
		element->SetParent(this);
	}

	// �폜���ꂽ���̂���
	for (UIElement* element : e.oldItems)
	{
		element->SetParent(nullptr);
	}
}


//==============================================================================
// UIStackPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIStackPanel, UILayoutPanel)

//------------------------------------------------------------------------------
UIStackPanel::UIStackPanel()
{
}

//------------------------------------------------------------------------------
UIStackPanel::~UIStackPanel()
{
}

//------------------------------------------------------------------------------
void UIStackPanel::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
Size UIStackPanel::MeasureOverride(const Size& constraint)
{
	Size size = constraint;

	if (m_orientation == Orientation::Horizontal)
	{
		// ���ɕ��ׂ�ꍇ�A���̐�����݂��Ȃ�
		size.width = std::numeric_limits<float>::infinity();
	}
	else
	{
		// �c�ɕ��ׂ�ꍇ�A�����̐�����݂��Ȃ�
		size.height = std::numeric_limits<float>::infinity();
	}

	Size desiredSize;
	for (UIElement* child : *GetChildren())
	{
		child->MeasureLayout(size);

		const Size& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			desiredSize.width += childDesiredSize.width;
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		else
		{
			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height += child->GetDesiredSize().height;
		}
	}

	return desiredSize;
}

//------------------------------------------------------------------------------
Size UIStackPanel::ArrangeOverride(const Size& finalSize)
{
	float prevChildSize = 0;
	RectF childRect;
	for (UIElement* child : *GetChildren())
	{
		const Size& childDesiredSize = child->GetDesiredSize();
		if (m_orientation == Orientation::Horizontal)
		{
			childRect.x += prevChildSize;
			prevChildSize = childDesiredSize.width;
			childRect.width = prevChildSize;
			childRect.height = finalSize.height;//std::min(finalSize.Height, childDesiredSize.Height);
		}
		else
		{
			childRect.y += prevChildSize;
			prevChildSize = childDesiredSize.height;
			childRect.height = prevChildSize;
			childRect.width = finalSize.width;// std::min(finalSize.Width, childDesiredSize.Width);
		}

		child->ArrangeLayout(childRect);
	}

	return finalSize;
}


//==============================================================================
// UICanvas
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UICanvas, UILayoutPanel)

//------------------------------------------------------------------------------
UICanvasPtr UICanvas::Create()
{
	auto ptr = UICanvasPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UICanvas::UICanvas()
{
}

//------------------------------------------------------------------------------
UICanvas::~UICanvas()
{
}

//------------------------------------------------------------------------------
void UICanvas::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
Size UICanvas::MeasureOverride(const Size& constraint)
{
	return UILayoutPanel::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
Size UICanvas::ArrangeOverride(const Size& finalSize)
{
	//ThicknessF canvas;
	
	
	for (UIElement* child : *GetChildren())
	{
		//if (child->GetInvalidateFlags().TestFlag(detail::InvalidateFlags::ParentChangedUpdating))
		//{
		//}
		
		const Size& desiredSize = child->GetDesiredSize();
		Size size = child->GetSizeInternal();
		size.width = Math::IsNaN(size.width) ? desiredSize.width : size.width;
		size.height = Math::IsNaN(size.height) ? desiredSize.height : size.height;

		RectF childRect(child->GetPositionInternal(), child->GetSizeInternal());
		AlignmentAnchor anchor = child->GetAnchorInternal();
		
		if (anchor != AlignmentAnchor::None)
		{
			const ThicknessF& margin = GetMargineInternal();
			
			float l = NAN, t = NAN, r = NAN, b = NAN;
			if (anchor.TestFlag(AlignmentAnchor::LeftOffsets))
				l = margin.Left;
			else if (anchor.TestFlag(AlignmentAnchor::LeftRatios))
				l = finalSize.width * margin.Left;
			
			if (anchor.TestFlag(AlignmentAnchor::TopOffsets))
				t = margin.Top;
			else if (anchor.TestFlag(AlignmentAnchor::TopRatios))
				t = finalSize.height * margin.Top;
			
			if (anchor.TestFlag(AlignmentAnchor::RightOffsets))
				r = finalSize.width - margin.Right;
			else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				r = finalSize.width - (finalSize.width * margin.Right);
			
			if (anchor.TestFlag(AlignmentAnchor::BottomOffsets))
				b = finalSize.height - margin.Bottom;
			else if (anchor.TestFlag(AlignmentAnchor::RightRatios))
				b = finalSize.height - (finalSize.height * margin.Bottom);

			if (anchor.TestFlag(AlignmentAnchor::HCenter))
				childRect.x = (finalSize.width - childRect.width) / 2;

			if (anchor.TestFlag(AlignmentAnchor::VCenter))
				childRect.y = (finalSize.height - childRect.height) / 2;

			if (!Math::IsNaN(l) || !Math::IsNaN(r))
			{
				if (!Math::IsNaN(l) && Math::IsNaN(r))
				{
					childRect.x = l;
				}
				else if (Math::IsNaN(l) && !Math::IsNaN(r))
				{
					childRect.x = r - childRect.width;
				}
				else
				{
					childRect.x = l;
					childRect.width = r - l;
				}
			}

			if (!Math::IsNaN(t) || !Math::IsNaN(b))
			{
				if (!Math::IsNaN(t) && Math::IsNaN(b))
				{
					childRect.y = t;
				}
				else if (Math::IsNaN(t) && !Math::IsNaN(b))
				{
					childRect.y = b - childRect.height;
				}
				else
				{
					childRect.y = t;
					childRect.height = b - t;
				}
			}

			child->ArrangeLayout(childRect);
		}
		else
		{
			child->ArrangeLayout(childRect);
		}
	}

	return finalSize;
}


//==============================================================================
// DefinitionBase
//==============================================================================
class UIGridLayout::DefinitionBase
	: public Object
{
public:

	DefinitionBase()
		: m_type(GridLengthType::Ratio)
		, m_size(0.0f)
		, m_minSize(0.0f)
		, m_maxSize(FLT_MAX)
		, m_desiredSize(0.0f)
		, m_actualOffset(0.0f)
		, m_actualSize(0.0f)
	{
	}

	virtual ~DefinitionBase()
	{
	}

	GridLengthType GetType() const { return m_type; }

	float GetAvailableDesiredSize() const
	{
		if (m_type == GridLengthType::Auto) {
			return m_desiredSize;
		}
		else if (m_type == GridLengthType::Pixel) {
			return Math::Clamp(m_size, m_minSize, m_maxSize);
		}
		else {
			return 0;
		}
	}

	float GetRatioWeight() const
	{
		return (m_size == 0.0f) ? 1.0f : m_size;
	}

	void AdjustActualSize()
	{
		m_actualSize = Math::Clamp(m_actualSize, m_minSize, m_maxSize);
	}

protected:
	GridLengthType	m_type;
	float			m_size;
	float			m_minSize;
	float			m_maxSize;

public:
	float			m_desiredSize;
	float			m_actualOffset;	// �ŏI�I�t�Z�b�g
	float			m_actualSize;	// �ŏI�T�C�Y
};

//==============================================================================
// ColumnDefinition
//==============================================================================
class UIGridLayout::ColumnDefinition
	: public DefinitionBase
{
public:

	ColumnDefinition()
	{
	}

	virtual ~ColumnDefinition()
	{
	}

	void SetWidth(float value, GridLengthType type = GridLengthType::Pixel) { m_size = value; m_type = type; }
	float GetWidth() const { return m_size; }

	void SetMinWidth(float value) { m_minSize = value; }
	float GetMinWidth() const { return m_minSize; }

	void SetMaxWidth(float value) { m_maxSize = value; }
	float GetMaxWidth() const { return m_maxSize; }
};

//==============================================================================
// RowDefinition
//==============================================================================
class UIGridLayout::RowDefinition
	: public DefinitionBase
{
public:

	RowDefinition()
	{
	}

	virtual ~RowDefinition()
	{
	}

	void SetHeight(float value, GridLengthType type = GridLengthType::Pixel) { m_size = value; m_type = type; }
	float GetHeight() const { return m_size; }

	void SetMinHeight(float value) { m_minSize = value; }
	float GetMinHeight() const { return m_minSize; }

	void SetMaxHeight(float value) { m_maxSize = value; }
	float GetMaxHeight() const { return m_maxSize; }
};

//==============================================================================
// UIGridLayout
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIGridLayout, UILayoutPanel)

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create()
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayoutPtr UIGridLayout::Create(int columnCount, int rowCount)
{
	auto ptr = UIGridLayoutPtr::MakeRef();
	ptr->Initialize(detail::UIManager::GetInstance());
	ptr->SetGridSize(columnCount, rowCount);
	return ptr;
}

//------------------------------------------------------------------------------
UIGridLayout::UIGridLayout()
	: m_columnDefinitions()
	, m_rowDefinitions()
{
}

//------------------------------------------------------------------------------
UIGridLayout::~UIGridLayout()
{
}

//------------------------------------------------------------------------------
void UIGridLayout::Initialize(detail::UIManager* manager)
{
	UILayoutPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
void UIGridLayout::SetGridSize(int columnCount, int rowCount)
{
	m_columnDefinitions.Clear();
	m_rowDefinitions.Clear();

	for (int i = 0; i < columnCount; ++i)
		m_columnDefinitions.Add(RefPtr<ColumnDefinition>::MakeRef());
	for (int i = 0; i < rowCount; ++i)
		m_rowDefinitions.Add(RefPtr<RowDefinition>::MakeRef());
}

//------------------------------------------------------------------------------
void UIGridLayout::AddColumnDefinition(GridLengthType type, float width, float minWidth, float maxWidth)
{
	auto ptr = RefPtr<ColumnDefinition>::MakeRef();
	ptr->SetWidth(width, type);
	ptr->SetMinWidth(minWidth);
	ptr->SetMaxWidth(maxWidth);
	m_columnDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
void UIGridLayout::AddRowDefinition(GridLengthType type, float height, float minHeight, float maxHeight)
{
	auto ptr = RefPtr<RowDefinition>::MakeRef();
	ptr->SetHeight(height, type);
	ptr->SetMinHeight(minHeight);
	ptr->SetMaxHeight(maxHeight);
	m_rowDefinitions.Add(ptr);
}

//------------------------------------------------------------------------------
Size UIGridLayout::MeasureOverride(const Size& constraint)
{
	for (UIElement* child : *GetChildren())
	{
		// �܂��͎q�� Measure
		child->MeasureLayout(constraint);

		// child ���z�u�����ׂ� column �� row ��T��
		int colIdx = child->GetLayoutColumn();
		int rowIdx = child->GetLayoutRow();
		colIdx = m_columnDefinitions.IsOutOfRange(colIdx) ? 0 : colIdx;
		rowIdx = m_rowDefinitions.IsOutOfRange(rowIdx) ? 0 : rowIdx;
		ColumnDefinition* col = m_columnDefinitions.IsEmpty() ? nullptr : m_columnDefinitions.GetAt(colIdx);
		RowDefinition*    row = m_rowDefinitions.IsEmpty() ? nullptr : m_rowDefinitions.GetAt(rowIdx);

		// �q�v�f�� DesiredSize (�Œ�T�C�Y) �𑪂�̂́A�Z���̃T�C�Y�w�肪 "Auto" �̎������ł悢�B
		const Size& childDesiredSize = child->GetDesiredSize();
		if (col != nullptr && col->GetType() == GridLengthType::Auto)
		{
			col->m_desiredSize = std::max(col->m_desiredSize, childDesiredSize.width);
		}
		if (row != nullptr && row->GetType() == GridLengthType::Auto)
		{
			row->m_desiredSize = std::max(row->m_desiredSize, childDesiredSize.height);
		}
	}

	// �e�Z���� DesiredSize ���W�v���āAGrid �S�̂� DesiredSize �����߂�
	Size desiredSize = UILayoutPanel::MeasureOverride(constraint);
	for (auto col : m_columnDefinitions)
	{
		desiredSize.width += col->GetAvailableDesiredSize();
	}
	for (auto row : m_rowDefinitions)
	{
		desiredSize.height += row->GetAvailableDesiredSize();
	}

	return desiredSize;
}

//------------------------------------------------------------------------------
Size UIGridLayout::ArrangeOverride(const Size& finalSize)
{
	// "Auto" �� "Pixel" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳����B
	// �܂��A"*" �ł���s��̐����J�E���g����B
	Size totalActual = Size::Zero;
	float starColCount = 0.0f;
	float starRowCount = 0.0f;
	for (auto col : m_columnDefinitions)
	{
		if (col->GetType() == GridLengthType::Auto || col->GetType() == GridLengthType::Pixel)
		{
			col->m_actualSize = col->GetAvailableDesiredSize();
			totalActual.width += col->m_actualSize;
		}
		else
		{
			starColCount += col->GetRatioWeight();
		}
	}
	for (auto row : m_rowDefinitions)
	{
		if (row->GetType() == GridLengthType::Auto || row->GetType() == GridLengthType::Pixel)
		{
			row->m_actualSize = row->GetAvailableDesiredSize();
			totalActual.height += row->m_actualSize;
		}
		else
		{
			starRowCount += row->GetRatioWeight();
		}
	}

	// "1*" ���̃Z���̗̈�
	Size starUnit(
		(starColCount != 0.0f) ? (finalSize.width - totalActual.width) / starColCount : 0.0f,
		(starRowCount != 0.0f) ? (finalSize.height - totalActual.height) / starRowCount : 0.0f);
	starUnit.width = std::max(0.0f, starUnit.width);	// ���l�̓_��
	starUnit.height = std::max(0.0f, starUnit.height);	// ���l�̓_��

	// "*" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳���A
	// �S�Z���̃I�t�Z�b�g (�ʒu) ���m�肳����
	PointF totalOffset = PointF::Zero;
	for (auto col : m_columnDefinitions)
	{
		if (col->GetType() == GridLengthType::Ratio)
		{
			col->m_actualSize = starUnit.width * col->GetRatioWeight();
		}

		col->AdjustActualSize();

		// �Z��X���W�m��
		col->m_actualOffset = totalOffset.x;
		totalOffset.x += col->m_actualSize;
	}
	for (auto row : m_rowDefinitions)
	{
		if (row->GetType() == GridLengthType::Ratio)
		{
			row->m_actualSize = starUnit.height * row->GetRatioWeight();
		}

		row->AdjustActualSize();

		// �Z��Y���W�m��
		row->m_actualOffset = totalOffset.y;
		totalOffset.y += row->m_actualSize;
	}

	// �q�v�f�̍ŏI�ʒu�E�T�C�Y���m�肳����
	for (UIElement* child : *GetChildren())
	{
		int colIdx = child->GetLayoutColumn();
		int rowIdx = child->GetLayoutRow();
		int colSpan = child->GetLayoutColumnSpan();
		int rowSpan = child->GetLayoutRowSpan();
		colSpan = std::max(1, colSpan);	// �Œ� 1
		rowSpan = std::max(1, rowSpan);	// �Œ� 1
		colSpan = std::min(colSpan, colIdx + m_columnDefinitions.GetCount());	// �ő�l����
		rowSpan = std::min(rowSpan, rowIdx + m_rowDefinitions.GetCount());		// �ő�l����

		// Span ���l�����ăT�C�Y���m��
		RectF rect = RectF::Zero;
		if (m_columnDefinitions.IsEmpty())
		{
			rect.width = finalSize.width;
		}
		else
		{
			rect.x = m_columnDefinitions[colIdx]->m_actualOffset;
			for (int iCol = 0; iCol < colSpan; ++iCol)
			{
				rect.width += m_columnDefinitions[colIdx + iCol]->m_actualSize;
			}
		}
		if (m_rowDefinitions.IsEmpty())
		{
			rect.height = finalSize.height;
		}
		else
		{
			rect.y = m_rowDefinitions[rowIdx]->m_actualOffset;
			for (int iRow = 0; iRow < rowSpan; ++iRow)
			{
				rect.height += m_rowDefinitions[rowIdx + iRow]->m_actualSize;
			}
		}

		// Arrange
		child->ArrangeLayout(rect);
	}

	return finalSize;
}

LN_NAMESPACE_END
