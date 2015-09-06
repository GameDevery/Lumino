﻿

#pragma once
#include <map>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include "Common.h"
#include "EventArgs.h"
#include "CursorImage.h"

namespace Lumino
{
namespace GUI
{
namespace Internal { class GUIPainter; }
	
/**
	@brief		GUI 機能の管理クラスです。
*/
class GUIManager
	: public RefObject
{
public:
	static const float DefaultouseButtonClickTimeout;

public:
	struct ConfigData
	{
		Graphics::GraphicsManager*		GraphicsManager;
		Platform::Window*				MainWindow;
		Documents::DocumentsManager*	DocumentsManager;

		ConfigData()
			: GraphicsManager(NULL)
			, MainWindow(NULL)
			, DocumentsManager(NULL)
		{}
	};

public:
	GUIManager();
	virtual ~GUIManager();

public:
	void Initialize(const ConfigData& configData);
	void Finalize();
	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }
	RootFrame* GetDefaultRootFrame() const { return m_defaultRootFrame; }

	void RegisterFactory(const String& typeFullName, ObjectFactory factory);	// とりあえず内部用。言語バインダのことは後で。
	CoreObject* CreateObject(const String& typeFullName);

	bool InjectMouseMove(float clientX, float clientY);
	bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	bool InjectMouseWheel(int delta, float clientX, float clientY);
	bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectChar(TCHAR ch);
	//bool InjectChar(int ch);
	void InjectElapsedTime(float elapsedTime);

	double GetTime() const { return m_time; }

	void SetViewPixelSize(const Size& viewPixelSize) { m_viewPixelSize = viewPixelSize; }	// TODO: GUIContext
	void Render();	// TODO: GUIContext

public:	// internal
	ResourceDictionary* GetDefaultTheme() { return m_defaultTheme; }
	CombinedLocalResource* GetRootCombinedResource() { return m_rootCombinedResource; }
	EventArgsPool* GetEventArgsPool() { return &m_eventArgsPool; }

	void SetFocusElement(UIElement* element);	// TODO: GUIContext

	void CaptureMouse(UIElement* element);
	void ReleaseMouseCapture(UIElement* element);

private:
	bool UpdateMouseHover(const PointF& mousePos);	// ハンドリングしたら true を返す
	void BuildDefaultTheme();


private:	// internal
	void AddAnimationClock(AnimationClock* clock);
	void RemoveAnimationClock(AnimationClock* clock);

	//static char KeyToChar(Key keyCode);

private:
	typedef std::map<String, ObjectFactory>	ObjectFactoryMap;
	typedef std::pair<String, ObjectFactory>	ObjectFactoryPair;

	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	Internal::GUIPainter*				m_painter;

	EventArgsPool						m_eventArgsPool;
	RootFrame*							m_defaultRootFrame;
	ResourceDictionary*					m_defaultTheme;
	CombinedLocalResource*				m_rootCombinedResource;
	UIElement*							m_focusElement;
	UIElement*							m_mouseHoverElement;		///< 現在マウス位置にある UIElement
	UIElement*							m_capturedElement;			///< 現在キャプチャ中の UIElement
	RefPtr<Platform::Window>			m_mainWindow;
	Array<AnimationClock*>				m_activeAnimationClockList;	///< TODO: インデックススタックを使ったリストにしたい

	ObjectFactoryMap					m_objectFactoryMap;

	double								m_time;	// TODO: context

	// GUIContext
	struct MouseClickTracker
	{
		double		LastTime;
		int			ClickCount;
		UIElement*	HoverElement;
	};

	MouseClickTracker m_mouseClickTrackers[MouseButton::TERMINATOR];

	RefPtr<Graphics::Texture>	m_defaultSkinTexture;
	RefPtr<CursorImage>			m_cursorImageTable[1];
	RefPtr<CursorImage>			m_currentCursorImage;
	PointF						m_mousePosition;
	float						m_mouseButtonClickTimeout;
	float						m_cursorAnimationTime;
	Size						m_viewPixelSize;


	friend class GUIHelper;
};

} // namespace GUI
} // namespace Lumino
