
#pragma once
#include "Common.h"
//#include "../Base/List.h"
//#include "../Base/String.h"
#include "../Base/Delegate.h"
#include "../Base/StlHelper.h"

LN_NAMESPACE_BEGIN
namespace tr
{

/**
	@brief		
*/
class ReflectionEventBase
{
protected:
	ReflectionEventBase() = default;
	virtual ~ReflectionEventBase() = default;

private:
	friend class Object;
	virtual void raise(ReflectionEventArgs* e) const = 0;
};

/**
	@brief		
	@details	
*/
template<class TArgs>
class ReflectionEvent
	: public ReflectionEventBase
{
public:
	ReflectionEvent() = default;
	virtual ~ReflectionEvent() = default;

public:

	/**
		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
	*/
	void addHandler(const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.push_back(handler);
	}
	
	/**
		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
	*/
	void removeHandler(const Delegate<void(TArgs*)>& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}
	
	/**
		@brief	���[�e�B���O�C�x���g�̃n���h����ǉ����܂��B
	*/
	//void operator += (const std::function<void(TArgs*)>& handler)
	//{
	//	addHandler(handler);
	//}

	void operator += (const Delegate<void(TArgs*)>& handler)
	{
		m_handlerList.push_back(handler);
	}
	
	/**
		@brief	�w�肵���n���h���Ɉ�v����n���h�����A���̃X���b�g����폜���܂��B
	*/
	void operator -= (const Delegate<void(TArgs*)>& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

private:
	std::vector< Delegate<void(TArgs*)> > m_handlerList;

	virtual void raise(ReflectionEventArgs* e) const override
	{
		for (const Delegate<void(TArgs*)>& d : m_handlerList)
		{
			d.call(static_cast<TArgs*>(e));
		}
	}
};





/**
	@brief		���[�e�B���O�t���[�����[�N�𔺂�Ȃ��C�x���g�B
*/
//template<typename>
//class DelegateEvent {};
template<typename... TArgs>
class DelegateEvent/*<void(TArgs...)>*/		// �߂�l�� void �Œ�B�n���h����1���o�^����Ă��Ȃ��Ƃ��͉���Ԃ��΂����̂�������Ȃ����߁B
{
public:
	typedef Delegate<void(TArgs...)> DelegateType;

	void addHandler(const DelegateType& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

	void addHandler(const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) �������ƈÖٕϊ����������R���p�C���G���[�ƂȂ��Ă������ߗp��
	{
		addHandler(DelegateType(handler));
	}

	void removeHandler(const DelegateType& handler)
	{
		ln::detail::StlHelper::remove(m_handlerList, handler);
	}

	void operator += (const DelegateType& handler)
	{
		addHandler(handler);
	}

	//void operator += (const std::function<void(TArgs...)>& handler)	// void operator += (const DelegateType& handler) �������ƈÖٕϊ����������R���p�C���G���[�ƂȂ��Ă������ߗp��
	//{
	//	addHandler(DelegateType(handler));
	//}

	void operator -= (const DelegateType& handler)
	{
		removeHandler(handler);
	}

private:
	friend class Object;

	std::vector<DelegateType> m_handlerList;

	void clear()
	{
		m_handlerList.clear();
	}

	bool isEmpty() const
	{
		return m_handlerList.empty();
	}

	void raise(TArgs... args)
	{
		int count = m_handlerList.size();
		if (count > 0)
		{
			for (int i = 0; i < count - 1; ++i)
			{
				m_handlerList[i].call(args...);
			}
			m_handlerList[count - 1].call(args...);	// �߂�l��߂��͍̂Ō��1�B(.NET �̓���)
		}
	}
};





} // namespace tr
LN_NAMESPACE_END
