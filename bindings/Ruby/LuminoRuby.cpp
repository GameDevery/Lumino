#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

std::vector<TypeInfo>	Manager::m_typeInfoList;
std::vector<VALUE>		Manager::m_objectList;
std::stack<int>			Manager::m_objectListIndexStack;

//------------------------------------------------------------------------------
void Manager::Initialize()
{
	// �Ǘ����X�g�̋󂫔ԍ����l�߂�
	for (int i = 0; i < InitialListSize; i++)
	{
		m_objectList.push_back(Qnil);
		m_objectListIndexStack.push(i);
	}
	
	// Ruby �n�̏�����
	InitEnums();
	InitStructs();
	InitClasses();
	
	// �^���̓o�^
	m_typeInfoList.push_back(TypeInfo());	// �_�~�[��1�l�߂Ă���
	RegisterTypeInfo();
}

//------------------------------------------------------------------------------
void Manager::Finalize()
{
}

//------------------------------------------------------------------------------
VALUE Manager::GetWrapperObjectFromHandle(LNHandle handle)
{
	int objectIndex = (int)LNObject_GetUserData(handle);
	if (objectIndex == 0)
	{
		int typeinfoIndex = (int)LNObject_GetBindingTypeData(handle);
		VALUE obj = m_typeInfoList[typeinfoIndex].factory(m_typeInfoList[typeinfoIndex].klass, handle);
		RegisterWrapperObject(obj);
		return obj;
	}
	else
	{
		return m_objectList[objectIndex];
	}
}

//------------------------------------------------------------------------------
void Manager::RegisterWrapperObject(VALUE obj)
{
	// �Ǘ����X�g����t�̎��͊g������
	if (m_objectListIndexStack.size() == 0)
	{
		int growCount = m_objectList.size();
		for (int i = 0; i < growCount; i++)
		{
			m_objectList.push_back(Qnil);
			m_objectListIndexStack.push(growCount + i);
		}
	}
	
	int index = m_objectListIndexStack.top();
	m_objectListIndexStack.pop();
	m_objectList[index] = obj;
}

//------------------------------------------------------------------------------
void Manager::UnregisterWrapperObject(LNHandle handle)
{
	int index = (int)LNObject_GetUserData(handle);
	m_objectList[index] = Qnil;
	m_objectListIndexStack.push(index);
}

//------------------------------------------------------------------------------
const char* LNGetLastErrorMessage()
{
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	return LNInternal_ConvertToUTF8String(str, -1);
}

// �֐����� create_makefile �Ŏw�肵�����O�ƍ��킹�Ă����Ȃ���΂Ȃ�Ȃ�
extern "C" void Init_Lumino()
{
	// ���[�g���W���[��
	g_luminoModule = rb_define_module("Lumino");
	
    // ��O��`
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	Manager::Initialize();
}


