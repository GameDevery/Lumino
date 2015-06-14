
#pragma once
#include "Variant.h"

namespace Lumino
{

/**
	@brief		
*/
template<class TObject>
class CoreObjectList
	: public CoreObject
{
public:
	typedef typename ArrayList<TObject*>::iterator			iterator;
	typedef typename ArrayList<TObject*>::const_iterator	const_iterator;
	typedef typename ArrayList<TObject*>::reference			reference;
	typedef typename ArrayList<TObject*>::const_reference	const_reference;

public:
	CoreObjectList() {}
	virtual ~CoreObjectList() { Clear(); }

public:

	/// �v�f�����擾����
	int GetCount() const { return m_list.GetCount(); }

	/// �w��C���f�b�N�X�ɗv�f���i�[����
	void SetAtBase(int index, TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.SetAt(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// �w��C���f�b�N�X�̗v�f���擾����
	TObject* GetAtBase(int index) const
	{
		return m_list.GetAt(index);
	}

	/// �v�f�𖖔��ɒǉ�����
	void Add(TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.Add(item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// �S�Ă̗v�f���폜����
	void Clear()
	{
		LN_FOREACH(TObject* item, m_list) {
			OnItemRemoved(item);	// TODO: erase ���Ȃ���ЂƂ��ĂԂׂ�����
			LN_SAFE_RELEASE(item);
		}
		m_list.Clear();
	}

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	void Insert(int index, TObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.Insert(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// item �ƈ�v����ŏ��̗v�f���폜����
	void Remove(TObject* item)
	{
		bool b = m_list.Remove(item);
		if (b) {
			OnItemRemoved(item);
			LN_SAFE_RELEASE(item);
		}
	}

	/// �w�肵���C���f�b�N�X�̗v�f���폜����
	void RemoveAt(int index)
	{
		TObject* item = m_list.GetAt(index);
		m_list.RemoveAt(index);
		OnItemRemoved(item);
		LN_SAFE_RELEASE(item);
	}

public:
	// STL interface
	iterator		begin()			{ return m_list.begin(); }
	const_iterator	begin() const	{ return m_list.begin(); }
	iterator		end()			{ return m_list.end(); }
	const_iterator	end() const		{ return m_list.end(); }

protected:
	virtual bool OnItemAdding(TObject* item) { return true; }
	virtual void OnItemAdded(TObject* item) {}
	virtual void OnItemRemoved(TObject* item) {}

protected:
	ArrayList<TObject*>	m_list;
};

} // namespace Lumino
