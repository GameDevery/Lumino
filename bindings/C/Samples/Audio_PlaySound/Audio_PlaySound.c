//==============================================================================
// �����t�@�C�������[�v�Đ����܂��B
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	LNHandle sound;

	// �����@�\������������
	if (LNApplication_InitializeAudio() != LN_OK) return 1;

	// �����t�@�C������ Sound �I�u�W�F�N�g�����
	if (LNSound_Create(_T("../Media/ln23.ogg"), &sound) != LN_OK) return 1;
	LNSound_SetVolume(sound, 1.0);			// ����
	LNSound_SetPitch(sound, 1.5);			// �s�b�`
	LNSound_SetLoopEnabled(sound, LN_TRUE);	// ���[�vON

	// �Đ�
	LNSound_Play(sound);

	printf("Enter �������ƒ�~���܂�...\n");
	getchar();

	// ��~
	LNSound_Stop(sound);

	// ���C�u�����̏I������
	LNApplication_Terminate();
	return 0;
}
