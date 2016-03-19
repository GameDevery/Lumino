/*
	[2016/3/16] ImageEffect �� OnUpdate �͎����Ȃ��B
		Graphics ���W���[���Ƃ��ẮA�t���[�����[�N�I�Ȏ��ԍX�V�̋@�\�͎��ׂ��ł͂Ȃ��Ǝv���B
		Particle �� Model �̓��[�U�[�ɒ��� Update �Ăяo���Ă��炤���A
		Scene ���W���[���Ń��b�v�����N���X����Ăяo�����B

		���ł��Ƃ����ƁAEditor ���[�h�ł� OnUpdate �͕��ʎg��Ȃ�����B
		���Ƀc�[���E�B���h�E�B
		�`�悪�K�v�ɂȂ����� Invalidate() ���čĕ`�悪�K�v�Ȃ��Ƃ������āA
		���Ƃł܂Ƃ߂ĕ`�悵�Ă��炤�Ƃ����X�^�C���B
		���̂Ƃ��̃A�j���[�V�����́AOnUpdate �ł͂Ȃ��A�j���[�V�������Ǘ����郂�W���[����
		AnimationClock �݂����� (WPF�I��) �̂�o�^���ăv���p�e�B�ɑ΂��ăA�j���[�V��������̂���ʓI�B

		������Ǝg���Â炭�Ȃ邩������Ȃ����ǁA�J�ڎ��Ԃ��w�肵�� OnUpdate �ŃA�j���[�V��������]���̕��@�͔p�~�B
		�����A����ς�g�������Ȃ邱�Ƃ͏o�Ă���Ǝv���̂ŁA
		Game ���[�h�p�̃A�j���[�V�����Ǘ����W���[���� AnimationClock ��o�^����悤��
		���[�e�B���e�B�֐������̂͌�X�K�v�����B
*/
#include "../Internal.h"
#include <Lumino/Graphics/ImageEffect/ImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ImageEffect
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ImageEffect, Object);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ImageEffect::ImageEffect()
	: m_manager(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ImageEffect::~ImageEffect()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ImageEffect::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
