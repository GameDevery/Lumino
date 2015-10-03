#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../hsp3plugin/hsp3plugin.h"
#include "../hsp3plugin/hspwnd.h"
#include "../../../src/C_API/LuminoC.h"
#include "LuminoHSP.h"

void* retValPtr = NULL;

//-----------------------------------------------------------------------------
// ���ߎ��s���ɌĂяo�����
//-----------------------------------------------------------------------------
static int cmdfunc(int cmd)
{
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	int ret = 0;
	if (Commands_cmdfunc(cmd, &ret)) {
		return ret;
	}
	puterror(HSPERR_UNSUPPORTED_FUNCTION);
	return RUNMODE_RUN;
}

//-----------------------------------------------------------------------------
// �g�ݍ��ݕϐ��܂��͊֐����s���ɌĂ΂��
//-----------------------------------------------------------------------------
static void *reffunc(int *type_res, int cmd)
{
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//
	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	if (!Structs_reffunc(cmd, type_res, &retValPtr))
	{
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('�ŏI��邩�𒲂ׂ�
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	return retValPtr;
}

//-----------------------------------------------------------------------------
// HSP �I�����ɌĂяo�����
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Terminate();
	return 0;
}

//-----------------------------------------------------------------------------
// HSP �p�v���O�C���̃G���g���[�|�C���g
//-----------------------------------------------------------------------------
EXPORT void WINAPI hsp3cmdinit(HSP3TYPEINFO* info)
{
	//		�v���O�C�������� (���s�E�I��������o�^���܂�)
	//
	hsp3sdk_init(info);			// SDK�̏�����(�ŏ��ɍs�Ȃ��ĉ�����)
	info->cmdfunc = cmdfunc;		// ���s�֐�(cmdfunc)�̓o�^
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	/*
	//	�C�x���g�R�[���o�b�N�𔭐�������C�x���g��ʂ�ݒ肷��
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// �C�x���g�֐�(eventfunc)�̓o�^
	*/

	// �\���̌^�̓o�^
	RegisterStructTypes(info);
}

