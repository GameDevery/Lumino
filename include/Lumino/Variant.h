
#pragma once
#include <Lumino/Base/Delegate.h>

namespace Lumino
{
class Variant;
class VariantList;
class Property;
class PropertyChangedEventArgs;

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
	TypeInfo() {}

	TypeInfo(const TCHAR* fullName, TypeInfo* baseClass)
		: m_fullName(fullName)
		, m_baseClass(baseClass)
	{ }

	void RegisterProperty(Property* prop);
	Property* FindProperty(const String& name) const;

	bool operator == (const TypeInfo& info) const { return m_fullName == info.m_fullName; }
	bool operator < (const TypeInfo& info) const { return m_fullName < info.m_fullName; }

private:
	String				m_fullName;		///< ���S�C����
	TypeInfo*			m_baseClass;
	Array<Property*>	m_propertyList;
};

/**
	@brief		
*/
class CoreObject
	: public RefObject
{
public:

public:
	CoreObject();
	virtual ~CoreObject();

public:


	/// �v���p�e�B�̒l��ݒ肵�܂��B
	//virtual void SetPropertyValue(const String& propertyName, const Variant& value);
	virtual void SetPropertyValue(const Property* prop, const Variant& value);

	/// �v���p�e�B�̒l���擾���܂��B
	//virtual Variant GetPropertyValue(const String& propertyName) const;
	virtual Variant GetPropertyValue(const Property* prop) const;

	String ToString();

	virtual void SetUserData(void* data) { m_userData = data; }

	virtual void* GetUserData() const { return m_userData; }

	/// �e�팾��o�C���_����ݒ肳���^��� ID ���擾���܂��B
	virtual void* GetBindingTypeData() const { return NULL; }

	//const PropertyList& GetPropertyList() const { return m_propertyList; }

	//Property* FindProperty(const String& name) const
	//{
	//	Property* prop;
	//	if (m_propertyList.TryGetValue(name, &prop)) {
	//		return prop;
	//	}
	//	return NULL;
	//}

	Event02<CoreObject*, PropertyChangedEventArgs*>	PropertyChanged;

protected:
	/// ���� CoreObject �Ƀv���p�e�B��o�^���܂��B
	//void RegisterProperty(const String& propertyName, const Variant& defaultValue);
	//void RegisterProperty(Property* prop);

	void OnPropertyChanged(const String& name, const Variant& newValue);

private:
	friend TypeInfo* GetTypeInfo(CoreObject* obj);
	//virtual TypeInfo* GetThisTypeInfo() const { return NULL; };	// TODO: �������z�֐��ɂ��ă}�N����`����������

private:
	typedef SortedArray<String, Property*>	PropertyList;
	typedef SortedArray<const Property*, Variant>	PropertyDataStore;

	void*				m_userData;
	//PropertyList		m_propertyList;
	PropertyDataStore*	m_propertyDataStore;


private:
	static TypeInfo m_typeInfo;
	virtual TypeInfo* GetThisTypeInfo() const;
public:
	static TypeInfo* GetClassTypeInfo();
};

#define LN_CORE_OBJECT_TYPE_INFO_DECL() \
	private: \
		static TypeInfo m_typeInfo; \
		static void* m_coreObjectBindingTypeData; \
		virtual TypeInfo* GetThisTypeInfo() const; \
	public: \
		static TypeInfo* GetClassTypeInfo(); \
		virtual void* GetBindingTypeData() const { return m_coreObjectBindingTypeData; } \
		static void SetBindingTypeData(void* data) { m_coreObjectBindingTypeData = data; }

#define LN_CORE_OBJECT_TYPE_INFO_IMPL(subClassFullName, baseClass) \
	TypeInfo subClassFullName::m_typeInfo(_T(#subClassFullName), baseClass::GetClassTypeInfo()); \
	void* subClassFullName::m_coreObjectBindingTypeData = NULL; \
	TypeInfo* subClassFullName::GetThisTypeInfo() const { return &m_typeInfo; } \
	TypeInfo* subClassFullName::GetClassTypeInfo() { return &m_typeInfo; }

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
	Variant(const Enum& value);
	Variant(const SizeF& value);
	Variant(const Rect& value);
	~Variant();
	Variant& operator = (const Variant& obj) { Copy(obj); return (*this); }

	Variant(CoreObject* obj);

	template<class T>
	Variant(const RefPtr<T>& obj)
		: m_type(VariantType_Unknown)
		, m_uint(0)
	{
		Set(obj);
	}

public:
	VariantType GetType() const { return m_type; }
	bool GetBool() const;

	void SetInt(int value);
	int GetInt() const;

	void SetFloat(float value);
	float GetFloat() const;

	void Set(CoreObject* obj);
	CoreObject* GetObject() const;

	void SetSizeF(const SizeF& value);
	const SizeF& GetSizeF() const;

	void SetRect(const Rect& value);
	const Rect& GetRect() const;


	//template <class T>
	//struct TypeTrait { typedef void other; };

	

	template<typename T>
	T Cast() const { return static_cast<T>(GetObject()); }

	template<> SizeF Cast() const { return GetSizeF(); }
	template<> bool Cast() const { return GetBool(); }
	template<> int Cast() const { return GetInt(); }
	template<> float Cast() const { return GetFloat(); }
	template<> CoreObject* Cast() const { return GetObject(); }
	template<> Rect Cast() const { return GetRect(); }
	//template<> Enum Cast() const { return GetInt(); }

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
	VariantList() {}
	virtual ~VariantList() { Clear(); }

public:

	/// �v�f�����擾����
	int GetCount() const { return m_list.GetCount(); }

	/// �w��C���f�b�N�X�ɗv�f���i�[����
	void SetAtBase(int index, CoreObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.SetAt(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// �w��C���f�b�N�X�̗v�f���擾����
	CoreObject* GetAtBase(int index) const
	{
		return m_list.GetAt(index);
	}

	/// �v�f�𖖔��ɒǉ�����
	void Add(CoreObject* item)
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
		LN_FOREACH(CoreObject* item, m_list) {
			OnItemRemoved(item);	// TODO: erase ���Ȃ���ЂƂ��ĂԂׂ�����
			LN_SAFE_RELEASE(item);
		}
		m_list.Clear();
	}

	/// �w�肵���C���f�b�N�X�̈ʒu�ɗv�f��}������
	void Insert(int index, CoreObject* item)
	{
		if (OnItemAdding(item))
		{
			m_list.Insert(index, item);
			LN_SAFE_ADDREF(item);
			OnItemAdded(item);
		}
	}

	/// item �ƈ�v����ŏ��̗v�f���폜����
	void Remove(CoreObject* item)
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
		CoreObject* item = m_list.GetAt(index);
		m_list.RemoveAt(index);
		OnItemRemoved(item);
		LN_SAFE_RELEASE(item);
	}

protected:
	virtual bool OnItemAdding(CoreObject* item) { return true; }
	virtual void OnItemAdded(CoreObject* item) {}
	virtual void OnItemRemoved(CoreObject* item) {}

protected:
	Array<CoreObject*>	m_list;
};

#define LN_VARIANT_OBJECT_LIST_DECL(itemType) \
	void SetAt(int index, itemType* item); \
	itemType* GetAt(int index) const;

#define LN__VARIANT_OBJECT_LIST_IMPL(listType, itemType) \
	void listType::SetAt(int index, itemType* item) { RefObjectList::SetAtBase(index, item); } \
	itemType* listType::GetAt(int index) const { return static_cast<itemType*>(RefObjectList::GetAtBase(index)); }

/**
	@brief
*/
template<class TRefObj>
class GenericVariantList
	: public VariantList
{
public:
	class const_iterator
	{
	public:
		typedef Array<RefObject*>				internal_list;
		typedef TRefObj*						value_type;
		typedef internal_list::difference_type	difference_type;
		typedef const value_type*				pointer;
		typedef const value_type				reference;

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
		const_iterator& operator+=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator+(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		const_iterator& operator-=(difference_type offset)				{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator-(difference_type offset) const			{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		difference_type operator-(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return m_internalItr[offset]; }
		bool operator==(const const_iterator& right) const	{ return m_internalItr.operator==(right.m_internalItr); }
		bool operator!=(const const_iterator& right) const	{ return m_internalItr.operator!=(right.m_internalItr); }
		bool operator<(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const const_iterator& right) const	{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		const_iterator(const internal_list::const_iterator& itr) : m_internalItr(itr) {}

		internal_list::const_iterator m_internalItr;
	};

	class iterator
	{
	public:
		typedef Array<RefObject*>				internal_list;
		typedef TRefObj*						value_type;
		typedef internal_list::difference_type	difference_type;
		typedef value_type*						pointer;
		typedef value_type						reference;

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
		const_iterator& operator+=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator+(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		const_iterator& operator-=(difference_type offset)			{ LN_THROW(0, NotImplementedException); return (*this); }
		const_iterator operator-(difference_type offset) const		{ LN_THROW(0, NotImplementedException); return const_iterator(); }
		difference_type operator-(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return 0; }

		reference operator[](difference_type offset) const	{ return static_cast<reference>(m_internalItr[offset]); }
		bool operator==(const iterator& right) const		{ return m_internalItr.operator==(right.m_internalItr); }
		bool operator!=(const iterator& right) const		{ return m_internalItr.operator!=(right.m_internalItr); }
		bool operator<(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>(const iterator& right) const			{ LN_THROW(0, NotImplementedException); return false; }
		bool operator<=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }
		bool operator>=(const iterator& right) const		{ LN_THROW(0, NotImplementedException); return false; }

	private:
		friend class GenericVariantList;
		iterator(const internal_list::iterator& itr) : m_internalItr(itr) {}

		internal_list::iterator m_internalItr;
	};

public:
	GenericVariantList() {}
	virtual ~GenericVariantList() {}

	void SetAt(int index, TRefObj* item) { RefObjectList::SetAtBase(index, item); }
	TRefObj* GetAt(int index) const { return static_cast<TRefObj*>(RefObjectList::GetAtBase(index)); }

	iterator		begin()			{ return iterator(m_list.begin()); }
	const_iterator	begin() const	{ return const_iterator(m_list.begin()); }
	iterator		end()			{ return iterator(m_list.end()); }
	const_iterator	end() const		{ return const_iterator(m_list.end()); }
};


class PropertyChangedEventArgs
{
public:
	String	PropertyName;
	Variant	NewValue;
};

} // namespace Lumino
