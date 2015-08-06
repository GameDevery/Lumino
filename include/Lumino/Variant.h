
#pragma once
#include <iterator>
#include <type_traits>
#include <Lumino/Base/Delegate.h>
//#include "RoutedEvent.h"

namespace Lumino
{
class CoreObject;
class Variant;
class VariantList;
class Property;
class PropertyChangedEventArgs;
class RoutedEvent;
class RoutedEventHandler;
class RoutedEventArgs;

enum VariantType
{
	VariantType_Unknown = 0,
	VariantType_Bool,
	VariantType_Int,
	VariantType_Float,
	VariantType_String,
	VariantType_List,
	VariantType_Object,

	VariantType_SizeF,
	VariantType_Rect,

	VariantType_Max,			///< (Terminator)
};

class TypeInfo
{
public:
	typedef uint32_t* (*HasLocalValueFlagsGetter)(CoreObject* _this);

public:
	TypeInfo();

	TypeInfo(const TCHAR* fullName, TypeInfo* baseClass, HasLocalValueFlagsGetter getter)
		: m_fullName(fullName)
		, m_baseClass(baseClass)
		, m_hasLocalValueFlagsGetter(getter)
	{ }

	void RegisterProperty(Property* prop);
	Property* FindProperty(const String& name) const;

	void RegisterRoutedEvent(RoutedEvent* ev);
	RoutedEvent* FindRoutedEvent(const String& name) const;	// TODO: ����Ȃ�����
	void InvokeRoutedEvent(CoreObject* owner, const RoutedEvent* ev, RoutedEventArgs* e);

	/// RoutedEventHandler �́A���[�U�[�����I�ɒǉ��ł���n���h�������O�ɌĂяo�����B
	/// WPF �ł́u�ÓI�n���h���v�ƌĂ΂�Ă���B���I�C�x���g�ɓo�^����̂ɔ�ׁA���������g�p���Ȃ��B
	void RegisterRoutedEventHandler(const RoutedEvent* ev, RoutedEventHandler* handler);
	RoutedEventHandler* FindRoutedEventHandler(const RoutedEvent* ev) const;

	TypeInfo* GetBaseClass() const { return m_baseClass; }
	uint32_t* GetHasLocalValueFlags(CoreObject* obj) { return m_hasLocalValueFlagsGetter(obj); }

	bool operator == (const TypeInfo& info) const { return m_fullName == info.m_fullName; }
	bool operator < (const TypeInfo& info) const { return m_fullName < info.m_fullName; }

protected:

private:
	typedef SortedArray<const RoutedEvent*, RoutedEventHandler*>	RoutedEventHandlerList;

	String					m_fullName;		///< ���S�C����
	TypeInfo*				m_baseClass;
	Array<Property*>		m_propertyList;
	Array<RoutedEvent*>		m_routedEventList;
	RoutedEventHandlerList	m_routedEventHandlerList;
	HasLocalValueFlagsGetter	m_hasLocalValueFlagsGetter;
};

/**
	@brief		
*/
class CoreObject
	: public RefObject
{
public:
	static const int IsArrayObject = 0;
public:
	static const RoutedEvent*	PropertyChangedEvent;

public:
	CoreObject();
	virtual ~CoreObject();

public:

	/**
		@brief		�v���p�e�B�̒l��ݒ肵�܂��B
	*/
	void SetPropertyValue(const Property* prop, const Variant& value);
	
	/**
		@brief		�v���p�e�B�̒l���擾���܂��B
	*/
	Variant GetPropertyValue(const Property* prop) const;

	/**
		@brief		�v���p�e�B�̒l��ݒ肵�܂��B���炩���ߌ^���������Ă���ꍇ�ASetPropertyValue() �������Ȃ��I�[�o�[�w�b�h�Őݒ�ł��܂��B
	*/
	template<typename TValue>
	void SetTypedPropertyValue(const Property* prop, const TValue& value);

	/**
		@brief		�v���p�e�B�̒l���擾���܂��B���炩���ߌ^���������Ă���ꍇ�AGetPropertyValue() �������Ȃ��I�[�o�[�w�b�h�Őݒ�ł��܂��B
	*/
	template<typename TValue>
	TValue GetTypedPropertyValue(const Property* prop) const;


	String ToString();

	virtual void SetUserData(void* data) { m_userData = data; }

	virtual void* GetUserData() const { return m_userData; }

	/// �e�팾��o�C���_����ݒ肳���^��� ID ���擾���܂��B
	virtual void* GetBindingTypeData() const { return NULL; }

	/**
		@brief	���̃I�u�W�F�N�g�Ɏw�肵���v���p�e�B�̃��[�J���l���ݒ肳��Ă��邩���m�F���܂��B
	*/
	bool HasLocalPropertyValue(const Property* prop);


	Event01<PropertyChangedEventArgs*>	PropertyChanged;

	// internal
	void NotifyPropertyChange(const Property* prop, const Variant& newValue, const Variant& oldValue/*PropertyChangedEventArgs* e*/);

protected:
	// �o�^����Ă���n���h���ƁA(Bubble�̏ꍇ)�_����̐e�փC�x���g��ʒm����
	virtual void RaiseEventInternal(const RoutedEvent* ev, RoutedEventArgs* e);

	virtual void OnPropertyChanged(PropertyChangedEventArgs* e);
	//bool HasLocalValueInternal(const Property* prop);

	class LocalValueFlags
	{
	public:
		uint32_t	Flags;
		LocalValueFlags()
			: Flags(0)
		{}
	};

private:
	friend TypeInfo* GetTypeInfo(CoreObject* obj);
	//virtual TypeInfo* GetThisTypeInfo() const { return NULL; };	// TODO: �������z�֐��ɂ��ă}�N����`����������
	static uint32_t* GetHasLocalValueFlags(CoreObject* _this) { return NULL; }

private:
	typedef SortedArray<String, Property*>	PropertyList;
	typedef SortedArray<const Property*, Variant>	PropertyDataStore;

	void*				m_userData;
	//PropertyList		m_propertyList;
	PropertyDataStore*	m_propertyDataStore;

	//typedef SortedArray<const RoutedEvent*, Property*>	RoutedEventList;

private:
	static TypeInfo m_typeInfo;
	virtual TypeInfo* GetThisTypeInfo() const;
public:
	static TypeInfo* GetClassTypeInfo();
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<typename TValue>
void CoreObject::SetTypedPropertyValue(const Property* prop, const TValue& value)
{
	LN_THROW(prop != NULL, ArgumentException);
	auto t = static_cast<const TypedProperty<TValue>*>(prop);
	t->SetValueDirect(this, value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<typename TValue>
TValue CoreObject::GetTypedPropertyValue(const Property* prop) const
{
	LN_THROW(prop != NULL, ArgumentException);
	auto t = static_cast<const TypedProperty<TValue>*>(prop);
	return t->GetValueDirect(this);
}


#define LN_CORE_OBJECT_TYPE_INFO_DECL() \
	private: \
		static TypeInfo m_typeInfo; \
		static void* m_coreObjectBindingTypeData; \
		virtual TypeInfo* GetThisTypeInfo() const; \
		LocalValueFlags m_hasLocalValueFlags; \
		static uint32_t* GetHasLocalValueFlags(CoreObject* _this); \
	public: \
		static TypeInfo* GetClassTypeInfo(); \
		virtual void* GetBindingTypeData() const { return m_coreObjectBindingTypeData; } \
		static void SetBindingTypeData(void* data) { m_coreObjectBindingTypeData = data; }

#define LN_CORE_OBJECT_TYPE_INFO_IMPL(subClassFullName, baseClass) \
	TypeInfo subClassFullName::m_typeInfo(_T(#subClassFullName), baseClass::GetClassTypeInfo(), &subClassFullName::GetHasLocalValueFlags); \
	void* subClassFullName::m_coreObjectBindingTypeData = NULL; \
	TypeInfo* subClassFullName::GetThisTypeInfo() const { return &m_typeInfo; } \
	TypeInfo* subClassFullName::GetClassTypeInfo() { return &m_typeInfo; } \
	uint32_t* subClassFullName::GetHasLocalValueFlags(CoreObject* _this) { return &static_cast<subClassFullName*>(_this)->m_hasLocalValueFlags.Flags; }



/// �w�肳�ꂽ�I�u�W�F�N�g�̌^�����擾����
inline TypeInfo* GetTypeInfo(CoreObject* obj)
{
	return obj->GetThisTypeInfo();
}

/**
	@brief		
*/
class Variant
{
public:
	static const Variant Null;

public:
	Variant();
	Variant(const Variant& value);
	Variant(bool value);
	Variant(int value);
	Variant(float value);
	Variant(const String& value);
	Variant(const Enum& value);
	Variant(VariantList* value);
	Variant(const SizeF& value);
	Variant(const Rect& value);
	~Variant();
	Variant& operator = (const Variant& obj) { Copy(obj); return (*this); }

	Variant(CoreObject* obj);

	template<class T>
	Variant(RefPtr<T>& obj)
		: m_type(VariantType_Unknown)
		, m_uint(0)
	{
		if (T::IsArrayObject == 0) 
			Set(obj);
		else
		{
			CoreObject* t = obj.GetObjectPtr();
			SetList(static_cast<VariantList*>(t));
		}
	}

public:
	VariantType GetType() const { return m_type; }
	bool GetBool() const;

	void SetInt(int value);
	int GetInt() const;

	void SetFloat(float value);
	float GetFloat() const;

	void SetString(const String& value);
	String GetString() const;

	void SetList(VariantList* value);
	VariantList* GetList() const;

	void Set(CoreObject* obj);
	CoreObject* GetObject() const;

	void SetSizeF(const SizeF& value);
	const SizeF& GetSizeF() const;

	void SetRect(const Rect& value);
	const Rect& GetRect() const;


	template<typename T>
	T& RefCast() const { return static_cast<T&>(GetObject()); }


public:
	
	/**
		@brief		�w�肵�� Variant �̒l���w�肵���^�ɃL���X�g����B
		@code
					UIElement* item = Variant::Cast<UIElement*>(value);
		@endcode
	*/
	template<typename T>
	static T Cast(const Variant& value) { return CastSelector<T, typename std::is_base_of<Enum, T>::type >::GetValue(value); }

private:
	template<typename T, typename TIsEnum> struct CastSelector { static T GetValue(const Variant& v) { return static_cast<T>(v.GetObject()); } };
	template<> struct CastSelector < bool, std::false_type > { static bool GetValue(const Variant& v) { return v.GetBool(); } };
	template<> struct CastSelector < int, std::false_type > { static int GetValue(const Variant& v) { return v.GetInt(); } };
	template<> struct CastSelector < float, std::false_type > { static float GetValue(const Variant& v) { return v.GetFloat(); } };
	template<> struct CastSelector < String, std::false_type > { static String GetValue(const Variant& v) { return v.GetString(); } };
	template<> struct CastSelector < Rect, std::false_type > { static Rect GetValue(const Variant& v) { return v.GetRect(); } };
	template<> struct CastSelector < SizeF, std::false_type > { static SizeF GetValue(const Variant& v) { return v.GetSizeF(); } };
	template<typename T> struct CastSelector < T, std::true_type > { static T GetValue(const Variant& v) { return *((T*)(&v.m_enum)); } };	// TODO: �^�`�F�b�N

public:
	bool operator == (const Variant& right) const;

private:
	void Copy(const Variant& obj);
	void Release();

private:
	VariantType	m_type;
	union
	{
		uint64_t		m_uint;
		bool			m_bool;
		int				m_int;
		float			m_float;
		/*String*			m_string;*/
		VariantList*	m_valueList;
		CoreObject*		m_object;
		float			m_sizeF[2];
		int				m_rect[4];
		Enum			m_enum;
	};
	String			m_string;
};

/**
	@brief
*/
class VariantList
	: public CoreObject
{
public:
	static const int IsArrayObject = 1;

public:
	VariantList() {}
	virtual ~VariantList() { Clear(); }

public:

	bool IsOutOfRange(int index) const { return m_list.IsOutOfRange(index); }
	bool IsEmpty() const { return m_list.IsEmpty(); }

	/// �v�f�����擾����
	int GetCount() const { return m_list.GetCount(); }

	/// �w��C���f�b�N�X�ɗv�f���i�[����
	void SetAtVariant(int index, const Variant& item)
	{
		if (OnItemAdding(item))
		{
			m_list.SetAt(index, item);
			OnItemAdded(item);
		}
	}

	/// �w��C���f�b�N�X�̗v�f���擾����
	const Variant& GetAtVariant(int index) const
	{
		return m_list.GetAt(index);
	}

	/// �v�f�𖖔��ɒǉ�����
	void AddVariant(const Variant& item)
	{
		if (OnItemAdding(item))
		{
			m_list.Add(item);
			OnItemAdded(item);
		}
	}

	/// �S�Ă̗v�f���폜����
	void Clear()
	{
		for (Variant& item : m_list) {
			OnItemRemoved(item);	// TODO: erase ���Ȃ���ЂƂ��ĂԂׂ�����
		}
		m_list.Clear();
	}

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	void InsertVariant(int index, const Variant& item)
	{
		if (OnItemAdding(item))
		{
			m_list.Insert(index, item);
			OnItemAdded(item);
		}
	}

	/// item �ƈ�v����ŏ��̗v�f���폜����
	void RemoveVariant(const Variant& item)
	{
		bool b = m_list.Remove(item);
		if (b) {
			OnItemRemoved(item);
		}
	}

	/// �w�肵���C���f�b�N�X�̗v�f���폜����
	void RemoveAt(int index)
	{
		Variant item = m_list.GetAt(index);
		m_list.RemoveAt(index);
		OnItemRemoved(item);
	}

public:
	class const_iterator : public std::iterator<std::forward_iterator_tag, const Variant>
	{
	public:
		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }
		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class VariantList;
		const_iterator(const Array<Variant>::const_iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::const_iterator m_internalItr;
	};

	class iterator : public std::iterator<std::forward_iterator_tag, Variant>
	{
	public:
		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return static_cast<reference>(*m_internalItr); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }
		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const iterator& right) const		{ return m_internalItr != right.m_internalItr; }
		bool operator<(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class VariantList;
		iterator(const Array<Variant>::iterator& itr) : m_internalItr(itr) {}	// begin() �� end() �Ŏg�p����
		Array<Variant>::iterator m_internalItr;
	};

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }

protected:
	virtual bool OnItemAdding(const Variant& item) { return true; }
	virtual void OnItemAdded(const Variant& item) {}
	virtual void OnItemRemoved(const Variant& item) {}

protected:
	Array<Variant>	m_list;
};

#define LN_VARIANT_OBJECT_LIST_DECL(itemType) \
	void SetAt(int index, itemType* item); \
	itemType* GetAt(int index) const;

#define LN__VARIANT_OBJECT_LIST_IMPL(listType, itemType) \
	void listType::SetAt(int index, itemType* item) { RefObjectList::SetAtBase(index, item); } \
	itemType* listType::GetAt(int index) const { return static_cast<itemType*>(RefObjectList::GetAtBase(index)); }


template<typename T>
struct TestTraits
{
	typedef T& DirectReference;
};

template<typename T>
struct TestTraits<T*>
{
	typedef T* DirectReference;
};

/**
	@brief
*/
template<typename TValue>
class GenericVariantList
	: public VariantList
{
public:
	typedef TValue value_type;
	typedef ptrdiff_t difference_type;
	typedef ptrdiff_t distance_type;	// retained
	//typedef typename std::vector<TValue>::pointer pointer;
	//typedef typename std::vector<TValue>::reference reference;
	typedef TValue* pointer;
	typedef TValue& reference;


public:
	/// �w��C���f�b�N�X�ɗv�f���i�[����
	void SetAt(int index, const TValue& item) { SetAtVariant(index, item); }

	/// �w��C���f�b�N�X�̗v�f���擾����
	TValue GetAt(int index) const { return Variant::Cast<TValue>(GetAtVariant(index)); }	// TODO: �ł���ΎQ�ƂŕԂ��������c

	/// �v�f�𖖔��ɒǉ�����
	void Add(const TValue& item) { AddVariant(item); }

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	void Insert(int index, const TValue& item) { InsertVariant(index, item); }

public:
	class const_iterator //: public std::iterator<std::forward_iterator_tag, const TValue>
	{
	public:
		const_iterator() : m_internalItr() {}
		const_iterator(const const_iterator& obj) : m_internalItr(obj.m_internalItr) {}
		const_iterator& operator = (const const_iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		reference operator*() const		{ return *m_internalItr; }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		const_iterator& operator++()	{ ++m_internalItr; return (*this); }
		const_iterator operator++(int)	{ const_iterator tmp = *this; ++(*this); return tmp; }
		const_iterator& operator--()	{ --m_internalItr; return (*this); }
		const_iterator operator--(int)	{ const_iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr == right.m_internalItr; }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr != right.m_internalItr; }
		bool operator<(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		const_iterator(const Array<Variant>::const_iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::const_iterator m_internalItr;
	};

	class iterator// : public std::iterator<std::forward_iterator_tag, TValue>
	{
	public:
		iterator() : m_internalItr() {}
		iterator(const iterator& obj) : m_internalItr(obj.m_internalItr) {}
		iterator& operator = (const iterator& obj) { m_internalItr = obj.m_internalItr; return (*this); }
		typename TestTraits<TValue>::DirectReference operator*() const		{ return Variant::Cast<TValue>(*m_internalItr); }//{ return *static_cast<TValue**>(&(m_internalItr->Cast<TValue>())); }
		pointer operator->() const		{ LN_THROW(0, NotImplementedException); return NULL; }
		iterator& operator++()			{ ++m_internalItr; return (*this); }
		iterator operator++(int)		{ iterator tmp = *this; ++(*this); return tmp; }
		iterator& operator--()			{ --m_internalItr; return (*this); }
		iterator operator--(int)		{ iterator tmp = *this; --(*this); return tmp; }
		//const_iterator& operator+=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator+(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//const_iterator& operator-=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		//const_iterator operator-(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		//difference_type operator-(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr.operator==(right.m_internalItr); }
		bool operator!=(const iterator& right) const		{ return m_internalItr.operator!=(right.m_internalItr); }
		bool operator<(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		iterator(const Array<Variant>::iterator& itr) : m_internalItr(itr) {}
		Array<Variant>::iterator m_internalItr;
	};

public:
	GenericVariantList() {}
	virtual ~GenericVariantList() {}

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }
};



} // namespace Lumino
