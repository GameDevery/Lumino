//==============================================================================
// �Q�[���p�̉����Đ��@�\�̃T���v���ł��B
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	// �����@�\������������
	LNApplication_InitializeAudio();

	printf("Enter �������� BGM ���Đ����܂�...\n");
	getchar();	// Enter �҂�

	// BGM ���Đ�����
	LNGameAudio_PlayBGM(_T("../Media/ln23.ogg"), 1, 1, 0);

	printf("Enter �������� BGS ���Đ����܂�...\n");
	getchar();	// Enter �҂�

	// BGS ���Đ�����
	LNGameAudio_PlayBGS(_T("../Media/water02.wav"), 0.5, 1, 0);

	printf("Enter �������� ME ���Đ����܂�...\n");
	getchar();	// Enter �҂�

	// ME ���Đ�����
	LNGameAudio_PlayME(_T("../Media/lnme_victory1.ogg"), 1, 1);

	printf("Enter �������� SE ���Đ����܂�...\n");
	getchar();	// Enter �҂�

	// SE ���Đ�����
	LNGameAudio_PlaySE(_T("../Media/ln_cursor_1.wav"), 1, 1);

	printf("Enter �������� BGM �� BGS ���~���܂�...\n");
	getchar();	// Enter �҂�

	// BGM ���~���� (5�b�����ăt�F�[�h�A�E�g)
	LNGameAudio_StopBGM(5.0);

	// BGS ���~����
	LNGameAudio_StopBGS(0.0);

	printf("Enter �������ƏI�����܂�...\n");
	getchar();	// Enter �҂�

	// ���C�u�����̏I������
	LNApplication_Terminate();
	return 0;
}
