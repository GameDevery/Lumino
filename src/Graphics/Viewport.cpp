/*

	- Viewport �� Layer �̉�B
	- Layer �� Camera �� UI �̕`���ƂȂ郌���_�[�^�[�Q�b�g�ƍl����B


	Viewport
	- Viewport �͕K�������_�[�^�[�Q�b�g�����B

	RenderingUnit (RenderingCompositeItem)
	- Camera �� UIManager? �̃x�[�X�N���X�ƂȂ�B
	- RenderingUnit �� Viewport �̎q�BCamera �ȂǂɁu���� RT �ɏ����Ăˁv��`����̂��d���B�|�X�g�G�t�F�N�g�͂��܂��B�ʂ̃N���X�ɕ����Ă��ǂ��B
	- Render ���Ăׂ΃|�X�g�G�t�F�N�g���݂ŕ`�悳���B
	- �������ݐ�̃����_�[�^�[�Q�b�g�����B���� RenderingUnit �Ƌ��L���Ă���o�b�N�o�b�t�@��������Ȃ����A�X�i�b�v�V���b�g��ۑ�����Ɨ��������̂�������Ȃ��B
	- ClearMode �̓���������ōs���B
	- �|�X�g�G�t�F�N�g�p�� RT �̃L���b�V�����ARenderingUnit �P�ʂƂȂ�BRenderingUnit ���܂����ꍇ�͎g���܂킵�Ă悢�B
	- �D��x�͊֌W�Ȃ��B���C���[���ŕ`�悷��B

	RT �̃L���b�V��

	�P�[�X�F�}���J�[

	�P�[�X�FSnapShotCamera ���ʏ�� Camera �ɐ؂�ւ�邱�Ƃ͂���H
		�Ď��J�����̐؂�ւ��Ƃ��H
		�� SnapShot �� RT �� Sprite �� 2D �r���[�ɕ\�����܂��傤�B


	��	�܂Ƃ�

	Viewport �́u�X�N���[���ɕ\�����邽�߂̃����_�[�^�[�Q�b�g�v�ƍl����B
	�����_�����O�J�n�O�� Viewport ���W�v���A���̃t���[���ŁA��Ƀ|�X�g�G�t�F�N�g�Ɏg����
	�����_�[�^�[�Q�b�g���L���b�V������̂��d���B

	���̃����_�[�^�[�Q�b�g�́u���� Viewport ���I�[�i�[�Ƃ��鎩�����T�C�Y���郌���_�[�^�[�Q�b�g�v�Ƃ����
	Scene �� Camera �Ƃ� ImageEffect �Ƃ�����͓Ɨ����Ďg����B Graphics �ɓ���Ă����Ă������B


	- �����̃J�����̃����_�����O��1�̃r���[�|�[�g�ɂ܂Ƃ߂����B
	- �|�X�g�G�t�F�N�g�̓J�����Ɋ��蓖�Ă����B���O�����C�y�ɁB
	- �����J������1�̃r���[�|�[�g�ɋ��L���ď������ޏꍇ�A��̃J�����̃|�X�g�G�t�F�N�g�͑O�̃J�����̕`�挋�ʂɉe������B
		���Ȃ��悤�ɂ���͓̂���B�|�X�g�G�t�F�N�g�J�n�O�͊�{�I�ɃN���A������B
		�������Ȃ��悤�ɂ������Ȃ烌���_�[�^�[�Q�b�g�𕪂��Ă��炤�B

	-	�N���A�̗L����w�i�F�́AViewport �� Camera ���ꂼ�ꂪ���B
		Camera �̃N���A�́AViewport �Ƃ͊֌W�Ȃ��AScene �̃X�i�b�v�V���b�g���B�鎞�Ɏg���ׂ��B�f�t�H���g�� OFF�B
		�P�ɃE�B���h�E�w�i�F��ς������Ƃ��̂Ƃ��� Viewport �̔w�i�F��ύX����B

	-	�J�����ɂ́A�|�X�g�G�t�F�N�g�J�n��AScene�̃����_�����O���n�߂�O�ɁA
		�J�����ɕ`���Ƃ��Ďw�肳��Ă��郌���_�[�^�[�Q�b�g�̓��e�����݂̃����_�[�^�[�Q�b�g�ɓ]������B
		����ŁA���łɕ`��ς݂̃V�[���ɏ㏑������`�Ń|�X�g�G�t�F�N�g��K�p�ł���B
		�I�v�V�����ɂ��Ă����������B

	- �J�����̕`�揇�̎w��͂ǂ�����H
		Viewport �� Camera �͓Ɨ����������B
		Viewport �͂����܂Łu�����ɏ����Ă˃����_�����O�^�[�Q�b�g�v�����J���邾���B
		����� Camera �ɃZ�b�g������A���邢�� UI �����ڏ������肷��B
		Camera �̕`�揇�� Scene ���Ő��䂷��K�v������B

		Unity �� �D��x(Camera.Depth) UE4 �̓T�|�[�g����ĂȂ��Hhttps://answers.unrealengine.com/questions/53233/render-in-offset-order.html

		Viewport �ɂԂ牺���郌�C���[�ŊǗ�����H
		�� �ł��AViewport ����Ɨ����Ă��� SceneSnapShot �Ƃ��̗D��x�͂ǂ�����Ďw�肷��H
			�� �����ɂ͕s�\�ȋC������B
				Scene ���ƁA����̏�ʂɂ��� Viewport+Layer�H
				Layer �� �X�̗D��x �̏��Ń\�[�g���Ă͂ǂ����H
				�D��x�� SnapShot �p�J���������ɕt���Ă����������B�E�B���h�E�ɕ`�悷��̂͊�{�I�Ɉ�ԍŌゾ���B


	�|�X�g�G�t�F�N�g�̕`�����������T�C�Y������
	�� �T�C�Y���L�[�Ƃ��ăL���b�V���������B

	- Viewport �� Layer ����������Ȃ�A���� Layer ��1�̃|�X�g�G�t�F�N�g���܂ޕ`��̂܂Ƃ܂�ƂȂ�B

	- UE4 �� FRCPassPostProcessAA::Process �Ƃ��Q�l�ɂȂ邩���B

	- UE4 �݂����ɂ���ƁA1�̃|�X�g�G�t�F�N�g�I�u�W�F�N�g�͕����̃J�����Ȃǂ̃r���[����Q�Ƃ����B
		�� �ŗL�̏�Ԃ����̂͂�낵���Ȃ��B�E�B���h�E�T�C�Y�Ƃ��B



*/
#include "Internal.h"
#include <Lumino/Graphics/Viewport.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// Viewport
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Viewport::Viewport()
	: m_renderTarget(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Viewport::~Viewport()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Viewport::Initialize(RenderTarget* renderTarget)
{
	m_renderTarget = renderTarget;
}

LN_NAMESPACE_END