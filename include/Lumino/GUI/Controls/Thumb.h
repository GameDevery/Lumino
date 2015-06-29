
#pragma once
#include "../UIElement.h"

namespace Lumino
{
namespace GUI
{

/**
	@brief
	@note		Thumb �̓}�E�X�h���b�O�ɂ��ړ��ʂ�ʒm���邽�߂̃R���g���[���B
				Thumb ���̂̈ʒu���ړ�������̂ł͂Ȃ��_�ɒ��ӁB
				ScrollBar �͂������AGridSpritter �� Thumb �̃T�u�N���X�B
*/
class Thumb
	: public Control
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Thumb);
public:
	Thumb(GUIManager* manager);
	virtual ~Thumb();
};

} // namespace GUI
} // namespace Lumino
