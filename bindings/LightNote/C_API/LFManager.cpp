
#pragma once
#include "LFInternal.h"
#include "LFManager.h"

namespace Lumino
{

//=============================================================================
// LFManager
//=============================================================================

Lumino::Application*	LFManager::Application = NULL;
bool					LFManager::IsSystemInitialized = false;

LFManager::ObjectEntryList	LFManager::m_objectEntryList;
Stack<int>					LFManager::m_objectIndexStack;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PreInitialize()
{
	if (Application == NULL)
	{
		//CheckCommonDefinition();
		Application = Lumino::Application::Create();
		IsSystemInitialized = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::PostInitialize()
{
	// �I�u�W�F�N�g�Ǘ��z��
	for (int i = 0; i < 512; ++i)
	{
		ObjectEntry e;
		e.Object = NULL;
		e.Index = i;
		m_objectEntryList.Add(e);

		if (i > 0) {  // 0 �� NULL �����B���Ԃɂ���B
			m_objectIndexStack.Push(i);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void LFManager::Finalize()
{
	// �c���Ă���I�u�W�F�N�g��S�č폜
	LN_FOREACH(ObjectEntry& e, m_objectEntryList) {
		LN_SAFE_RELEASE(e.Object);
	}

	if (Application != NULL)
	{
		//Application->Finalize();
		LN_SAFE_RELEASE(Application);
	}

	IsSystemInitialized = false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNHandle LFManager::CheckRegisterObject(CoreObject* obj)
{
	LN_VERIFY(obj != NULL) { return NULL; }

	// �o�^�ς݂Ȃ�n���h�� (�Ǘ��z���̃C���f�b�N�X) ��Ԃ�
	if (obj->GetUserData() != NULL) {
		return ((ObjectRegisterData*)obj->GetUserData())->Index;
	}

	// �Ǘ��z�񂪂��ׂĖ��܂��Ă���ꍇ
	if (m_objectIndexStack.IsEmpty())
	{
		// �����ɒǉ�����
		ObjectEntry e;
		e.Object = obj;
		//e.Interface = obj;
		e.Index = m_objectEntryList.GetCount();
		e.RefCount = 1;
		m_objectEntryList.Add(e);

		// ���[�U�[�f�[�^�o�^
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		return e.Index;
	}
	else
	{
		// �󂫏ꏊ���擾
		int newPos = m_objectIndexStack.GetTop();
		m_objectIndexStack.Pop();

		// �i�[
		ObjectEntry& e = m_objectEntryList[newPos];

		e.Object = obj;
		e.Index = newPos;
		e.RefCount = 1;

		// ���[�U�[�f�[�^�o�^
		ObjectRegisterData* data = LN_NEW ObjectRegisterData();
		data->Index = e.Index;
		obj->SetUserData(data);

		return e.Index;
	}
}

} // namespace Lumino
