
#include "Internal.h"
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIInput.h>
#include "UIHelper.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIMouse
//==============================================================================

Point UIMouse::getPosition(UIElement* relativeTo)
{
	// TODO: �}�E�X�ʒu�̓X�N���[����̂��Ƃ��悤�ɂ����ق�����������
	UILayoutView* root = static_cast<UILayoutView*>(UIHelper::getLayoutRoot(relativeTo));
	const Point& pos = root->getMousePosition();

	const Rect& rc = relativeTo->getFinalGlobalRect();
	return Point(pos.x - rc.x, pos.y - rc.y);
}

LN_NAMESPACE_END
