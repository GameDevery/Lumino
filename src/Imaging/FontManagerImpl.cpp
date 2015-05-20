/*
�E�\�t�g�E�F�A�`��
BitmapFont
OutlineFont
�E�n�[�h�E�F�A�`��
PolygonFont		�c OutlineFont �̒��_�����g���B�܂͂� BitmapFont+��`
TextureFont		�c OutlineFont �܂��� BitmapFont �̃����_�����O���ʂ��g��

�� �J�[�j���O�ɂ���
GUI::RichText �ŏ����Ƃ����ASpan ���܂����ł��J�[�j���O�͈ێ������B
�� <red>i</red><blue>j</blue> �Ƃ��B

�J�[�j���O�̗ʂ�Font�N���X����Ƃ邯�ǁAGUI ���x���ł̓A���r�A��Ƃ���
TextBox.RightToLeft �v���p�e�B�ɂ���ĕ`��������ς��B

GUI �� FlowDocument(Span) ���x���ŃJ�[�j���O�͎g����悤�ɂ������B
�ʂɃJ�[�j���O�������Ȃ��Ă��ǂ����E�E�E�B

textRenderer.BeginLine()
for (span, spans) {
textRenderer.DrawLine(span.Text, span.Font, span.pen, span.blush);
}
textRenderer.EndLine()

������ textRenderer �� GUI �ŕ`�� Renderer�B

�� �����P�ʂ̃A�j���[�V�����Ƃ��̃G�t�F�N�g�́H
AfterEffects �́A"�e�L�X�g���C���[" �Ȃ���̂� "�G�t�F�N�g" ���A�^�b�`���邱�Ƃōs���B
�e�L�X�g���C���[�͕����̕`��� TextRenderer ���g�p���邪�A�ʒu�͎����Ō��߂�B
1�������Ƃɒ��_�o�b�t�@��p�ӂ��邩�A���b�V���݂����� 1�̒��_�o�b�t�@�� Subset �ŕ�������Ƃ���肴�܂͍l����K�v�����肻���B

�E�X�p�C�����_�E��
�E�u���[
�� ���Ȃ荂���x���ȕ����̋@�\�Ɗ֌W����B���b�V���Ɠ��������ɂ����ق����ǂ������B
*/
#include "../Internal.h"
#include "FontManagerImpl.h"

namespace Lumino
{
namespace Imaging
{

//=============================================================================
// FontManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager* FontManager::Create(FileManager* fileManager)
{
	return LN_NEW FontManagerImpl(fileManager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::FontManager()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManager::~FontManager()
{
}

//=============================================================================
// FontManagerImpl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManagerImpl::FontManagerImpl(FileManager* fileManager)
	: m_fileManager(fileManager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
FontManagerImpl::~FontManagerImpl()
{
}

} // namespace Imaging
} // namespace Lumino
