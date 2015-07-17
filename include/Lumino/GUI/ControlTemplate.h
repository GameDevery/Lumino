
#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

class Binding
{
public:
	Binding() : m_path() {}
	Binding(const String& srcPropPath) : m_path(srcPropPath) {}
	~Binding() {}

private:
	String	m_path;
};

/**
	@brief		
	@note	WPF �� FrameworkElementFactory �ɑ�������B
*/
class UIElementFactory	// TODO: UIElement �ȊO������悤�ɁB���O�ύX�B
	: public CoreObject
{
public:
	UIElementFactory(GUIManager* manager);
	virtual ~UIElementFactory();

	/// ���� UIElementFactory ����������C���X�^���X�̌^��
	void SetTypeName(const String& typeFullName) { m_targetTypeFullName = typeFullName; }

	void SetPropertyValue(const Property* prop, const Variant& value) { m_propertyValueList.SetValue(prop, value); }
	Variant GetPropertyValue(const Property* prop) const { return m_propertyValueList.GetValue(prop); }



	//void AddProperty(const String& propName, );
	//void AddTemplateBinding(const String& propName, );
	//void AddTemplateBinding(const String& propName, const Binding& binding)
	//{
	//}
	void AddTemplateBinding(const Property* prop, const String& srcPropPath)
	{
		PropertyInfo info;
		info.Kind = PropertyKind_TemplateBinding;
		info.SourcePropPath = srcPropPath;
		m_propertyInfoList.Add(prop, info);
	}


	void AddChild(UIElementFactory* child);


	// �ȉ�2�͂��Ƃ���1���������A�q�v�f���r���h����O�ɐe�v�f�ɒǉ����A���[�J�����\�[�X���X�V����K�v�����������ߕ������B
	// ���ꂾ���Ȃ番����K�v�������C�����邪�A�e�v�f�ɂ���Ēǉ��悪�قȂ�B
	// ���[�g�Ȃ� SetTemplateChild()�A�v���p�e�B�Ȃ� SetValue()�A���X�g�Ȃ� AddItem() ���A�F�X����B
	//CoreObject* CreateInstance();
	//void BuildInstance(CoreObject* element, UIElement* rootLogicalParent);

	CoreObject* CreateInstance(UIElement* rootLogicalParent);


private:
	enum PropertyKind
	{
		PropertyKind_Variant = 0,		///< ���ʂ̒l
		PropertyKind_Binding,			///< Binding
		PropertyKind_TemplateBinding,	///< TemplateBinding
	};

	struct PropertyInfo
	{
		PropertyKind	Kind;
		//Binding			BindingInfo;	///< PropertyKind_Binding �̂Ƃ��͂�����g��
		String			SourcePropPath;		///< PropertyKind_TemplateBinding �̂Ƃ��͂�����g��
	};

	typedef SortedArray<const Property*, PropertyInfo>	PropertyInfoList;
	typedef SortedArray<const Property*, Variant>	PropertyValueList;

	GUIManager*						m_manager;	// CreateInstance() �ŕK�v�B�����ł�����Ă����������H
	String							m_targetTypeFullName;
	PropertyInfoList				m_propertyInfoList;
	PropertyValueList			m_propertyValueList;
	Array<UIElementFactory*>		m_children;
};

typedef GenericVariantList<UIElementFactory*>	UIElementFactorylist;

/**
	@brief		
	@note	ContentPresenter ���K�v�B
*/
class ControlTemplate
	: public CoreObject
{
public:
	ControlTemplate();
	virtual ~ControlTemplate();

	void SetTargetType(const String& fullTypeName) { m_targetType = fullTypeName; }
	const String&  GetTargetType() const { return m_targetType; }

	void SetPropertyValue(const Property* prop, const Variant& value) { m_propertyValueList.SetValue(prop, value); }
	Variant GetPropertyValue(const Property* prop) const { return m_propertyValueList.GetValue(prop); }
	//void SetPropertyValue(const String& propertyName, const Variant& value) { m_propertyValueList.SetValue(propertyName, value); }
	//Variant GetPropertyValue(const String& propertyName) const { return m_propertyValueList.GetValue(propertyName); }

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// �w�肳�ꂽ Control �ɂ��̃e���v���[�g��K�p���܂��B
	void Apply(Control* control);

private:
	typedef SortedArray<const Property*, Variant>	PropertyValueList;

	String						m_targetType;		///< �ΏۃR���g���[���� ("Button" ��)
	PropertyValueList			m_propertyValueList;
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< �e���v���[�g�� VisualTree �̃��[�g�m�[�h
};


/**
	@brief
*/
class DataTemplate
	: public CoreObject
{
public:
	DataTemplate();
	virtual ~DataTemplate();

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// �w�肳�ꂽ Control �ɂ��̃e���v���[�g��K�p���܂��B
	void Apply(Control* control);

private:
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< �e���v���[�g�� VisualTree �̃��[�g�m�[�h

	// WPF �ł̓f�[�^�̌^�Ƃ������B�Ƃ肠���� ControlTemplate �Ƃ͕����Ă����B
};

/**
	@brief		
	@note	
*/
//class LayoutTemplate
//	: public CoreObject
//{
//public:
//	ControlTemplate();
//	virtual ~ControlTemplate();
//
//private:
//	String				m_targetType;		///< �ΏۃR���g���[���� ("Button" ��)
//	UIElementFactory*	m_visualTreeRoot;	///< �e���v���[�g�� VisualTree �̃��[�g�m�[�h
//};


/**
	@brief		
*/
class Setter
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Setter();
	Setter(const Property* prop, const Variant& value);
	virtual ~Setter();

public:
	void SetProperty(const Property* prop) { m_property = prop; }
	const Property* GetProperty() const { return m_property; }
	void SetValue(const Variant& value) { m_value = value; }
	const Variant& GetValue() const { return m_value; }

private:
	const Property*		m_property;
	Variant				m_value;
};

typedef GenericVariantList<Setter*>		SetterList;


/**
	@brief		
*/
class TriggerBase
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	TriggerBase();
	virtual ~TriggerBase();

public:	// internal
	/// routedEvent : �Ⴆ�΃T�u�N���X Trigger �� PropertyChangedEvent ���󂯎������ target �Ƀv���p�e�B���Z�b�g����
	/// tareget : 
	virtual void Invoke(RoutedEvent* routedEvent, CoreObject* tareget) = 0;
};

/**
	@brief		
*/
class Trigger
	: public TriggerBase
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Trigger();
	virtual ~Trigger();

public:
	void SetProperty(const Property* prop) { m_property = prop; }
	const Property* GetProperty() const { return m_property; }
	void SetValue(const Variant& value) { m_value = value; }
	const Variant& GetValue() const { return m_value; }

protected:
	virtual void Invoke(RoutedEvent* routedEvent, CoreObject* tareget);

private:
	const Property*		m_property;
	Variant				m_value;
};

/**
	@brief		
*/
class Style
	: public CoreObject
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
public:
	Style();
	virtual ~Style();

public:
	void SetTargetType();
	TypeInfo* GetTargetType();
	void SetBasedOn(Style* style) { m_baseStyle = style; }
	Style* GetBasedOn() const { return m_baseStyle; }

	SetterList* GetSetters() const { return m_setterList; }

	//Triggers

private:
	RefPtr<Style>		m_baseStyle;
	RefPtr<SetterList>	m_setterList;
};

/**
	@brief		
*/
class ResourceDictionary
	: public CoreObject
{
public:
	ResourceDictionary();
	virtual ~ResourceDictionary();

	bool TryGetItem(const String& key, CoreObject** outObject);

	void AddItem(const String& key, CoreObject* obj);


	/// x:key �������ATargetType �Ń^�[�Q�b�g���w�肳��Ă��� ControlTemplate �͂���Ō������� (�݂���Ȃ���� NULL)
	bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);

	void AddControlTemplate(ControlTemplate* outTemplate);

private:
	typedef std::map<String, CoreObject*>	ItemMap;
	typedef std::pair<String, CoreObject*> ItemPair;
	ItemMap	m_items;

	typedef std::map<String, ControlTemplate*>	ControlTemplateMap;
	typedef std::pair<String, ControlTemplate*>	ControlTemplatePair;
	ControlTemplateMap	m_controlTemplateMap;
};
	
/// UIElement �͐e UIElement �ɒǉ��ꂽ���A�e�� CombinedLocalResource ��
/// ������ Resource �v���p�e�B�̒l������ CombinedLocalResource �����B
/// ���̂��� VisualTree �̍쐬���s�����A���̎��͂��� CombinedLocalResource ����e��f�[�^��ǂݎ��B
/// ������ Resource �� NULL �ł���ΐe�� CombinedLocalResource �����̂܂܎Q�Ƃ���B
class CombinedLocalResource
	: public RefObject
{
public:
	CombinedLocalResource();
	virtual ~CombinedLocalResource();

	void Combine(CombinedLocalResource* parent, ResourceDictionary* local);

	CoreObject* GetItem(const String& key);
	bool TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate);

public:
	CombinedLocalResource*	m_parent;
	ResourceDictionary*		m_local;
};

} // namespace GUI
} // namespace Lumino
