

#pragma once
#include <map>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/GUI/CursorImage.h>
#include <Lumino/GUI/Common.h>
#include <Lumino/GUI/RoutedEventArgs.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
namespace Internal { class GUIPainter; }
	

/**
	@brief	
	@note	���̃N���X�̓C�x���g�����̕p�ɂ� new ������邽�߂Ɏg�p����B
			�Ⴆ�� MouseMove �C�x���g�͍ň����t���[���̂悤�ɔ�������\��������Anew �͑傫�ȃI�[�o�[�w�b�h�ɂȂ�B
			����Ȃ� union �𗘗p�����C�x���g�����\���̂��g���̂��ЂƂ̎肩������Ȃ��B
			
			�������A�{���C�u������ C# �� Ruby ���A���̌���ւ̃o�C���f�B���O��z�肵�Ă���B
			���R GUI ���W���[�������J����A�g������邱�Ƃ��O��B
			C# ���ō쐬�������[�U�[�R���g���[���� MouseEventArgs ���g�p���邱�Ƃ͑z�肵�Ȃ���΂Ȃ�Ȃ��B

			union �� struct �ɂ����ꍇ�A����͂���Ō���ʂɗ]�v�ȃI�[�o�[�w�b�h�⍇�킹���݂̎����������邱�ƂɂȂ�B
			�Ⴆ�� C# ��B�͒l�n���̂܂܂ł悢�̂��Aref ������̂��BRuby �� struct �͎g���Ȃ��̂Ō��� new ����H�ڂɂȂ邪�����̂��B

			Pool ���Ă����΁A�኱�����I�ł͂Ȃ��Ȃ邪�A�o�C���_���� new ���}���邱�Ƃ��ł���B

			��X�C�x���g�̎�ނ������Ă����Ƃ��͊g�����̂��߁A�C�x���g�����L�[�ɂ��� Create ����悤�Ȏd�g�݂��K�v�ɂȂ邩������Ȃ��B
*/
class EventArgsPool
{
public:
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount);	// TODO: ��߂�
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);	// TODO: ��߂�

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* Create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(Find(TEventArgs::GetClassTypeInfo()));
		if (e == NULL) {
			e = LN_NEW TEventArgs(args...);
			Register(e);
		}
		else {
			e->~TEventArgs();
			new (e)TEventArgs(args...);
		}
		e->Handled = false;
		e->AddRef();
		return e;
	}

private:
	typedef Array<RoutedEventArgs*>					EventArgsList;
	typedef SortedArray<TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	RoutedEventArgs* Find(TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.TryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->GetRefCount() == 1) {	// ���̃��X�g���炵���Q�Ƃ���Ă��Ȃ���ΕԂ�
					return e;
				}
			}
		}
		return NULL;
	}

	void Register(RoutedEventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.TryGetValue(GetTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.Add(GetTypeInfo(e), list);
		}
		list->Add(e);
	}


	//Array<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T FindFreeObject(const Array<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	Array<MouseEventArgs*>	m_mouseEventArgsPool;
	Array<KeyEventArgs*>	m_keyEventArgsPool;
};



/**
	@brief		GUI �@�\�̊Ǘ��N���X�ł��B
*/
class GUIManagerImpl
	: public RefObject
{
public:
	static const float DefaultouseButtonClickTimeout;

	static GUIManagerImpl* Instance;

public:
	struct ConfigData
	{
		GraphicsManager*		GraphicsManager;
		Platform::Window*				MainWindow;
		Documents::DocumentsManager*	DocumentsManager;

		ConfigData()
			: GraphicsManager(NULL)
			, MainWindow(NULL)
			, DocumentsManager(NULL)
		{}
	};

public:
	GUIManagerImpl();
	virtual ~GUIManagerImpl();

public:
	void Initialize(const ConfigData& configData);
	void Finalize();
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }

	void AddContextOnMainWindow(GUIContext* context) { m_mainWindowContextList.Add(context); }
	void RemoveContextOnMainWindow(GUIContext* context) { m_mainWindowContextList.Remove(context); }
	void UpdateLayoutOnMainWindow(const Size& viewPixelSize);
	void RenderOnMainWindow();

	bool InjectMouseMove(float clientX, float clientY);
	bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	bool InjectMouseWheel(int delta, float clientX, float clientY);
	bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectTextInput(TCHAR ch);
	void InjectElapsedTime(float elapsedTime);

private:
	Array<GUIContext*>	m_mainWindowContextList;












public:

	RootFrame* GetDefaultRootFrame() const { return m_defaultRootFrame; }

	Platform::Window* GetMainWindow() const { return m_mainWindow; }



	void RegisterFactory(const String& typeFullName, ObjectFactory factory);	// �Ƃ肠���������p�B����o�C���_�̂��Ƃ͌�ŁB
	CoreObject* CreateObject(const String& typeFullName);



	//double GetTime() const { return m_time; }

	//void SetViewPixelSize(const Size& viewPixelSize) { m_viewPixelSize = viewPixelSize; }	// TODO: GUIContext
	

public:	// internal
	ResourceDictionary* GetDefaultTheme() { return m_defaultTheme; }
	CombinedLocalResource* GetRootCombinedResource() { return m_rootCombinedResource; }
	EventArgsPool* GetEventArgsPool() { return &m_eventArgsPool; }
	CursorImage* GetCursorImage(CommonCursorImage img) { return m_cursorImageTable[img]; }
	Internal::GUIPainter* GetPainter() { return m_painter; }

private:
	bool UpdateMouseHover(const PointF& mousePos);	// �n���h�����O������ true ��Ԃ�
	void BuildDefaultTheme();


private:	// internal

	//static char KeyToChar(Key keyCode);

private:
	typedef std::map<String, ObjectFactory>	ObjectFactoryMap;
	typedef std::pair<String, ObjectFactory>	ObjectFactoryPair;

	RefPtr<GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	Internal::GUIPainter*				m_painter;

	EventArgsPool						m_eventArgsPool;
	RootFrame*							m_defaultRootFrame;
	ResourceDictionary*					m_defaultTheme;
	CombinedLocalResource*				m_rootCombinedResource;
	RefPtr<Platform::Window>			m_mainWindow;

	ObjectFactoryMap					m_objectFactoryMap;



	RefPtr<Texture>	m_defaultSkinTexture;
	RefPtr<CursorImage>			m_cursorImageTable[CommonCursorImage::TERMINATOR];
	//Size						m_viewPixelSize;


	friend class GUIHelper;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
