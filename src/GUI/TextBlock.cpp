/*
	�ERichTextBox
	�E�A�j���[�V���� (�X�v���C�g�e�L�X�g)
	����2�́A�\������O���t�͓����B���������Ⴄ�����B

*/
#include "../Internal.h"
#include <Lumino/GUI/TextBlock.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// TextBlock
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBlock, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBlock);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::TextBlock(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::~TextBlock()
{
}

} // namespace GUI
} // namespace Lumino

