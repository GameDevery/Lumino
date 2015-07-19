
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/UIElement.h>
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElementFactory
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElementFactory::UIElementFactory(GUIManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
UIElementFactory::~UIElementFactory()
{
	LN_FOREACH(UIElementFactory* f, m_children) {
		f->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElementFactory::AddChild(UIElementFactory* child)
{
	if (child != NULL)
	{
		m_children.Add(child);
		LN_SAFE_ADDREF(child);
	}
}

#if 0
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* UIElementFactory::CreateInstance()
{
	CoreObject* obj = m_manager->CreateObject(m_targetTypeFullName);
	if (obj == NULL) {
		// TODO: 
		LN_THROW(0, InvalidOperationException);
	}
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void UIElementFactory::BuildInstance(CoreObject* obj, UIElement* rootLogicalParent)
{
	// �v���p�e�B��ݒ肷��
	for (PropertyValueList::Pair& pair : m_propertyValueList)
	{
		if (pair.first->IsList())/*.second.GetType() == VariantType_List)*/
		{
			// ���X�g�̏ꍇ�͏�������B�I�u�W�F�N�g�̃����o�̃��X�g�͊��ɂ����Ă���O��ŁA
			// ����ɑ΂��ėv�f��1���� Add ���Ă����B
#if 1
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list)
			{
				UIElementFactory* factory = NULL;
				// �ݒ肵�����l������ Factory �������ꍇ�̓C���X�^���X������Đݒ肷��
				if (item.GetType() == VariantType_Object &&
					(factory = dynamic_cast<UIElementFactory*>(item.GetObject())) != NULL)	// TODO: dynamic_cast ����Ȃ��� TypeInfo �g���Ώ��������Ȃ邩���H
				{
					RefPtr<CoreObject> v(factory->CreateInstance());	// �����
					pair.first->AddItem(obj, v);						// �ǉ����� (���[�J�����\�[�X�X�V)
					factory->BuildInstance(v, rootLogicalParent);		// �v���p�e�B�ݒ�
				}
				else {
					pair.first->AddItem(obj, item);
				}
			}
#else
			Variant v = obj->GetPropertyValue(pair.first);
			LN_THROW(v.GetType() == VariantType_List, InvalidOperationException);	// �^�[�Q�b�g�̌^�͕K�� List �łȂ���΂Ȃ�Ȃ�
			VariantList* targetList = v.GetList();
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				targetList->AddVariant(item);
			}
#endif
		}
		else
		{
			Variant& item = pair.second;
			UIElementFactory* factory = NULL;
				
			// �ݒ肵�����l������ Factory �������ꍇ�̓C���X�^���X������Đݒ肷��
			if (item.GetType() == VariantType_Object &&
				(factory = dynamic_cast<UIElementFactory*>(item.GetObject())) != NULL)	// TODO: dynamic_cast ����Ȃ��� TypeInfo �g���Ώ��������Ȃ邩���H
			{
				RefPtr<CoreObject> v(factory->CreateInstance());	// �����
				pair.first->AddItem(obj, v);						// �ǉ����� (���[�J�����\�[�X�X�V)
				factory->BuildInstance(v, rootLogicalParent);		// �v���p�e�B�ݒ�
			}
			else {
				obj->SetPropertyValue(pair.first, item);
			}
		}
	}

	UIElement* element = dynamic_cast<UIElement*>(obj);
	if (element != NULL)
	{
		LN_FOREACH(PropertyInfoList::Pair& pair, m_propertyInfoList)
		{
			if (pair.second.Kind == PropertyKind_TemplateBinding)
			{
				const Property* prop = pair.first;//GetTypeInfo(element)->FindProperty(pair.first);
				//if (prop == NULL) {
				//	LN_THROW(0, InvalidOperationException);	// TODO: XML �G���[�Ƃ����낢��l����K�v������
				//}
				element->SetTemplateBinding(prop, pair.second.SourcePropPath, rootLogicalParent);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}

		//for (int i = 0; i < m_propertyInfoList.GetCount(); ++i)
		//{

		//	
		//}
		//
		// �q�̏���
		LN_FOREACH(UIElementFactory* factory, m_children) {
			RefPtr<CoreObject> e(factory->CreateInstance());	// �����
			element->AddChild(e);								// �ǉ����� (���[�J�����\�[�X�X�V)
			factory->BuildInstance(e, rootLogicalParent);		// �v���p�e�B�ݒ�
		}

		rootLogicalParent->PollingTemplateChildCreated(element);
	}
	else
	{
		// ContentElement �͂����ɗ���
	}
}
#endif
#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* UIElementFactory::CreateInstance(UIElement* rootLogicalParent)
{
	CoreObject* obj = m_manager->CreateObject(m_targetTypeFullName);
	if (obj == NULL) {
		// TODO: 
		LN_THROW(0, InvalidOperationException);
	}

	//std::map<String, int> tmp;
	////std::pair<String, int>& y;
	//typedef std::pair<String, int> tttt;
	//using namespace std;
	//LN_FOREACH(pair<String, int> y, tmp)
	//{
	//}

	// �v���p�e�B��ݒ肷��
	for (PropertyValueList::Pair& pair : m_propertyValueList)
	{
		if (pair.first->IsList())/*.second.GetType() == VariantType_List)*/
		{
			// ���X�g�̏ꍇ�͏�������B�I�u�W�F�N�g�̃����o�̃��X�g�͊��ɂ����Ă���O��ŁA
			// ����ɑ΂��ėv�f��1���� Add ���Ă����B
#if 1
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				if (item.GetType() == VariantType_Object &&
					dynamic_cast<UIElementFactory*>(item.GetObject()) != NULL)	// TODO: dynamic_cast ����Ȃ��� TypeInfo �g���Ώ��������Ȃ邩���H
				{
					RefPtr<CoreObject> v(static_cast<UIElementFactory*>(item.GetObject())->CreateInstance(rootLogicalParent));
					pair.first->AddItem(obj, v);
				}
				else {
					pair.first->AddItem(obj, item);
				}
			}
#else
			Variant v = obj->GetPropertyValue(pair.first);
			LN_THROW(v.GetType() == VariantType_List, InvalidOperationException);	// �^�[�Q�b�g�̌^�͕K�� List �łȂ���΂Ȃ�Ȃ�
			VariantList* targetList = v.GetList();
			VariantList* list = pair.second.GetList();
			for (Variant& item : *list) {
				targetList->AddVariant(item);
			}
#endif
		}
		else
		{
			Variant& item = pair.second;
			if (item.GetType() == VariantType_Object &&
				dynamic_cast<UIElementFactory*>(item.GetObject()) != NULL)	// TODO: dynamic_cast ����Ȃ��� TypeInfo �g���Ώ��������Ȃ邩���H
			{
				RefPtr<CoreObject> v(static_cast<UIElementFactory*>(item.GetObject())->CreateInstance(rootLogicalParent));
				pair.first->AddItem(obj, v);
			}
			else {
				obj->SetPropertyValue(pair.first, item);
			}
		}
	}

	UIElement* element = dynamic_cast<UIElement*>(obj);
	if (element != NULL)
	{
		LN_FOREACH(PropertyInfoList::Pair& pair, m_propertyInfoList)
		{
			if (pair.second.Kind == PropertyKind_TemplateBinding)
			{
				const Property* prop = pair.first;//GetTypeInfo(element)->FindProperty(pair.first);
				//if (prop == NULL) {
				//	LN_THROW(0, InvalidOperationException);	// TODO: XML �G���[�Ƃ����낢��l����K�v������
				//}
				element->SetTemplateBinding(prop, pair.second.SourcePropPath, rootLogicalParent);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}
		}

		//for (int i = 0; i < m_propertyInfoList.GetCount(); ++i)
		//{

		//	
		//}
		//
		// �q�̏���
		LN_FOREACH(UIElementFactory* c, m_children) {
			RefPtr<CoreObject> e(c->CreateInstance(rootLogicalParent));
			element->AddChild(e);
		}
		
		if (rootLogicalParent != NULL) {
			rootLogicalParent->PollingTemplateChildCreated(element);
		}
	}
	else
	{
		// ContentElement �͂����ɗ���
	}

	return obj;
}
#endif

//=============================================================================
// ControlTemplate
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ControlTemplate::ControlTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ControlTemplate::~ControlTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ControlTemplate::Apply(Control* control)
{
	if (LN_VERIFY_ASSERT(control != NULL)) { return; }

	// ���낢�냊�Z�b�g�BTemplateBinding �������B
	control->SetTemplateChild(NULL);

	if (m_visualTreeRoot != NULL) {
#if 0
		// �܂��C���X�^���X���쐬����
		RefPtr<CoreObject> obj(m_visualTreeRoot->CreateInstance());

		// ���ɐe�v�f�ɒǉ����� (���[�J�����\�[�X���X�V����)
		UIElement* element = dynamic_cast<UIElement*>(obj.GetObjectPtr());
		if (element != NULL) { control->SetTemplateChild(element); }

		// �Ō�Ƀv���p�e�B�̐ݒ�⑷�v�f�̍쐬���s��
		m_visualTreeRoot->BuildInstance(m_visualTreeRoot, control);
#else
		RefPtr<CoreObject> obj(m_visualTreeRoot->CreateInstance(control));
		UIElement* element = dynamic_cast<UIElement*>(obj.GetObjectPtr());
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
#endif
	}

	// �v���p�e�B�K�p
#if 0
	for (auto prop : m_propertyValueList)
	{
		bool isElement = false;
		if (prop.second.GetType() == VariantType_Object)
		{
			UIElementFactory* factory = dynamic_cast<UIElementFactory*>(prop.second.GetObject());
			if (factory != NULL)
			{
#if 0
				// �܂��C���X�^���X���쐬����
				RefPtr<CoreObject> element(factory->CreateInstance());

				// ���ɐe�v�f�ɒǉ����� (���[�J�����\�[�X���X�V����)
				control->SetPropertyValue(prop.first, element);

				// �Ō�Ƀv���p�e�B�̐ݒ�⑷�v�f�̍쐬���s��
				factory->BuildInstance(element, control);
#else
				RefPtr<CoreObject> element(factory->CreateInstance(control));
				control->SetPropertyValue(prop.first, element);
#endif

				isElement = true;
			}
		}

		if (!isElement) {
			control->SetPropertyValue(prop.first, prop.second);
		}
	}
#endif
}


#if 0
//=============================================================================
// DataTemplate
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DataTemplate::DataTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DataTemplate::~DataTemplate()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void DataTemplate::Apply(Control* control)
{
	if (LN_VERIFY_ASSERT(control != NULL)) { return; }

	if (m_visualTreeRoot != NULL)
	{
#if 0
		// �܂��C���X�^���X���쐬����
		CoreObject* obj = m_visualTreeRoot->CreateInstance();

		// ���ɐe�v�f�ɒǉ����� (���[�J�����\�[�X���X�V����)
		UIElement* element = dynamic_cast<UIElement*>(obj);
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
		else {
			// TODO: �����ɗ����Ƃ��Ƀ��������[�N����
		}

		// �Ō�Ƀv���p�e�B�̐ݒ�⑷�v�f�̍쐬���s��
		m_visualTreeRoot->BuildInstance(element, control);
#else
		CoreObject* obj = m_visualTreeRoot->CreateInstance(control);
		UIElement* element = dynamic_cast<UIElement*>(obj);
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
		else {
			// TODO: �����ɗ����Ƃ��Ƀ��������[�N����
		}
#endif
	}

	// TODO: �v���p�e�B�K�p�����B
}
#endif


//=============================================================================
// Setter
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Setter, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::Setter()
	: m_property(NULL)
	, m_value()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::Setter(const Property* prop, const Variant& value)
	: m_property(prop)
	, m_value(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Setter::~Setter()
{
}

//=============================================================================
// TriggerBase
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TriggerBase, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TriggerBase::TriggerBase()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TriggerBase::~TriggerBase()
{
}

//=============================================================================
// Trigger
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Trigger, TriggerBase);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Trigger::Trigger()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Trigger::~Trigger()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Trigger::Invoke(RoutedEvent* routedEvent, CoreObject* tareget)
{
}

//=============================================================================
// Style
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Style, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style::Style()
	: m_targetType(NULL)
	, m_baseStyle()
	, m_setterList()
	, m_triggerList()
{
	m_setterList.Attach(LN_NEW SetterList());
	m_triggerList.Attach(LN_NEW TriggerList());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style::~Style()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Style::Apply(UIElement* element)
{
	for (Setter* setter : *m_setterList)
	{
		const Property* prop = setter->GetProperty();
		const Variant& value = setter->GetValue();

		if (value.GetType() == VariantType_Object &&
			dynamic_cast<UIElementFactory*>(value.GetObject()) != NULL)	// TODO: dynamic_cast ����Ȃ��� TypeInfo �g���Ώ��������Ȃ邩���H
		{
			// InitializeComponent() ���Ă΂��
			RefPtr<CoreObject> v(static_cast<UIElementFactory*>(value.GetObject())->CreateInstance(NULL));
			element->SetPropertyValue(prop, v);
		}
		else {
			element->SetPropertyValue(prop, value);
		}
	}
}

//=============================================================================
// ResourceDictionary
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResourceDictionary::ResourceDictionary()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ResourceDictionary::~ResourceDictionary()
{
	LN_FOREACH(ItemPair p, m_items) {
		p.second->Release();
	}
	//LN_FOREACH(ControlTemplatePair p, m_controlTemplateMap) {
	//	p.second->Release();
	//}
	LN_FOREACH(Style* p, m_styleList) {
		p->Release();
	}
	//printf("");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ResourceDictionary::TryGetItem(const String& key, CoreObject** outObject)
{
	if (LN_VERIFY_ASSERT(outObject != NULL)) { return false; }
	ItemMap::iterator itr = m_items.find(key);
	if (itr != m_items.end()) {
		*outObject = itr->second;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ResourceDictionary::AddItem(const String& key, CoreObject* obj)
{
	if (LN_VERIFY_ASSERT(obj != NULL)) { return; }
	m_items[key] = obj;
	obj->AddRef();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style* ResourceDictionary::FindStyle(TypeInfo* type)
{
	Array<Style*>::iterator itr =
		std::find_if(m_styleList.begin(), m_styleList.end(), [type](Style* style){ return style->GetTargetType() == type; });
	if (itr != m_styleList.end()) {
		return *itr;
	}
	return NULL;
}
//bool ResourceDictionary::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
//{
//	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return false; }
//	ControlTemplateMap::iterator itr = m_controlTemplateMap.find(fullTypeName);
//	if (itr != m_controlTemplateMap.end()) {
//		*outTemplate = itr->second;
//		return true;
//	}
//	return false;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ResourceDictionary::AddControlTemplate(ControlTemplate* outTemplate)
//{
//	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return; }
//	m_controlTemplateMap[outTemplate->GetTargetType()] = outTemplate;
//	outTemplate->AddRef();
//}
void ResourceDictionary::AddStyle(Style* style)
{
	LN_VERIFY_RETURN(style != NULL);
	m_styleList.Add(style);
	style->AddRef();
}

//=============================================================================
// CombinedLocalResource
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CombinedLocalResource::CombinedLocalResource()
	: m_parent(NULL)
	, m_local(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CombinedLocalResource::~CombinedLocalResource()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CombinedLocalResource::Combine(CombinedLocalResource* parent, ResourceDictionary* local)
{
	m_parent = parent;
	m_local = local;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* CombinedLocalResource::GetItem(const String& key)
{
	CoreObject* obj;
	if (m_local != NULL && m_local->TryGetItem(key, &obj)) {
		return obj;
	}
	return m_parent->GetItem(key);

	// TOOD: �G���[����ł����H
	//LN_THROW(0, ArgumentException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Style* CombinedLocalResource::FindStyle(TypeInfo* type)
{
	// local �D��
	if (m_local != NULL)
	{
		Style* style = m_local->FindStyle(type);
		if (style != NULL) { return style; }
	}
	// parent
	if (m_parent != NULL)
	{
		Style* style = m_parent->FindStyle(type);
		if (style != NULL) { return style; }
	}
	return NULL;
}

//bool CombinedLocalResource::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
//{
//	// local �D��
//	if (m_local != NULL && m_local->TryGetControlTemplate(fullTypeName, outTemplate)) {
//		return true;
//	}
//	if (m_parent != NULL && m_parent->TryGetControlTemplate(fullTypeName, outTemplate)) {
//		return true;
//	}
//	return false;
//}

} // namespace GUI
} // namespace Lumino

