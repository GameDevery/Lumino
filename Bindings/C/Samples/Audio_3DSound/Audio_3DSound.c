//==============================================================================
// 3D �T�E���h�̃T���v���ł��B
//==============================================================================
#include <stdio.h>
#include <LuminoC.h>

int main()
{
	LNHandle sound;
	LNVector3 soundPos = { 12.0, 0.0, 0.0 };
	LNVector3 listenerPos = { 5, 0.0, 0.0 };
	LNVector3 listenerDir = { 0.0, 0.0, 1.0 };
	LNVector3 listenerVel = { 2.0, 0.0, 0.0 };

	// �����@�\������������
	LNApplication_InitializeAudio();

	// �����t�@�C������ Sound �I�u�W�F�N�g�����
	LNSound_Create(_T("../Media/water02.wav"), &sound);
	LNSound_Set3DEnabled(sound, LN_TRUE);			// 3D �����Ƃ��čĐ�����
	LNSound_SetEmitterPosition(sound, &soundPos);	// �����ʒu
	LNSound_SetEmitterMaxDistance(sound, 10.0);		// ���̋����܂ŕ�������
	LNSound_SetLoopEnabled(sound, LN_TRUE);			// ���[�vON
	
	// ���X�i�[�̈ʒu�E�����E���x���w�肷��
	LNSoundListener_SetPosition(&listenerPos);
	LNSoundListener_SetDirection(&listenerDir);
	LNSoundListener_SetVelocity(&listenerVel);

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
