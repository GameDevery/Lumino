
#include "Internal.h"
#include <Lumino/UI/UIPanel.h>
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIPanel
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIPanel, UIElement)

//------------------------------------------------------------------------------
UIPanel::UIPanel()
{
}

//------------------------------------------------------------------------------
UIPanel::~UIPanel()
{
}

//------------------------------------------------------------------------------
void UIPanel::Initialize(detail::UIManager* manager)
{
	UIElement::Initialize(manager);
	m_children = RefPtr<UIElementCollection>::MakeRef(this);

	// Panel �n�̃f�t�H���g�� Stretch
	SetHAlignment(HAlignment::Stretch);
	SetVAlignment(VAlignment::Stretch);
}

//------------------------------------------------------------------------------
void UIPanel::AddChild(UIElement* element)
{
	m_children->Add(element);
}

//------------------------------------------------------------------------------
void UIPanel::RemoveChild(UIElement* element)
{
	m_children->Remove(element);
}

//------------------------------------------------------------------------------
int UIPanel::GetVisualChildrenCount() const
{
	return m_children->GetCount();
}

//------------------------------------------------------------------------------
UIElement* UIPanel::GetVisualChildOrderd(int index) const
{
	return m_children->GetAt(index);
}

//------------------------------------------------------------------------------
SizeF UIPanel::MeasureOverride(const SizeF& constraint)
{
	SizeF desiredSize = UIElement::MeasureOverride(constraint);
	for (UIElement* child : *m_children)
	{
		child->MeasureLayout(constraint);
		const SizeF& childDesiredSize = child->GetDesiredSize();

		desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
		desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
	}
	return desiredSize;
}

//------------------------------------------------------------------------------
SizeF UIPanel::ArrangeOverride(const SizeF& finalSize)
{
	for (UIElement* child : *m_children)
	{
		SizeF childDesiredSize = child->GetDesiredSize();
		childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
		childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
		child->ArrangeLayout(RectF(0, 0, childDesiredSize));
	}
	return finalSize;
}

//------------------------------------------------------------------------------
void UIPanel::OnChildCollectionChanged(const tr::ChildCollectionChangedArgs& e)
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
LN_UI_TYPEINFO_IMPLEMENT(UIStackPanel, UIPanel)

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
	UIPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
SizeF UIStackPanel::MeasureOverride(const SizeF& constraint)
{
	SizeF size = constraint;

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

	SizeF desiredSize;
	for (UIElement* child : *GetChildren())
	{
		child->MeasureLayout(size);

		const SizeF& childDesiredSize = child->GetDesiredSize();
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
SizeF UIStackPanel::ArrangeOverride(const SizeF& finalSize)
{
	float prevChildSize = 0;
	RectF childRect;
	for (UIElement* child : *GetChildren())
	{
		const SizeF& childDesiredSize = child->GetDesiredSize();
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
LN_UI_TYPEINFO_IMPLEMENT(UICanvas, UIPanel)

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
	UIPanel::Initialize(manager);
}

//------------------------------------------------------------------------------
SizeF UICanvas::MeasureOverride(const SizeF& constraint)
{
	return UIPanel::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
SizeF UICanvas::ArrangeOverride(const SizeF& finalSize)
{
	//ThicknessF canvas;
	
	
	for (UIElement* child : *GetChildren())
	{
		//if (child->GetInvalidateFlags().TestFlag(detail::InvalidateFlags::ParentChangedUpdating))
		//{
		//}
		
		const SizeF& desiredSize = child->GetDesiredSize();
		SizeF size = child->GetSizeInternal();
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
	enum class ValueType
	{
		Auto,	// �q�v�f�̃T�C�Y�ɍ��킹�邩
		Pixel,	// �T�C�Y�𒼐ڎw�肷��
		Ratio,	// ���C�A�E�g��A�c��̗̈���g���� (Star)
	};

	DefinitionBase()
		: m_type(ValueType::Ratio)
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

	ValueType GetType() const { return m_type; }

	float GetAvailableDesiredSize() const
	{
		if (m_type == ValueType::Auto) {
			return m_desiredSize;
		}
		else if (m_type == ValueType::Pixel) {
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

protected:
	ValueType	m_type;
	float		m_size;
	float		m_minSize;
	float		m_maxSize;

public:
	float		m_desiredSize;
	float		m_actualOffset;	// �ŏI�I�t�Z�b�g
	float		m_actualSize;	// �ŏI�T�C�Y
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

	void SetWidth(float value, ValueType type = ValueType::Pixel) { m_size = value; m_type = type; }
	float GetWidth() const { return m_size; }

	void SetMinWidth(float value) { m_size = value; }
	float GetMinWidth() const { return m_size; }

	void SetMaxWidth(float value) { m_size = value; }
	float GetMaxWidth() const { return m_size; }
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

	void SetHeight(float value, ValueType type = ValueType::Pixel) { m_size = value; m_type = type; }
	float GetHeight() const { return m_size; }

	void SetMinHeight(float value) { m_size = value; }
	float GetMinHeight() const { return m_size; }

	void SetMaxHeight(float value) { m_size = value; }
	float GetMaxHeight() const { return m_size; }
};

//==============================================================================
// UIGridLayout
//==============================================================================
LN_UI_TYPEINFO_IMPLEMENT(UIGridLayout, UIPanel)

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
	UIPanel::Initialize(manager);
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
SizeF UIGridLayout::MeasureOverride(const SizeF& constraint)
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
		const SizeF& childDesiredSize = child->GetDesiredSize();
		if (col != nullptr && col->GetType() == DefinitionBase::ValueType::Auto)
		{
			col->m_desiredSize = std::max(col->m_desiredSize, childDesiredSize.width);
		}
		if (row != nullptr && row->GetType() == DefinitionBase::ValueType::Auto)
		{
			row->m_desiredSize = std::max(row->m_desiredSize, childDesiredSize.height);
		}
	}

	// �e�Z���� DesiredSize ���W�v���āAGrid �S�̂� DesiredSize �����߂�
	SizeF desiredSize = UIPanel::MeasureOverride(constraint);
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
SizeF UIGridLayout::ArrangeOverride(const SizeF& finalSize)
{
	// "Auto" �� "Pixel" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳����B
	// �܂��A"*" �ł���s��̐����J�E���g����B
	SizeF totalActual = SizeF::Zero;
	float starColCount = 0.0f;
	float starRowCount = 0.0f;
	for (auto col : m_columnDefinitions)
	{
		if (col->GetType() == DefinitionBase::ValueType::Auto || col->GetType() == DefinitionBase::ValueType::Pixel)
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
		if (row->GetType() == DefinitionBase::ValueType::Auto || row->GetType() == DefinitionBase::ValueType::Pixel)
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
	SizeF starUnit(
		(starColCount != 0.0f) ? (finalSize.width - totalActual.width) / starColCount : 0.0f,
		(starRowCount != 0.0f) ? (finalSize.height - totalActual.height) / starRowCount : 0.0f);
	starUnit.width = std::max(0.0f, starUnit.width);	// ���l�̓_��
	starUnit.height = std::max(0.0f, starUnit.height);	// ���l�̓_��

	// "*" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳���A
	// �S�Z���̃I�t�Z�b�g (�ʒu) ���m�肳����
	PointF totalOffset = PointF::Zero;
	for (auto col : m_columnDefinitions)
	{
		if (col->GetType() == DefinitionBase::ValueType::Ratio)
		{
			col->m_actualSize = starUnit.width;
		}

		// �Z��X���W�m��
		col->m_actualOffset = totalOffset.x;
		totalOffset.x += col->m_actualSize;
	}
	for (auto row : m_rowDefinitions)
	{
		if (row->GetType() == DefinitionBase::ValueType::Ratio)
		{
			row->m_actualSize = starUnit.height;
		}

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
