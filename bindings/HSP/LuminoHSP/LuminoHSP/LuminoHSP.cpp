// LuminoHSP.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "../hsp3plugin/hsp3plugin.h"
#include "../../../src/C_API/LuminoC.h"

#include "LuminoHSP.h"


#define TRACE printf
#if 0
#define sbAlloc hspmalloc
#define sbFree hspfree


//#define LNVector3 float

/*
�ϐ��o�b�t�@(�o�C�i��)�̃|�C���^�ƃT�C�Y��Ԃ��܂��B
�v�f���ϒ��̏ꍇ�͊Y������P�z��̃o�C�i���݂̂��ΏۂƂȂ�A
�v�f���Œ蒷(int,double��)�̏ꍇ�͑S�z��o�C�i����Ԃ��܂��B
�܂��A(*size)�Ƀ������m�ۃT�C�Y��Ԃ��܂��B
*/
static void* hspCommon_GetBlockSize(PVal *pval, PDAT *pdat, int *size)
{
	TRACE("hspCommon_GetBlockSize\n");
	*size = pval->size - (((char *)pdat) - pval->pt);
	return (pdat);
}

static void hspCommon_AllocBlock(PVal *pval, PDAT *pdat, int size)
{
	TRACE("hspCommon_AllocBlock\n");
}





//static short* g_LNVector3_aftertype;
static int g_LNVector3_typeid = 0;



static int GetVarSize(PVal *pval)
{
	//		PVAL�|�C���^�̕ϐ����K�v�Ƃ���T�C�Y���擾����
	//		(size�t�B�[���h�ɐݒ肳���)
	//
	int size;
	size = pval->len[1];
	if (pval->len[2]) size *= pval->len[2];
	if (pval->len[3]) size *= pval->len[3];
	if (pval->len[4]) size *= pval->len[4];
	size *= sizeof(LNVector3);
	return size;
}


/*
	pval�ϐ����K�v�Ƃ���T�C�Y���m�ۂ����������s�Ȃ��܂��B
	pval2��NULL�̏ꍇ�́A�V�K�f�[�^�ɂȂ�܂��B
	pval2���w�肳��Ă���ꍇ�́Apval2�̓��e���p�����čĊm�ۂ�
	�s�Ȃ����Ƃ������Ă��܂��B���̏ꍇ�Apval2��size�Apt�����Ƃ�
	���e���R�s�[����悤�ɂ��Ă��������B�܂��Apt�̉����������
	�s�Ȃ��K�v������܂��B

	��:
		if ( pval2 != NULL ) {
			memcpy( pt, pval->pt, pval->size );
			free( pval->pt );
		}
*/
static void hspLNVector3_Alloc(PVal *pval, const PVal *pval2)
{
	TRACE("hspLNVector3_Alloc\n");
	//		pval�ϐ����K�v�Ƃ���T�C�Y���m�ۂ���B
	//		(pval�����łɊm�ۂ���Ă��郁��������͌Ăяo�������s�Ȃ�)
	//		(flag�̐ݒ�͌Ăяo�������s�Ȃ�)
	//		(pval2��NULL�̏ꍇ�́A�V�K�f�[�^)
	//		(pval2���w�肳��Ă���ꍇ�́Apval2�̓��e���p�����čĊm��)
	//
	//int i;
#if 0
	if (pval->len[1] < 1) pval->len[1] = 1;		// �z����Œ�1�͊m�ۂ���
	int size = GetVarSize(pval);//sizeof(LNVector3) * pval->len[1];//GetVarSize(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	char* pt = sbAlloc(size);
	LNVector3* fv = (LNVector3 *)pt;
	memset(fv, 0, size);
	//for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { fv[i] = 0.0; }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
#endif

	int i, size;
	char *pt;
	LNVector3 *fv;
	if (pval->len[1] < 1) pval->len[1] = 1;		// �z����Œ�1�͊m�ۂ���
	size = GetVarSize(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	pt = sbAlloc(size);
	fv = (LNVector3 *)pt;
	for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { /*fv[i] = 0.0;*/ }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
	TRACE("hspLNVector3_Alloc e\n");
}

/*
pval���m�ۂ��Ă��郁������������܂��B
�����̏ꍇ�AHSP3�R�A�������I�ɌĂяo�����s�Ȃ��܂��B
*/
static void hspLNVector3_Free(PVal *pval)
{
	TRACE("hspLNVector3_Free\n");
	//		PVAL�|�C���^�̕ϐ����������������
	//
	if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}

/*
PVal�������f�[�^�̎��Ԃ�����擪�|�C���^��Ԃ��܂��B
(PDAT*�́A���ԃf�[�^�̃|�C���^�������Ă��܂��B)
�z��ϐ����T�|�[�g���Ă���ꍇ�́A�I�t�Z�b�g���(PVal->offset)��
���ƂɃf�[�^�̎��ԃ|�C���^�����߂�K�v������܂��B
*/
static PDAT* hspLNVector3_GetPtr(PVal *pval)
{
	TRACE("hspLNVector3_GetPtr\n");
	return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
	//return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
}

static void* hspLNVector3_Cnv(const void *buffer, int flag)
{
	TRACE("hspLNVector3_Cnv\n");
	throw HSPVAR_ERROR_TYPEMISS;
	return (void *)buffer;
}

static void* hspLNVector3_CnvCustom(const void *buffer, int flag)
{
	TRACE("hspLNVector3_CnvCustom\n");
	throw HSPVAR_ERROR_TYPEMISS;
	return (void *)buffer;
}

/*
pdat���������e�̃T�C�Y(�o�C�g��)��Ԃ��܂��B
���̊֐��́A�P�v�f�̃f�[�^�T�C�Y���ϒ�(basesize��-1)�̎���
�Ăяo����܂��B�ʏ�́Abasesize�Ɠ����T�C�Y��߂��Ă�����
�������B
*/
static int hspLNVector3_GetSize(const PDAT *pdatl)
{
	TRACE("hspLNVector3_GetSize\n");
	return sizeof(LNVector3);
}

static int hspLNVector3_GetUsing(const PDAT *pdat)
{
	return 1;
}

#define GetPtr(pval) ((LNVector3 *)pval)
/*
	������Z�q
*/
static void hspLNVector3_Set(PVal *pval, PDAT *pdat, const void *in)
{
	TRACE("hspLNVector3_Set\n");
	*GetPtr(pval) = *((LNVector3*)(in));
	//printf("%f, %f, %f\n", ((LNVector3*)pval)->X, ((LNVector3*)pval)->Y, ((LNVector3*)pval)->Z);
}


//static int GetVarSize(PVal *pval)
//{
//	//		PVAL�|�C���^�̕ϐ����K�v�Ƃ���T�C�Y���擾����
//	//		(size�t�B�[���h�ɐݒ肳���)
//	//
//	int size;
//	size = pval->len[1];
//	if (pval->len[2]) size *= pval->len[2];
//	if (pval->len[3]) size *= pval->len[3];
//	if (pval->len[4]) size *= pval->len[4];
//	size *= sizeof(float);
//	return size;
//}


EXPORT int hspLNVector3_typeid()
{
	TRACE("hspLNVector3_typeid\n");
	return g_LNVector3_typeid;
}

EXPORT void hspLNVector3_Init(HspVarProc *p)
{
	TRACE("hspLNVector3_Init\n");
	//aftertype = &p->aftertype;
	
	/*
	���Z��Ƀ^�C�v���ύX����鉉�Z�q�����ꍇ�́A���̃t�B�[���h��
	���Z��̃^�C�v�l���V�X�e���ɒʒm���܂��B
	*/
	//g_LNVector3_aftertype = &p->aftertype;

	p->Set = hspLNVector3_Set;

	p->GetPtr = hspLNVector3_GetPtr;
	//p->Cnv = hspLNVector3_Cnv;
	//p->CnvCustom = hspLNVector3_CnvCustom;
	p->GetSize = hspLNVector3_GetSize;
	p->GetBlockSize = hspCommon_GetBlockSize;
	p->AllocBlock = hspCommon_AllocBlock;
	//p->GetUsing = hspLNVector3_GetUsing;

	p->Alloc = hspLNVector3_Alloc;
	p->Free = hspLNVector3_Free;


	//p->ArrayObject = HspVarVariant_ArrayObject;
	//p->ArrayObjectRead = HspVarVariant_ArrayObjectRead;
	//p->ObjectWrite = HspVarVariant_ObjectWrite;
	//p->ObjectMethod = HspVarVariant_ObjectMethod;

	//p->AddI = HspVarFloat_AddI;
	//p->SubI = HspVarFloat_SubI;
	//p->MulI = HspVarFloat_MulI;
	//p->DivI = HspVarFloat_DivI;
	//p->ModI = HspVarFloat_Invalid;
	//p->AndI = HspVarFloat_Invalid;
	//p->OrI  = HspVarFloat_Invalid;
	//p->XorI = HspVarFloat_Invalid;
	//p->EqI = HspVarFloat_EqI;
	//p->NeI = HspVarFloat_NeI;
	//p->GtI = HspVarFloat_GtI;
	//p->LtI = HspVarFloat_LtI;
	//p->GtEqI = HspVarFloat_GtEqI;
	//p->LtEqI = HspVarFloat_LtEqI;
	//p->RrI = HspVarFloat_Invalid;
	//p->LrI = HspVarFloat_Invalid;

	p->vartype_name = "LNVector3";				// �^�C�v��
	p->version = 0x001;					// �^�^�C�v�����^�C���o�[�W����(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE | /*HSPVAR_SUPPORT_VARUSE | */HSPVAR_SUPPORT_FLEXARRAY;/* | HSPVAR_SUPPORT_FLEXARRAY*/;
	// �T�|�[�g�󋵃t���O(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(LNVector3);		// �P�̃f�[�^���g�p����T�C�Y(byte) / �ϒ��̎���-1
	g_LNVector3_typeid = p->flag;
}

















//
//
//// ����́A�G�N�X�|�[�g���ꂽ�ϐ��̗�ł��B
//LUMINOHSP_API int nLuminoHSP=0;
//
//// ����́A�G�N�X�|�[�g���ꂽ�֐��̗�ł��B
//LUMINOHSP_API int fnLuminoHSP(void)
//{
//	return 42;
//}
//
//// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
//// �N���X��`�Ɋւ��Ă� LuminoHSP.h ���Q�Ƃ��Ă��������B
//CLuminoHSP::CLuminoHSP()
//{
//	return;
//}











#endif


/*------------------------------------------------------------*/
/*
HSPVAR core interface (float)
*/
/*------------------------------------------------------------*/

#define GetPtr(pval) ((LNVector3 *)pval)
#define sbAlloc hspmalloc
#define sbFree hspfree

static int mytype;
//static float conv;
//static short *aftertype;
//static char custom[64];

// Core
static PDAT *HspVarFloat_GetPtr(PVal *pval)
{
	return (PDAT *)(((LNVector3 *)(pval->pt)) + pval->offset);
}

//static void *HspVarFloat_Cnv(const void *buffer, int flag)
//{
//	//		���N�G�X�g���ꂽ�^ -> �����̌^�ւ̕ϊ����s�Ȃ�
//	//		(�g�ݍ��݌^�ɂ̂ݑΉ���OK)
//	//		(�Q�ƌ��̃f�[�^��j�󂵂Ȃ�����)
//	//
//	switch (flag) {
//	case HSPVAR_FLAG_STR:
//		conv = (float)atof((char *)buffer);
//		return &conv;
//	case HSPVAR_FLAG_INT:
//		conv = (float)(*(int *)buffer);
//		return &conv;
//	case HSPVAR_FLAG_DOUBLE:
//		conv = (float)(*(double *)buffer);
//		break;
//	default:
//		throw HSPVAR_ERROR_TYPEMISS;
//	}
//	return (void *)buffer;
//}


//static void *HspVarFloat_CnvCustom(const void *buffer, int flag)
//{
//	//		(�J�X�^���^�C�v�̂�)
//	//		�����̌^ -> ���N�G�X�g���ꂽ�^ �ւ̕ϊ����s�Ȃ�
//	//		(�g�ݍ��݌^�ɑΉ�������)
//	//		(�Q�ƌ��̃f�[�^��j�󂵂Ȃ�����)
//	//
//	float p;
//	p = *(float *)buffer;
//	switch (flag) {
//	case HSPVAR_FLAG_STR:
//		sprintf(custom, "%f", p);
//		break;
//	case HSPVAR_FLAG_INT:
//		*(int *)custom = (int)p;
//		break;
//	case HSPVAR_FLAG_DOUBLE:
//		*(double *)custom = (double)p;
//		break;
//	default:
//		throw HSPVAR_ERROR_TYPEMISS;
//	}
//	return custom;
//}


static int GetVarSize2(PVal *pval)
{
	//		PVAL�|�C���^�̕ϐ����K�v�Ƃ���T�C�Y���擾����
	//		(size�t�B�[���h�ɐݒ肳���)
	//
	int size;
	size = pval->len[1];
	if (pval->len[2]) size *= pval->len[2];
	if (pval->len[3]) size *= pval->len[3];
	if (pval->len[4]) size *= pval->len[4];
	size *= sizeof(LNVector3);
	return size;
}


static void HspVarFloat_Free(PVal *pval)
{
	//		PVAL�|�C���^�̕ϐ����������������
	//
	if (pval->mode == HSPVAR_MODE_MALLOC) { sbFree(pval->pt); }
	pval->pt = NULL;
	pval->mode = HSPVAR_MODE_NONE;
}


static void HspVarFloat_Alloc(PVal *pval, const PVal *pval2)
{
	//		pval�ϐ����K�v�Ƃ���T�C�Y���m�ۂ���B
	//		(pval�����łɊm�ۂ���Ă��郁��������͌Ăяo�������s�Ȃ�)
	//		(flag�̐ݒ�͌Ăяo�������s�Ȃ�)
	//		(pval2��NULL�̏ꍇ�́A�V�K�f�[�^)
	//		(pval2���w�肳��Ă���ꍇ�́Apval2�̓��e���p�����čĊm��)
	//
	int i, size;
	char *pt;
	LNVector3 *fv;
	if (pval->len[1] < 1) pval->len[1] = 1;		// �z����Œ�1�͊m�ۂ���
	size = GetVarSize2(pval);
	pval->mode = HSPVAR_MODE_MALLOC;
	pt = sbAlloc(size);
	fv = (LNVector3 *)pt;
	for (i = 0; i<(int)(size / sizeof(LNVector3)); i++) { /*fv[i] = 0.0;*/ }
	if (pval2 != NULL) {
		memcpy(pt, pval->pt, pval->size);
		sbFree(pval->pt);
	}
	pval->pt = pt;
	pval->size = size;
}

/*
static void *HspVarFloat_ArrayObject( PVal *pval, int *mptype )
{
//		�z��v�f�̎w�� (������/�A�z�z��p)
//
throw HSPERR_UNSUPPORTED_FUNCTION;
return NULL;
}
*/

// Size
static int HspVarFloat_GetSize(const PDAT *pval)
{
	return sizeof(LNVector3);
}

// Set
static void HspVarFloat_Set(PVal *pval, PDAT *pdat, const void *in)
{
	*GetPtr(pdat) = *((LNVector3 *)(in));
	printf("%f, %f, %f\n", ((LNVector3*)pdat)->X, ((LNVector3*)pdat)->Y, ((LNVector3*)pdat)->Z);
}


static void *GetBlockSize(PVal *pval, PDAT *pdat, int *size)
{
	*size = pval->size - (((char *)pdat) - pval->pt);
	return (pdat);
}

static void AllocBlock(PVal *pval, PDAT *pdat, int size)
{
}


/*------------------------------------------------------------*/

EXPORT int HspVarFloat_typeid(void)
{
	return mytype;
}


EXPORT void HspVarFloat_Init(HspVarProc *p)
{
	//aftertype = &p->aftertype;

	p->Set = HspVarFloat_Set;
	//p->Cnv = HspVarFloat_Cnv;
	p->GetPtr = HspVarFloat_GetPtr;
	//p->CnvCustom = HspVarFloat_CnvCustom;
	p->GetSize = HspVarFloat_GetSize;
	p->GetBlockSize = GetBlockSize;
	p->AllocBlock = AllocBlock;

	//	p->ArrayObject = HspVarFloat_ArrayObject;
	p->Alloc = HspVarFloat_Alloc;
	p->Free = HspVarFloat_Free;

	//p->AddI = HspVarFloat_AddI;
	//p->SubI = HspVarFloat_SubI;
	//p->MulI = HspVarFloat_MulI;
	//p->DivI = HspVarFloat_DivI;
	//	p->ModI = HspVarFloat_Invalid;

	//	p->AndI = HspVarFloat_Invalid;
	//	p->OrI  = HspVarFloat_Invalid;
	//	p->XorI = HspVarFloat_Invalid;

	//p->EqI = HspVarFloat_EqI;
	//p->NeI = HspVarFloat_NeI;
	//p->GtI = HspVarFloat_GtI;
	//p->LtI = HspVarFloat_LtI;
	//p->GtEqI = HspVarFloat_GtEqI;
	//p->LtEqI = HspVarFloat_LtEqI;


	//	p->RrI = HspVarFloat_Invalid;
	//	p->LrI = HspVarFloat_Invalid;

	p->vartype_name = "LNVector3";				// �^�C�v��
	p->version = 0x001;					// �^�^�C�v�����^�C���o�[�W����(0x100 = 1.0)
	p->support = HSPVAR_SUPPORT_STORAGE | HSPVAR_SUPPORT_FLEXARRAY;
	// �T�|�[�g�󋵃t���O(HSPVAR_SUPPORT_*)
	p->basesize = sizeof(LNVector3);		// �P�̃f�[�^���g�p����T�C�Y(byte) / �ϒ��̎���-1
	mytype = p->flag;
}

/*------------------------------------------------------------*/






//-----------------------------------------------------------------------------
// ���ߎ��s���ɌĂяo�����
//-----------------------------------------------------------------------------
static int cmdfunc(int cmd)
{
	TRACE("cmdfunc\n");
	//		���s���� (���ߎ��s���ɌĂ΂�܂�)
	//
	code_next();							// ���̃R�[�h���擾(�ŏ��ɕK���K�v�ł�)

	switch (cmd) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// newcmd

		p1 = code_getdi(123);		// �����l���擾(�f�t�H���g123)
		stat = p1;					// �V�X�e���ϐ�stat�ɑ��
		break;

	//case 0x01:								// newcmd2
	//	stat = cmd_LNApplication_InitializeAudio();
	//	break;

	//case 0x02:								// newcmd3
	//	stat = cmd_LNAudio_PlayBGM();
	//	break;

	case 0x10:								// newcmd4
		//newcmd4();
	{
		std::string p1 = code_gets();
		PVal* p2;
		printf("p2\n");
		code_getva(&p2);
		printf("p2->flag: %d\n", p2->flag);
		if (p2->flag != HspVarFloat_typeid()) { throw HSPVAR_ERROR_TYPEMISS; }
		float p3 = code_getd();
		printf("call\n");
		stat = LNAudio_PlaySE3D(p1.c_str(), (LNVector3*)p2->pt, p3);
		break;
	}

	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}
	return RUNMODE_RUN;
}

static LNVector3 retVal;
void* retValPtr = NULL;
//-----------------------------------------------------------------------------
// �g�ݍ��ݕϐ��܂��͊֐����s���ɌĂ΂��
//-----------------------------------------------------------------------------
static void *reffunc(int *type_res, int cmd)
{
	TRACE("reffunc s\n");
	//		�֐��E�V�X�e���ϐ��̎��s���� (�l�̎Q�Ǝ��ɌĂ΂�܂�)
	//
	//			'('�Ŏn�܂邩�𒲂ׂ�
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != '(') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();


	switch (cmd) {							// �T�u�R�}���h���Ƃ̕���

	case 0x00:								// float�֐�
	{
		int r = code_getprm();
		printf("r:%d\n", r);
		double p1 = *((double*)mpval->pt);// code_getd();
		printf("r:%d\n", r);
		puterror(HSPERR_NO_DEFAULT);	// long jump
		printf("rf:%d\n", r);
		double p2 = code_getd();
		double p3 = code_getd();
		retVal.X = p1;
		retVal.Y = p2;
		retVal.Z = p3;
		retValPtr = &retVal;
		break;
	}
	default:
		puterror(HSPERR_UNSUPPORTED_FUNCTION);
	}

	//			'('�ŏI��邩�𒲂ׂ�
	//
	if (*type != TYPE_MARK) puterror(HSPERR_INVALID_FUNCPARAM);
	if (*val != ')') puterror(HSPERR_INVALID_FUNCPARAM);
	code_next();

	TRACE("reffunc e\n");
	//*type_res = HspVarFloat_typeid();		// �Ԓl�̃^�C�v���w�肷��
	//*type_res = hspLNVector3_typeid();
	*type_res = HspVarFloat_typeid();
	return retValPtr;//(void *)&ref_fval;
}

//-----------------------------------------------------------------------------
// HSP �I�����ɌĂяo�����
//-----------------------------------------------------------------------------
static int termfunc(int option)
{
	LNApplication_Finalize();
	printf("LNApplication_Finalize\n");
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
	info->reffunc = reffunc;		// �Q�Ɗ֐�(reffunc)�̓o�^
	info->termfunc = termfunc;		// �I���֐�(termfunc)�̓o�^

	/*
	//	�C�x���g�R�[���o�b�N�𔭐�������C�x���g��ʂ�ݒ肷��
	info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// �C�x���g�֐�(eventfunc)�̓o�^
	*/


	LNConfig_SetConsoleEnabled(LN_TRUE);
	LNApplication_InitializeAudio();

	// �V�����^�̒ǉ�
	registvar(-1, HspVarFloat_Init);
	//registvar(-1, hspLNVector3_Init);
	printf("hsp3cmdinit e\n");


	printf("LNApplication_InitializeAudio\n");
}

