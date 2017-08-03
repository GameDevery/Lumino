
#pragma once

LN_NAMESPACE_BEGIN
namespace detail {

class LayoutHelper2
{
public:

	//------------------------------------------------------------------------------
	template<class TElement>
	static void measureLayout(TElement* element, const Size& availableSize)
	{
		if (element->isLayoutVisible())
		{
			// Margin �� Padding ���l������
			const Thickness& margin = element->getLayoutMargin();
			//const Thickness& padding = element->getLayoutPadding();
			float spaceWidth = (margin.left + margin.right);// +(padding.left + padding.right);
			float spaceHeight = (margin.top + margin.bottom);// +(padding.top + padding.bottom);
			Size localAvailableSize(
				std::max(availableSize.width - spaceWidth, 0.0f),
				std::max(availableSize.height - spaceHeight, 0.0f));

			Size desiredSize = element->measureOverride(localAvailableSize);

			// Margin ���l������
			desiredSize.width += spaceWidth;
			desiredSize.height += spaceHeight;

			element->setLayoutDesiredSize(desiredSize);
		}
		else
		{
			element->setLayoutDesiredSize(Size::Zero);
		}
	}

	//------------------------------------------------------------------------------
	template<class TElement>
	static void arrangeLayout(TElement* element, const Rect& finalLocalRect)
	{
		if (element->isLayoutVisible())
		{
			// finalLocalRect �͂��̗v�f��z�u�ł���̈�T�C�Y�B�ƁA�e�v�f���ł̃I�t�Z�b�g�B
			// �v�f�ɒ��ڐݒ肳��Ă���T�C�Y�����傫�����Ƃ�����B
			// TODO: HorizontalAlignment �����l�����āA�ŏI�I�ȍ��W�ƃT�C�Y�����肷��B
			//		 ���̗v�f�̃T�C�Y���ȗ�����Ă���΁AStretch �Ȃ�T�C�Y�͍ő�ɁA����ȊO�Ȃ�ŏ��ɂȂ�B

			const Size& areaSize = finalLocalRect.getSize();

#if 1
			HAlignment		hAlign = element->getLayoutHAlignment();
			VAlignment		vAlign = element->getLayoutVAlignment();
#else
			ILayoutElement* parent = GetLayoutParent();
			HAlignment		hAlign = GetLayoutHAlignment();
			VAlignment		vAlign = GetLayoutVAlignment();
			const HAlignment* parentHAlign = (parent != nullptr) ? parent->GetLayoutContentHAlignment() : nullptr;
			const VAlignment* parentVAlign = (parent != nullptr) ? parent->GetLayoutContentVAlignment() : nullptr;
			if (parentHAlign != nullptr) hAlign = *parentHAlign;
			if (parentVAlign != nullptr) vAlign = *parentVAlign;
#endif

			//Size ds;// = getLayoutDesiredSize();
			//ds.width = Math::IsNaNOrInf(layoutSize.width) ? finalLocalRect.width : layoutSize.width;
			//ds.height = Math::IsNaNOrInf(layoutSize.height) ? finalLocalRect.height : layoutSize.height;
			Size ds = element->getLayoutDesiredSize();
			LN_ASSERT(!Math::isNaNOrInf(ds.width));
			LN_ASSERT(!Math::isNaNOrInf(ds.height));

			// DesiredSize �� Margin �l���ς�

			// Alignment �Œ�������̈�́Amargin �̈���܂�
			const Thickness& margin = element->getLayoutMargin();
			float marginWidth = margin.left + margin.right;
			float marginHeight = margin.top + margin.bottom;

			Size layoutSize = element->getLayoutSize();
			Rect arrangeRect;
			detail::LayoutHelper::adjustHorizontalAlignment(areaSize, ds, Math::isNaN(layoutSize.width), hAlign, &arrangeRect);
			detail::LayoutHelper::adjustVerticalAlignment(areaSize, ds, Math::isNaN(layoutSize.height), vAlign, &arrangeRect);

			// Margin ���l������ (0 �ȉ��ɂ͏o���Ȃ�)
			arrangeRect.width = std::max(arrangeRect.width - marginWidth, 0.0f);
			arrangeRect.height = std::max(arrangeRect.height - marginHeight, 0.0f);

			// Padding ���l������
			//const Thickness& padding = element->getLayoutPadding();

			//Size contentAreaSize(
			//	std::max(arrangeRect.width - padding.getWidth(), 0.0f),
			//	std::max(arrangeRect.height - padding.getHeight(), 0.0f));
			Size contentAreaSize(
				std::max(arrangeRect.width, 0.0f),
				std::max(arrangeRect.height, 0.0f));
			Size finalContentAreaSize = element->arrangeOverride(contentAreaSize);

			Rect finalRenderRect;
			finalRenderRect.x = finalLocalRect.x + margin.left + arrangeRect.x;
			finalRenderRect.y = finalLocalRect.y + margin.top + arrangeRect.y;
			finalRenderRect.width = finalContentAreaSize.width;// +padding.getWidth();
			finalRenderRect.height = finalContentAreaSize.height;// +padding.getHeight();
			//Rect finalContentRect;
			//finalContentRect.x = finalRenderRect.x + padding.left;
			//finalContentRect.y = finalRenderRect.y + padding.top;
			//finalContentRect.width = finalRenderRect.width - padding.getWidth();
			//finalContentRect.height = finalRenderRect.height - padding.getHeight();
			element->setLayoutFinalLocalRect(finalRenderRect/*, finalContentRect*/);
		}
		else
		{
			element->setLayoutFinalLocalRect(Rect::Zero/*, Rect::Zero*/);
		}
	}
};

} // namespace detail
LN_NAMESPACE_END
