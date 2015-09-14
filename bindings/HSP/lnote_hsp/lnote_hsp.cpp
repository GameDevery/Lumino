// lnote_hsp.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "hsp3plugin/hsp3plugin.h"
#include <lnote.h>

//#include "stdafx.h"
//#include "lnote_hsp.h"
//
//
//// ����́A�G�N�X�|�[�g���ꂽ�ϐ��̗�ł��B
//LNOTE_HSP_API int nlnote_hsp=0;
//
//// ����́A�G�N�X�|�[�g���ꂽ�֐��̗�ł��B
//LNOTE_HSP_API int fnlnote_hsp(void)
//{
//	return 42;
//}
//
//// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
//// �N���X��`�Ɋւ��Ă� lnote_hsp.h ���Q�Ƃ��Ă��������B
//Clnote_hsp::Clnote_hsp()
//{
//	return;
//}


int cmd_LNApplication_InitializeAudio()
{
	return LNApplication_InitializeAudio();
}
int cmd_LNAudio_PlayBGM()
{
	std::string p1 = code_gets();
	int p2 = code_getdi(100);
	int p3 = code_getdi(100);
	double p4 = code_getdd(0.0);
	return LNAudio_PlayBGM(p1.c_str(), p2, p3, p4);
}

static int cmdfunc(int cmd)
{
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch (cmd) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// newcmd

		p1 = code_getdi(123);		// �����l���擾(�f�t�H���g123)
		stat = p1;					// �V�X�e���ϐ�stat�ɑ��
		break;

	case 0x01:								// newcmd2
		stat = cmd_LNApplication_InitializeAudio();
		break;

	case 0x02:								// newcmd3
		stat = cmd_LNAudio_PlayBGM();
		break;

	case 0x03:								// newcmd4
		//newcmd4();
		break;

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Terminate();
	return 0;
}

//-----------------------------------------------------------------------------
// HSP �p�v���O�C���̃G���g���[�|�C���g
//-----------------------------------------------------------------------------
EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO *info)
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init(info);			// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)
	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = NULL;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	/*
	//	�C�x���g�R�[���o�b�N�𔭐�������C�x���g��ʂ�ݒ肷��
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// �C�x���g�֐�(eventfunc)�̓o�^
	*/
}
