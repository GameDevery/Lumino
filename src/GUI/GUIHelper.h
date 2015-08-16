
#pragma once
#include <functional>
#include <Lumino/GUI/GUIManager.h>

namespace Lumino
{
namespace GUI
{
	
// �l�X�ȃI�u�W�F�N�g�� private �����o�ɃA�N�Z�X��������N���X�B
// ���̃N���X�͐�΂Ƀ��C�u�����̃��[�U�[�Ɍ�����Ƃ���Ɍ��J���Ă͂Ȃ�Ȃ��B
// �t���[�����[�N�̓s����A�����̃����o�� public �Ō��J���Ȃ���΂Ȃ�Ȃ����A����������͂��܂�ɂ��댯�Ȏ����B
// ��ނ𓾂� friend class �ɓ������B
// C# �� internal ���g����Ίy�Ȃ̂����E�E�E�B
class GUIHelper
{
public:
	template<class T, typename... TArgs>
	static RefPtr<T> CreateUIElemenInstance(TArgs... args) { return RefPtr<T>(T::internalCreateInstance(args...)); }

	static void GUIManager_AddAnimationClock(GUIManager* manager, AnimationClock* clock) { manager->AddAnimationClock(clock); }
	static void GUIManager_RemoveAnimationClock(GUIManager* manager, AnimationClock* clock) { manager->RemoveAnimationClock(clock); }


	static void UIElement_SetKeyName(UIElement* element, const String& name) { element->m_keyName = name; }
	static void UIElement_SetTemplateParent(UIElement* element, UIElement* templateParent) { element->m_templateParent = templateParent; }
	static Array< RefPtr<AnimationClock> >* UIElement_GetAnimationClockList(UIElement* element) { return &element->m_animationClockList; }


	static void ForEachVisualChildren(UIElement* element, std::function<void(UIElement* child)> func)
	{
		int count = element->GetVisualChildrenCount();
		for (int i = 0; i < count; ++i)
		{
			func(element->GetVisualChild(i));
		}
	}

	static bool SimpleMeasureOverrideSingleVisual(UIElement* element, const SizeF& constraint, SizeF* desiredSize)
	{
		int count = element->GetVisualChildrenCount();
		if (count > 0)
		{
			UIElement* child = element->GetVisualChild(0);
			if (child != NULL)
			{
				child->MeasureLayout(constraint);
				*desiredSize = child->GetDesiredSize();
				return true;
			}
		}
		return false;
	}

	static bool SimpleArrangeOverrideSingleVisual(UIElement* element, const SizeF& finalSize)
	{
		int count = element->GetVisualChildrenCount();
		if (count > 0)
		{
			UIElement* child = element->GetVisualChild(0);
			if (child != NULL)
			{
				child->ArrangeLayout(RectF(0, 0, finalSize));
				return true;
			}
		}
		return false;
	}
};

} // namespace GUI
} // namespace Lumino
