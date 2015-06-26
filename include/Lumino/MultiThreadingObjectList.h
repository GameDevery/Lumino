
#pragma once
#include <Lumino/Base/Array.h>

namespace Lumino
{

/**
	@brief		�����̃X���b�h����̗v�f�̒ǉ��E�폜��⏕���� RefObject �̔z��ł��B
	@details	�v�f�̒ǉ��E�폜�� AddObject() �� RemoveObject() �ōs���܂��B
				�ǉ��܂��͍폜����Ă������ɔz��{�̂��X�V����킯�ł͂���܂���B
				�z��{�̂��ʂ̃X���b�h�ŃC�e���[�g���ł��邱�Ƃɔ����A�ǉ��܂��͍폜�҂����X�g�ɒǉ�����܂��B
				Commit() ���Ă΂ꂽ���_�ő҂����X�g���g�p���A�z��{�̂��X�V���܂��B
*/
template<typename TRefObj>
class MultiThreadingRefObjectList
{
public:
	typedef Array<TRefObj>	ObjectArray;

private:
	ObjectArray	m_objectArray;		///< ���C���̃��X�g
	ObjectArray	m_registerList;		///< ���C���̃��X�g�ւ̒ǉ��҂����X�g
	ObjectArray	m_unregisterList;	///< ���C���̃��X�g����̍폜�҂����X�g

public:
	MultiThreadingRefObjectList() {}
	~MultiThreadingRefObjectList() { Clear(); }

public:

	/// �ǉ�
	void AddObject(TRefObj obj)
	{
		assert(obj);
		assert(std::find(m_registerList.begin(), m_registerList.end(), obj) == m_registerList.end());	// ���ɒǉ��v������Ă���
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) == m_objectArray.end());		// ���ݍ폜�҂��ł���
		m_registerList.Add(obj);
		obj->AddRef();
	}

	/// �폜
	void RemoveObject(TRefObj obj)
	{
		assert(obj);

		// �o�^���X�g�ɓ����Ă��炱�̎��_�ō폜���Ă��܂�
		typename ObjectArray::iterator itr = std::find(m_registerList.begin(), m_registerList.end(), obj);
		if (itr != m_registerList.end())
		{
			m_registerList.erase(itr);
			return;
		}

		assert(std::find(m_unregisterList.begin(), m_unregisterList.end(), obj) == m_unregisterList.end());
		assert(std::find(m_objectArray.begin(), m_objectArray.end(), obj) != m_objectArray.end());
		m_unregisterList.Add(obj);
		obj->AddRef();
	}

	/// ����
	void Commit()
	{
		typename ObjectArray::iterator itr, end;

		// �ǉ�
		if (!m_registerList.IsEmpty())
		{
			itr = m_registerList.begin();
			end = m_registerList.end();
			for (; itr != end; ++itr)
			{
				m_objectArray.Add(*itr);
				// �Q�ƃJ�E���g�� m_registerList ����O������ m_objectArray ��
				// �ǉ����镪�� �}0 �Ȃ̂ő���͂��Ȃ�
			}
			m_registerList.Clear();
		}

		// �폜
		if (!m_unregisterList.IsEmpty())
		{
			itr = m_unregisterList.begin();
			end = m_unregisterList.end();
			for (; itr != end; ++itr)
			{
				typename ObjectArray::iterator pos = std::find(m_objectArray.begin(), m_objectArray.end(), (*itr));
				if (pos != m_objectArray.end())
				{
					(*pos)->Release();	// m_unregisterList ����O����
					(*pos)->Release();	// m_registerList ����O����
					m_objectArray.erase(pos);
				}
			}
			m_unregisterList.Clear();
		}
	}

	/// ���̃��X�g���炵���Q�Ƃ���Ă��Ȃ��I�u�W�F�N�g (�Q�ƃJ�E���g�� 1 �ł���I�u�W�F�N�g) ����菜���܂��B
	void CollectGC()
	{
		typename ObjectArray::iterator itr = m_objectArray.begin();
		typename ObjectArray::iterator end = m_objectArray.end();
		for (; itr != end;)
		{
			if ((*itr)->GetRefCount() == 1)
			{
				(*itr)->Release();
				itr = m_objectArray.erase(itr);
				end = m_objectArray.end();
			}
			else {
				++itr;
			}
		}
	}

	/// �ǉ��\�񒆔z��擾
	ObjectArray& GetRegistOrderObjectArray() { return m_registerList; }

	/// �폜�\�񒆔z��擾
	ObjectArray& GetUnegistOrderObjectArray() { return m_unregisterList; }

	/// �z��擾
	ObjectArray& GetObjectArray() { return m_objectArray; }

	/// ���ׂăN���A (�I�������p�BClear() ���ĂԃX���b�h�ȊO�͒�~���Ă��邱��)
	void Clear()
	{
		typename ObjectArray::iterator itr, end;

		itr = m_registerList.begin();
		end = m_registerList.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_registerList.Clear();

		itr = m_unregisterList.begin();
		end = m_unregisterList.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_unregisterList.Clear();

		itr = m_objectArray.begin();
		end = m_objectArray.end();
		for (; itr != end; ++itr) (*itr)->Release();
		m_objectArray.Clear();
	}
};

} // namespace Lumino
