
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

template<class TPanel>
class LayoutImpl
{
public:
	using BaseMeasureOverrideCallback = Size(*)(TPanel* panel, const Size& constraint);

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_measureOverride(TPanel* panel, const Size& constraint, BaseMeasureOverrideCallback baseCallback)
	{
		Size desiredSize = baseCallback(static_cast<TPanel*>(panel), constraint);
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);

			child->measureLayout(constraint);
			const Size& childDesiredSize = child->getLayoutDesiredSize();

			desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
			desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
		}
		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UILayoutPanel_arrangeOverride(TPanel* panel, const Vector2& offset, const Size& finalSize)
	{
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);
			Size childDesiredSize = child->getLayoutDesiredSize();
			childDesiredSize.width = std::max(finalSize.width, childDesiredSize.width);
			childDesiredSize.height = std::max(finalSize.height, childDesiredSize.height);
			child->arrangeLayout(Rect(offset.x, offset.y, childDesiredSize));
		}
		return finalSize;
	}

	//------------------------------------------------------------------------------
	static Size UIStackPanel_measureOverride(ILayoutPanel* panel, const Size& constraint, Orientation orientation)
	{
		Size size = constraint;

		if (orientation == Orientation::Horizontal)
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
		int childCount = panel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = panel->getLayoutChild(i);
			child->measureLayout(size);

			const Size& childDesiredSize = child->getLayoutDesiredSize();
			if (orientation == Orientation::Horizontal || orientation == Orientation::ReverseHorizontal)
			{
				desiredSize.width += childDesiredSize.width;
				desiredSize.height = std::max(desiredSize.height, childDesiredSize.height);
			}
			else
			{
				desiredSize.width = std::max(desiredSize.width, childDesiredSize.width);
				desiredSize.height += child->getLayoutDesiredSize().height;
			}
		}

		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UIStackPanel_arrangeOverride(TPanel* panel, const Size& finalSize, Orientation orientation)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		Size childrenBoundSize(finalSize.width, finalSize.height);

		float prevChildSize = 0;
		float rPos = 0;
		Rect childRect(0, 0, 0, 0);
		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);
			const Size& childDesiredSize = child->getLayoutDesiredSize();

			switch (orientation)
			{
			case Orientation::Horizontal:
				childRect.x += prevChildSize;
				prevChildSize = childDesiredSize.width;
				childRect.width = prevChildSize;
				childRect.height = childrenBoundSize.height;
				break;
			case Orientation::Vertical:
				childRect.y += prevChildSize;
				prevChildSize = childDesiredSize.height;
				childRect.width = childrenBoundSize.width;
				childRect.height = prevChildSize;
				break;
			case Orientation::ReverseHorizontal:
				prevChildSize = childDesiredSize.width;
				rPos -= prevChildSize;
				childRect.x = childrenBoundSize.width + rPos;
				childRect.width = prevChildSize;
				childRect.height = childrenBoundSize.height;
				break;
			case Orientation::ReverseVertical:
				prevChildSize = childDesiredSize.height;
				rPos -= prevChildSize;
				childRect.y = childrenBoundSize.height + rPos;
				childRect.width = childrenBoundSize.width;
				childRect.height = prevChildSize;
				break;
			default:
				assert(0);
				break;
			}

			child->arrangeLayout(childRect);
		}

		return finalSize;
	}

	//------------------------------------------------------------------------------
	static Size UIGridLayout_measureOverride(TPanel* panel, const Size& constraint, BaseMeasureOverrideCallback baseCallback)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		int colDefCount = basePanel->getLayoutGridColumnDefinitionCount();
		int rowDefCount = basePanel->getLayoutGridRowDefinitionCount();

		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);

			// �܂��͎q�� measure
			child->measureLayout(constraint);

			// child ���z�u�����ׂ� column �� row ��T��
			int colIdx = child->getLayoutColumn();
			int rowIdx = child->getLayoutRow();

			colIdx = (0 <= colIdx && colIdx < colDefCount) ? colIdx : 0;
			rowIdx = (0 <= rowIdx && rowIdx < rowDefCount) ? rowIdx : 0;

			detail::GridDefinitionData* col = (colIdx < colDefCount) ? basePanel->getLayoutGridColumnDefinition(colIdx) : nullptr;
			detail::GridDefinitionData* row = (rowIdx < rowDefCount) ? basePanel->getLayoutGridRowDefinition(rowIdx) : nullptr;

			// �q�v�f�� DesiredSize (�Œ�T�C�Y) �𑪂�̂́A�Z���̃T�C�Y�w�肪 "Auto" �̎������ł悢�B
			const Size& childDesiredSize = child->getLayoutDesiredSize();
			if (col != nullptr && col->type == GridLengthType::Auto)
			{
				col->desiredSize = std::max(col->desiredSize, childDesiredSize.width);
			}
			if (row != nullptr && row->type == GridLengthType::Auto)
			{
				row->desiredSize = std::max(row->desiredSize, childDesiredSize.height);
			}
		}

		// �e�Z���� DesiredSize ���W�v���āAGrid �S�̂� DesiredSize �����߂�
		Size desiredSize = baseCallback(panel, constraint);
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			desiredSize.width += basePanel->getLayoutGridColumnDefinition(iCol)->getAvailableDesiredSize();
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			desiredSize.height += basePanel->getLayoutGridRowDefinition(iRow)->getAvailableDesiredSize();
		}

		return desiredSize;
	}

	//------------------------------------------------------------------------------
	static Size UIGridLayout_arrangeOverride(TPanel* panel, const Size& finalSize)
	{
		ILayoutPanel* basePanel = static_cast<ILayoutPanel*>(panel);
		Size childrenBoundSize(finalSize.width, finalSize.height);

		// "Auto" �� "Pixel" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳����B
		// �܂��A"*" �ł���s��̐����J�E���g����B
		Size totalActual = Size::Zero;
		float starColCount = 0.0f;
		float starRowCount = 0.0f;
		int colDefCount = basePanel->getLayoutGridColumnDefinitionCount();
		int rowDefCount = basePanel->getLayoutGridRowDefinitionCount();
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			auto* col = basePanel->getLayoutGridColumnDefinition(iCol);
			if (col->type == GridLengthType::Auto || col->type == GridLengthType::Pixel)
			{
				col->actualSize = col->getAvailableDesiredSize();
				totalActual.width += col->actualSize;
			}
			else
			{
				starColCount += col->getRatioSize();
			}
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			auto* row = basePanel->getLayoutGridRowDefinition(iRow);
			if (row->type == GridLengthType::Auto || row->type == GridLengthType::Pixel)
			{
				row->actualSize = row->getAvailableDesiredSize();
				totalActual.height += row->actualSize;
			}
			else
			{
				starRowCount += row->getRatioSize();
			}
		}

		// "1*" ���̃Z���̗̈���v�Z����
		Size starUnit(
			(starColCount != 0.0f) ? (childrenBoundSize.width - totalActual.width) / starColCount : 0.0f,
			(starRowCount != 0.0f) ? (childrenBoundSize.height - totalActual.height) / starRowCount : 0.0f);
		starUnit.width = std::max(0.0f, starUnit.width);	// ���l�̓_��
		starUnit.height = std::max(0.0f, starUnit.height);	// ���l�̓_��

		// "*" �w��ł��� Column/Row �̍ŏI�T�C�Y���m�肳���A
		// �S�Z���̃I�t�Z�b�g (�ʒu) ���m�肳����
		PointF totalOffset;
		for (int iCol = 0; iCol < colDefCount; iCol++)
		{
			auto* col = basePanel->getLayoutGridColumnDefinition(iCol);
			if (col->type == GridLengthType::Ratio)
			{
				col->actualSize = starUnit.width * col->getRatioSize();
			}

			col->adjustActualSize();

			// �Z��X���W�m��
			col->actualOffset = totalOffset.x;
			totalOffset.x += col->actualSize;
		}
		for (int iRow = 0; iRow < rowDefCount; iRow++)
		{
			auto* row = basePanel->getLayoutGridRowDefinition(iRow);
			if (row->type == GridLengthType::Ratio)
			{
				row->actualSize = starUnit.height * row->getRatioSize();
			}

			row->adjustActualSize();

			// �Z��Y���W�m��
			row->actualOffset = totalOffset.y;
			totalOffset.y += row->actualSize;
		}

		// �q�v�f�̍ŏI�ʒu�E�T�C�Y���m�肳����
		int childCount = basePanel->getLayoutChildrenCount();
		for (int i = 0; i < childCount; i++)
		{
			ILayoutElement* child = basePanel->getLayoutChild(i);
			int colIdx = child->getLayoutColumn();
			int rowIdx = child->getLayoutRow();
			int colSpan = child->getLayoutColumnSpan();
			int rowSpan = child->getLayoutRowSpan();
			colSpan = std::max(1, colSpan);	// �Œ� 1
			rowSpan = std::max(1, rowSpan);	// �Œ� 1
			colSpan = std::min(colSpan, colIdx + colDefCount);	// �ő�l����
			rowSpan = std::min(rowSpan, rowIdx + rowDefCount);	// �ő�l����

			// Span ���l�����ăT�C�Y���m��
			Rect rect = Rect(0, 0, 0, 0);
			if (colDefCount == 0)	// is empty
			{
				rect.width = childrenBoundSize.width;
			}
			else
			{
				rect.x += basePanel->getLayoutGridColumnDefinition(colIdx)->actualOffset;
				for (int iCol = 0; iCol < colSpan; ++iCol)
				{
					rect.width += basePanel->getLayoutGridColumnDefinition(colIdx + iCol)->actualSize;
				}
			}
			if (rowDefCount == 0)	// is empty
			{
				rect.height = childrenBoundSize.height;
			}
			else
			{
				rect.y += basePanel->getLayoutGridRowDefinition(rowIdx)->actualOffset;
				for (int iRow = 0; iRow < rowSpan; ++iRow)
				{
					rect.height += basePanel->getLayoutGridRowDefinition(rowIdx + iRow)->actualSize;
				}
			}

			// Arrange
			child->arrangeLayout(rect);
		}

		return finalSize;
	}
};

} // namespace detail
LN_NAMESPACE_END
