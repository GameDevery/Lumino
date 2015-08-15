
#include "../Internal.h"
#include <Lumino/GUI/VisualTreeHelper.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// VisualTreeHelper
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElement* VisualTreeHelper::FindChildByName(UIElement* parent, const TCHAR* name)
{
	LN_CHECK_ARGS_RETURNV(parent != NULL, NULL);
	LN_CHECK_ARGS_RETURNV(name != NULL, NULL);

	/* TODO: WPF �ł̓c���[�����ł͂Ȃ��A�r�W���A���c���[����������Ɉꏏ�� map ������Ă���B
		���� map �� internal�B
	*/

	int count = parent->GetVisualChildrenCount();
	for (int i = 0; i < count; ++i)
	{
		UIElement* child = parent->GetVisualChild(i);
		if (child->GetKeyName() == name) {
			return child;
		}

		// �ċA
		child = FindChildByName(child, name);
		if (child != NULL) {
			return child;
		}
	}

	return NULL;
}

} // namespace GUI
} // namespace Lumino
