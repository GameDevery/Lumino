
#pragma once
#include <Lumino/Application.h>
#include "LFCommon.h"

namespace Lumino
{

/// Handle �̊Ǘ����X�g�̗v�f
struct ObjectEntry
{
	CoreObject*		Object;		///< �I�u�W�F�N�g�{��
	void*			UserData;	///< �o�C���_������Z�b�g���郆�[�U�[�f�[�^�B��{�I�ɊǗ��z��̃C���f�b�N�X�ƂȂ�B
	int				Index;		///< ���̃I�u�W�F�N�g������Ǘ��z���̃C���f�b�N�X
	int				RefCount;	///< �O���Ɍ��J����Q�ƃJ�E���g (CoreObject �̎Q�ƃJ�E���g�Ƃ͕ʊǗ��ł���_�ɒ���)

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
	static void Finalize();

	/// obj �� Handle ��Ԃ��Bobj �����̃N���X�ɓo�^�ς݂����m�F���A�o�^����Ă��Ȃ���ΐV�����o�^����B
	static LNHandle CheckRegisterObject(CoreObject* obj);

public:
	static Lumino::Application*	Application;
	static bool					IsSystemInitialized;

private:
	typedef ArrayList<ObjectEntry>	ObjectEntryList;
	static ObjectEntryList		m_objectEntryList;
	static Stack<int>			m_objectIndexStack;

};

} // namespace Lumino
