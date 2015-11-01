
#pragma once
#include <vector>
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN

/**
	@brief		���� Release ����I�u�W�F�N�g���Ǘ�����v�[���B
*/
class AutoReleasePool
{
public:

	/**
		@brief		�J�����g�� AutoRseleasePool ���擾���܂��B
	*/
	static AutoReleasePool* GetCurrent();

	/**
		@brief		���� AutoRseleasePool �ɃI�u�W�F�N�g��ǉ����܂��B
	*/
	void AddObject(RefObject* obj);

	/**
		@brief		�ǉ�����Ă���I�u�W�F�N�g�� Release() ���Ăяo���A�v�[�����N���A���܂��B
	*/
	void Clear();

	/**
		@brief		�J�����g�� AutoRseleasePool �ɃI�u�W�F�N�g��ǉ����郆�[�e�B���e�B�ł��B
	*/
	static void AddObjectToCurrent(RefObject* obj);

private:
	AutoReleasePool();
	~AutoReleasePool();

	std::vector<RefObject*>	m_objectList;
};

LN_NAMESPACE_END
