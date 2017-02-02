﻿
#pragma once
#include "../Platform/EventListener.h"
#include "../Graphics/Viewport.h"
#include "UIElement.h"

LN_NAMESPACE_BEGIN
class PlatformWindow;
class SwapChain;

/**
	@brief		
*/
class UIViewportLayer
	: public ViewportLayer
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static const int DefaultZIndex = 100;

protected:
	friend class UIFrameWindow;
	UIViewportLayer(UILayoutView* view);
	virtual ~UIViewportLayer();
	virtual DrawList* GetRenderer() override { return nullptr; }
	virtual void Render() override;
	virtual void ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;

LN_INTERNAL_ACCESS:
	void Initialize();

private:
	UILayoutView*		m_view;
	RefPtr<DrawList>	m_renderingContext;
	RefPtr<detail::InternalRenderer>	m_internalRenderer;
};


/**
	@brief		
*/
class UIFrameWindow
	: public Object
	, public IEventListener
{
	LN_UI_TYPEINFO_DECLARE();
public:

	PlatformWindow* GetPlatformWindow() const { return m_platformWindow; }

	Viewport* GetViewport() const { return m_mainViewport; }

	void SetSize(const SizeI& size);

protected:
	UIFrameWindow();
	virtual ~UIFrameWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow, SwapChain* swapChain, UILayoutView* view);
	virtual bool OnEvent(const PlatformEventArgs& e) override;

LN_INTERNAL_ACCESS:
	detail::UIManager* GetManager() const { return m_manager; }
	SwapChain* GetSwapChain() const { return m_swapChain; }


	void Render();

	void BeginRendering();
	void RenderContents();
	void EndRendering();

private:
	void UpdateViewportTransform();

	detail::UIManager*		m_manager;
	PlatformWindow*			m_platformWindow;
	SwapChain*				m_swapChain;
	Viewport*				m_mainViewport;
	RefPtr<UIViewportLayer>	m_uiLayer;
};


/**
	@brief		メインウィンドウのクラス。
*/
class UIMainWindow
	: public UIFrameWindow
{
	LN_UI_TYPEINFO_DECLARE();
public:
	UIContext* GetMainUIContext() const { return m_mainUIContext; }

LN_INTERNAL_ACCESS:
	UIMainWindow();
	virtual ~UIMainWindow();
	void Initialize(detail::UIManager* manager, PlatformWindow* platformWindow);

	void InjectElapsedTime(float elapsedTime);
	void UpdateLayout(const Size& viewSize);	// TODO: ゆくゆくは SwapChain や Viewport も UIFrameWindow にもってくる。そのとき、この viewSize はいらなくなる
	void RenderUI();

private:
	UIContext*	m_mainUIContext;
};




class UINativeHostWindow;
using UINativeHostWindowPtr = RefPtr<UINativeHostWindow>;

/**
	@brief		ネイティブウィンドウをホストするための UIFrameWindow クラスです。
*/
class UINativeHostWindow
	: public UIFrameWindow
{
	LN_UI_TYPEINFO_DECLARE();
public:

	/**
		@brief		UIFrameWindow を作成します。
	*/
	static UINativeHostWindowPtr Create(intptr_t windowHandle);


public:

	/** ホストされたネイティブウィンドウへ描画を行います。*/
	void Render();

LN_INTERNAL_ACCESS:
	UINativeHostWindow();
	virtual ~UINativeHostWindow();
	void Initialize(detail::UIManager* manager, intptr_t windowHandle);

private:
	UIContext*	m_mainUIContext;
};

LN_NAMESPACE_END