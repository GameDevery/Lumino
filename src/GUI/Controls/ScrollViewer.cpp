/*
	ScrollViewer �̊T�v
	https://msdn.microsoft.com/ja-jp/library/ms750665(v=vs.110).aspx

	ScrollViewer �̊e��v���p�e�B
	http://smart-pda.net/wp7/tips/scrollviewer_tips/

	ScrollViewer �̓��\�b�h�Ăяo���� (��ʓI�ɂ͈�x�R�}���h�L���[�ɗ��ߍ����) 
	ScrollViewer.ScrollInfo �ɒʒm����B
	ScrollViewer ���q�v�f�̃I�t�Z�b�g���v�Z����̂ł͂Ȃ��_�ɒ��ӁB
	
	ScrollContentPresenter �� Child(IScrollInfo) ���ATemplatedParent(ScrollViewer).ScrollInfo �ɃZ�b�g����B
	(HookupScrollingComponents �Ō���)

	IScrollInfo �� StackPanel �� DocumentViewer�ATextBoxView ���Ƃ��Ď��������B
	���ۂɎq�v�f�̃I�t�Z�b�g���v�Z����̂͂����̃N���X�B
	������������邽�߂ɁA�e�N���X ScrollData �Ƃ��������N���X�������Ă���B

*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollViewer.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ScrollContentPresenter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollContentPresenter, ContentPresenter);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollContentPresenter);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::ScrollContentPresenter(GUIManager* manager)
	: ContentPresenter(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollContentPresenter::~ScrollContentPresenter()
{
}

//=============================================================================
// ScrollViewer
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollViewer);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::ScrollViewer(GUIManager* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::~ScrollViewer()
{
}

} // namespace GUI
} // namespace Lumino

