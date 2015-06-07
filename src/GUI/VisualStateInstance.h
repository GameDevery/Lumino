
#pragma once
#include <Lumino/GUI/VisualState.h>

namespace Lumino
{
namespace GUI
{

/// �f�[�^�N���X�ł��� VisualStateManager�AVisualStateGroup�AVisualState �ɑ΂���ʂ� UIElement ���Ǘ�������B
class VisualStateInstance
{
public:
	VisualStateInstance() {}
	~VisualStateInstance() {}

	/// VisualStateManager ��ύX����
	void SetVisualStateManager(VisualStateManager* manager);

private:
	RefPtr<VisualStateManager>	m_manager;
};


} // namespace GUI
} // namespace Lumino
