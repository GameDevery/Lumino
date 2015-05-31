
#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

namespace Lumino
{
namespace GUI
{

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

	void AddChild(UIElementFactory* child);


	UIElement* CreateInstance();

private:
	GUIManager*						m_manager;	// CreateInstance() �ŕK�v�B�����ł�����Ă����������H
	String							m_targetTypeFullName;
	ArrayList<UIElementFactory*>	m_children;
};

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

	void SetVisualTreeRoot(UIElementFactory* factory) { m_visualTreeRoot = factory; }

	/// �w�肳�ꂽ Control �ɂ��̃e���v���[�g��K�p���܂��B
	void Apply(Control* control);

private:
	String						m_targetType;		///< �ΏۃR���g���[���� ("Button" ��)
	RefPtr<UIElementFactory>	m_visualTreeRoot;	///< �e���v���[�g�� VisualTree �̃��[�g�m�[�h
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
