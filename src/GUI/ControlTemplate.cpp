
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
				pair.first->AddItem(obj, item);
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
			obj->SetPropertyValue(pair.first, pair.second);
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
		
		rootLogicalParent->PollingTemplateChildCreated(element);
	}
	else
	{
		// ContentElement �͂����ɗ���
	}

	return obj;
}

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
		RefPtr<CoreObject> obj(m_visualTreeRoot->CreateInstance(control));
		UIElement* element = dynamic_cast<UIElement*>(obj.GetObjectPtr());
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
	}

	// �v���p�e�B�K�p
	for (auto prop : m_propertyValueList)
	{
		bool isElement = false;
		if (prop.second.GetType() == VariantType_Object)
		{
			UIElementFactory* factory = static_cast<UIElementFactory*>(prop.second.GetObject());
			if (factory != NULL)
			{
				RefPtr<CoreObject> element(factory->CreateInstance(control));
				control->SetPropertyValue(prop.first, element);
				isElement = true;
			}
		}

		if (!isElement) {
			control->SetPropertyValue(prop.first, prop.second);
		}
	}
}


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

	if (m_visualTreeRoot != NULL) {
		CoreObject* obj = m_visualTreeRoot->CreateInstance(control);
		UIElement* element = dynamic_cast<UIElement*>(obj);
		if (element != NULL)
		{
			control->SetTemplateChild(element);
		}
		else {
			// TODO: �����ɗ����Ƃ��Ƀ��������[�N����
		}
	}

	// TODO: �v���p�e�B�K�p�����B
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
	LN_FOREACH(ControlTemplatePair p, m_controlTemplateMap) {
		p.second->Release();
	}
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
bool ResourceDictionary::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
{
	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return false; }
	ControlTemplateMap::iterator itr = m_controlTemplateMap.find(fullTypeName);
	if (itr != m_controlTemplateMap.end()) {
		*outTemplate = itr->second;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ResourceDictionary::AddControlTemplate(ControlTemplate* outTemplate)
{
	if (LN_VERIFY_ASSERT(outTemplate != NULL)) { return; }
	m_controlTemplateMap[outTemplate->GetTargetType()] = outTemplate;
	outTemplate->AddRef();
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
bool CombinedLocalResource::TryGetControlTemplate(const String& fullTypeName, ControlTemplate** outTemplate)
{
	// local �D��
	if (m_local != NULL && m_local->TryGetControlTemplate(fullTypeName, outTemplate)) {
		return true;
	}
	if (m_parent != NULL && m_parent->TryGetControlTemplate(fullTypeName, outTemplate)) {
		return true;
	}
	return false;
}

} // namespace GUI
} // namespace Lumino

