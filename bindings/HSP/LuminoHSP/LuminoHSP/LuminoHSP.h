// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ LUMINOHSP_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// LUMINOHSP_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef LUMINOHSP_EXPORTS
#define LUMINOHSP_API __declspec(dllexport)
#else
#define LUMINOHSP_API __declspec(dllimport)
#endif

// ���̃N���X�� LuminoHSP.dll ����G�N�X�|�[�g����܂����B
class LUMINOHSP_API CLuminoHSP {
public:
	CLuminoHSP(void);
	// TODO: ���\�b�h�������ɒǉ����Ă��������B
};

extern LUMINOHSP_API int nLuminoHSP;

LUMINOHSP_API int fnLuminoHSP(void);
