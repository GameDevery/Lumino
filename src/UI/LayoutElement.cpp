
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

	const Size& layoutSize = GetLayoutSize();
	Size ds = GetLayoutDesiredSize();
	ds.width = Math::IsNaNOrInf(layoutSize.width) ? layoutSize.width : ds.width;
	ds.height = Math::IsNaNOrInf(layoutSize.height) ? layoutSize.height : ds.height;

	RectF arrangeRect;
	detail::LayoutHelper::AdjustHorizontalAlignment(areaSize, ds, hAlign, &arrangeRect);
	detail::LayoutHelper::AdjustVerticalAlignment(areaSize, ds, vAlign, &arrangeRect);

	// Margin ���l������ (0 �ȉ��ɂ͏o���Ȃ�)
	const ThicknessF& margin = GetLayoutMargin();
	float marginWidth = margin.Left + margin.Right;
	float marginHeight = margin.Top + margin.Bottom;
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

	const Size& size = GetLayoutSize();
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

LN_NAMESPACE_END
