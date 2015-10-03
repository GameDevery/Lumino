
#pragma once
#include "../../../src/ApplicationImpl.h"	// TODO
#include <Lumino/Application.h>
#include "../include/LNCommon.h"

/// Handle �̊Ǘ����X�g�̗v�f
struct ObjectEntry
{
	CoreObject*		Object;		///< �I�u�W�F�N�g�{��
	void*			UserData;	///< �o�C���_������Z�b�g���郆�[�U�[�f�[�^�B��{�I�ɊǗ��z��̃C���f�b�N�X�ƂȂ�B
	int				Index;		///< ���̃I�u�W�F�N�g������Ǘ��z���̃C���f�b�N�X
	int				RefCount;	///< �O���Ɍ��J����Q�ƃJ�E���g (���̒l�𒼐ڑ��삵�Ȃ����ƁB�K�� Manager �o�R�ő��삷��BCoreObject �̎Q�ƃJ�E���g�Ƃ͕ʊǗ��ł���_�ɒ���)

	ObjectEntry()
	{
		Object = NULL;
		UserData = NULL;
		Index = 0;
		RefCount = 0;
	}
};

/// CoreObject �̃��[�U�[�f�[�^�Ƃ��ēo�^����
struct ObjectRegisterData
{
	int				Index;		///< ���̃I�u�W�F�N�g������Ǘ��z���̃C���f�b�N�X
};
	
/// C_API �Ǘ��N���X
class LFManager
{
public:

	/// Application �������O����
	static void PreInitialize();

	/// Application �������㏈��
	static void PostInitialize();

	/// �I������
	static void Terminate();

	/// obj �� Handle ��Ԃ��Bobj �����̃N���X�ɓo�^�ς݂����m�F���A�o�^����Ă��Ȃ���ΐV�����o�^����B
	static LNHandle CheckRegisterObject(CoreObject* obj);

	/// ��O�������̏��� (Exeption �ȊO�̗�O�� NULL ���w�肵�ČĂяo�����Ƃ� Unknown ���Ԃ�)
	static LNResult ProcException(Exception* e);

	/// �I�u�W�F�N�g�̎Q�ƃJ�E���g���f�N�������g (CoreObject �ł͂Ȃ� ObjectEntry �̃J�E���g�𑀍삷��)
	static void ReleaseObject(LNHandle handle);

	/// �I�u�W�F�N�g�̎Q�ƃJ�E���g���C���N�������g (CoreObject �ł͂Ȃ� ObjectEntry �̃J�E���g�𑀍삷��)
	static void AddRefObject(LNHandle handle);

	/// handle �ɑ΂��� ObjectEntry ���擾����
	static ObjectEntry* GetObjectEntry(LNHandle handle);

public:
	static Lumino::ApplicationSettings	ConfigData;
	static Lumino::ApplicationImpl*		Application;
	static bool							IsSystemInitialized;
	static Exception*					LastException;
	static LNResult						LastErrorCode;
	static EncodingConverter*			TCharToUTF8Converter;

private:
	typedef Array<ObjectEntry>	ObjectEntryList;
	static ObjectEntryList		m_objectEntryList;
	static Stack<int>			m_objectIndexStack;

};

