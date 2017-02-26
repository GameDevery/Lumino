
#include "Internal.h"
#include <Lumino/UI/LayoutElement.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ILayoutElement
//==============================================================================

//------------------------------------------------------------------------------
ILayoutElement::ILayoutElement()
{
}

//------------------------------------------------------------------------------
ILayoutElement::~ILayoutElement()
{
}

//------------------------------------------------------------------------------
void ILayoutElement::UpdateLayout(const Size& viewSize)
{
	Size itemSize = GetLayoutSize();
	Size size(
		Math::IsNaNOrInf(itemSize.width) ? viewSize.width : itemSize.width,
		Math::IsNaNOrInf(itemSize.height) ? viewSize.height : itemSize.height);

	// �T�C�Y����܂��Ă��Ȃ��ꍇ�̓��C�A�E�g������ł��Ȃ�
	// TODO: ��O�̕����ǂ������H
	//if (Math::IsNaNOrInf(m_size.Width) || Math::IsNaNOrInf(m_size.Height)) { return; }

	MeasureLayout(size);
	ArrangeLayout(RectF(0, 0, size.width, size.height));
	UpdateTransformHierarchy(RectF(0, 0, size));
}

//------------------------------------------------------------------------------
void ILayoutElement::MeasureLayout(const Size& availableSize)
{
	//// �������t���O�����̗v�f�ɓ`�d������
	//if (m_parent != nullptr)
	//{
	//	// �t�H���g�� MeasureOverride() �̒��ōX�V����
	//	m_invalidateFlags |= (m_parent->m_invalidateFlags & detail::InvalidateFlags::Font);
	//}

	// �e�v�f����q�v�f��z�u�ł���͈�(availableSize)���󂯎��ADesiredSize ���X�V����B
	// �@ Pane �\[measure()   �c ���͈͓̔��Ȃ�z�u�ł����]�� Button
	// �A Pane ��[DesiredSize �c ���Ⴀ���̃T�C�Y�ł��肢���܂�]�\ Button		�����̎��_�� inf ��Ԃ����Ƃ����蓾��B
	// �B Pane �\[arrange()   �c ���̎q�v�f�Ƃ̌��ˍ����ōŏI�T�C�Y�̓R����]�� Button
	// http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_ctrl_arrange.htm

	// Margin ���l������
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	Size localAvailableSize(
		std::max(availableSize.width - marginWidth, 0.0f),
		std::max(availableSize.height - marginHeight, 0.0f));

	Size desiredSize = MeasureOverride(localAvailableSize);

	// Margin ���l������
	desiredSize.width += marginWidth;
	desiredSize.height += marginHeight;

	SetLayoutDesiredSize(desiredSize);
}

//------------------------------------------------------------------------------
void ILayoutElement::ArrangeLayout(const RectF& finalLocalRect)
{
	// finalLocalRect �͂��̗v�f��z�u�ł���̈�T�C�Y�B�ƁA�e�v�f���ł̃I�t�Z�b�g�B
	// �v�f�ɒ��ڐݒ肳��Ă���T�C�Y�����傫�����Ƃ�����B
	// TODO: HorizontalAlignment �����l�����āA�ŏI�I�ȍ��W�ƃT�C�Y�����肷��B
	//		 ���̗v�f�̃T�C�Y���ȗ�����Ă���΁AStretch �Ȃ�T�C�Y�͍ő�ɁA����ȊO�Ȃ�ŏ��ɂȂ�B

	const Size& areaSize = finalLocalRect.GetSize();

	ILayoutElement* parent = GetLayoutParent();
	HAlignment		hAlign = GetLayoutHAlignment();
	VAlignment		vAlign = GetLayoutVAlignment();
	const HAlignment* parentHAlign = (parent != nullptr) ? parent->GetLayoutContentHAlignment() : nullptr;
	const VAlignment* parentVAlign = (parent != nullptr) ? parent->GetLayoutContentVAlignment() : nullptr;
	if (parentHAlign != nullptr) hAlign = *parentHAlign;
	if (parentVAlign != nullptr) vAlign = *parentVAlign;

	Size layoutSize = GetLayoutSize();
	Size ds;// = GetLayoutDesiredSize();
	ds.width = Math::IsNaNOrInf(layoutSize.width) ? finalLocalRect.width : layoutSize.width;
	ds.height = Math::IsNaNOrInf(layoutSize.height) ? finalLocalRect.height : layoutSize.height;

	// Alignment �Œ�������̈�́Amargin �̈���܂�
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
	ds.width += marginWidth;
	ds.height += marginHeight;

	RectF arrangeRect;
	detail::LayoutHelper::AdjustHorizontalAlignment(areaSize, ds, hAlign, &arrangeRect);
	detail::LayoutHelper::AdjustVerticalAlignment(areaSize, ds, vAlign, &arrangeRect);

	// Margin ���l������ (0 �ȉ��ɂ͏o���Ȃ�)
	arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
	arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);


	Size renderSize = ArrangeOverride(arrangeRect.GetSize());
	RectF thisFinalLocalRect;
	thisFinalLocalRect.x = finalLocalRect.x + margin.Left + arrangeRect.x;
	thisFinalLocalRect.y = finalLocalRect.y + margin.Top + arrangeRect.y;
	thisFinalLocalRect.width = renderSize.width;
	thisFinalLocalRect.height = renderSize.height;
	SetLayoutFinalLocalRect(thisFinalLocalRect);
}

//------------------------------------------------------------------------------
Size ILayoutElement::MeasureOverride(const Size& constraint)
{
	// �߂�l�́Aconstraint �̐����̒��ŁA�q�v�f�����C�A�E�g���邽�߂ɕK�v�ȍŏ��T�C�Y�B
	// ���[�U�[�w��̃T�C�Y������ꍇ�͂����Ԃ��B
	// �������Aconstraint �𒴂��邱�Ƃ͂ł��Ȃ��B

	Size size = GetLayoutSize();
	Size desiredSize;
	// NaN �̏ꍇ�A���̗v�f�Ƃ��ĕK�v�ȍŏ��T�C�Y�� 0 �ƂȂ�B
	desiredSize.width = Math::IsNaNOrInf(size.width) ? 0.0f : size.width;
	desiredSize.height = Math::IsNaNOrInf(size.height) ? 0.0f : size.height;
	desiredSize.width = std::min(desiredSize.width, constraint.width);
	desiredSize.height = std::min(desiredSize.height, constraint.height);

	return desiredSize;
}

//------------------------------------------------------------------------------
Size ILayoutElement::ArrangeOverride(const Size& finalSize)
{
	return finalSize;
}

//------------------------------------------------------------------------------
void ILayoutElement::UpdateTransformHierarchy(const RectF& parentGlobalRect)
{
	const RectF& localRect = GetLayoutFinalLocalRect();
	RectF finalGlobalRect;
	//if (m_parent != nullptr)
	//{
	finalGlobalRect.x = parentGlobalRect.x + localRect.x;
	finalGlobalRect.y = parentGlobalRect.y + localRect.y;
		//m_combinedOpacity = m_parent->m_combinedOpacity * m_opacity;	// �s�����x���R�R�ō����Ă��܂�
	//}
	//else
	//{
	//	m_finalGlobalRect.x = m_finalLocalRect.x;
	//	m_finalGlobalRect.y = m_finalLocalRect.y;
	//	m_combinedOpacity = m_opacity;
	//}
	finalGlobalRect.width = localRect.width;
	finalGlobalRect.height = localRect.height;

	SetLayoutFinalGlobalRect(finalGlobalRect);

	// update children
	int childCount = GetVisualChildrenCount();
	for (int i = 0; i < childCount; i++)
	{
		ILayoutElement* child = GetVisualChild(i);
		child->UpdateTransformHierarchy(finalGlobalRect);
	}

	// �q�v�f
	//UIHelper::ForEachVisualChildren(this, [](UIElement* child) { child->UpdateTransformHierarchy(); });

}

LN_NAMESPACE_END
