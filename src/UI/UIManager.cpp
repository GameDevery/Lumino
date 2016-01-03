/*
	[2016/1/1] ���낢��ȃC�x���g�n���h�����O
		�E�I�[�o�[���C�h
			UIElement �����p�����AOnMouseMove() �����I�[�o�[���C�h������@�B

		�E���I�ȃn���h���o�^
			button.clicked += handler; �̂悤�ɁA�R���g���[�����g���l���C�x���g��o�^������@�B

		�E�ÓI�ȃn���h���o�^
			�I�[�o�[���C�h�Ƃ͕ʂɁA�h���N���X���x�[�X�ɑ΂��ăn���h���֐���o�^������@�B
			�ÓI�ȃn���h���́A���I�ȃn���h�����Ăяo�����O�ɌĂ΂��B

		WPF �ł́AOn�` �͓����� RaiseEvent ���Ăяo���̂��ړI�B
		�܂�A�q�v�f���烋�[�e�B���O�Œʒm����Ă����C�x���g�� On�` �Ńn���h�����O���邱�Ƃ͂ł��Ȃ��B

		Lumino �Ƃ��Ă� On�` �����[�e�B���O�ʒm�ŌĂяo�����悤�ɂ��Ă��܂��Ă����Ǝv���B
		�킩��₷���d���̕����ŁB

	[2016/1/1] �h�b�L���O�E�B���h�E�߂�
		���C���E�B���h�E�͒ʏ�ʂ�B
		AvalonDock �̂悤�ɁADockManager �R���g���[����z�u����B
		�T�u�E�B���h�E�� Platform ���W���[���� NativeWindow ���p�������E�B���h�E������������ǂ���������Ȃ��B

*/
#include "Internal.h"
#include <Lumino/UI/UIContext.h>
#include "EventArgsPool.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{
	
//=============================================================================
// UIManager
//=============================================================================
const float UIManager::MouseButtonClickTimeout = 0.3f;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIManager::UIManager()
	: m_eventArgsPool(nullptr)
	, m_graphicsManager(nullptr)
	, m_defaultUIContext(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIManager::~UIManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIManager::Initialize(const Settings& settings)
{
	m_eventArgsPool = LN_NEW EventArgsPool();
	m_graphicsManager = settings.graphicsManager;
	m_mainWindow = settings.mainWindow;

	m_defaultUIContext = LN_NEW UIContext();
	m_defaultUIContext->Initialize(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIManager::Finalize()
{
	LN_SAFE_RELEASE(m_defaultUIContext);
	LN_SAFE_DELETE(m_eventArgsPool);
}



} // namespace detail
LN_NAMESPACE_END
