/*
	ScrollViewer �̊T�v
	https://msdn.microsoft.com/ja-jp/library/ms750665(v=vs.110).aspx

	ScrollViewer �̊e��v���p�e�B
	http://smart-pda.net/wp7/tips/scrollviewer_tips/

	ScrollViewer �̓��\�b�h�Ăяo���� (��ʓI�ɂ͈�x�R�}���h�L���[�ɗ��ߍ����) 
	ScrollContentPresenter �ɒʒm����B
	���ۂɎq�v�f�ɃI�t�Z�b�g�����Ĕz�u����̂� ScrollContentPresenter �̖�ځB
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
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollContentPresenter);
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
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer);
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

