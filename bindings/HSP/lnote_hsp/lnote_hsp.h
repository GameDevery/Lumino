// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ LNOTE_HSP_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// LNOTE_HSP_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef LNOTE_HSP_EXPORTS
#define LNOTE_HSP_API __declspec(dllexport)
#else
#define LNOTE_HSP_API __declspec(dllimport)
#endif

// ���̃N���X�� lnote_hsp.dll ����G�N�X�|�[�g����܂����B
class LNOTE_HSP_API Clnote_hsp {
public:
	Clnote_hsp(void);
	// TODO: ���\�b�h�������ɒǉ����Ă��������B
};

extern LNOTE_HSP_API int nlnote_hsp;

LNOTE_HSP_API int fnlnote_hsp(void);
