﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/UIDialog.hpp>
#include "../Effect/EffectManager.hpp"  // TODO: test
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"

namespace ln {

UIFrameRenderView::UIFrameRenderView()
    : m_rootElement(nullptr)
{
}

void UIFrameRenderView::init()
{
    RenderView::init();

    m_renderingContext = makeRef<UIRenderingContext>();
    m_sceneRenderingPipeline = makeRef<detail::FlatRenderingPipeline>();
    m_sceneRenderingPipeline->init();
    m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();
    m_viewPoint = makeObject<RenderViewPoint>();

    //m_clearRenderPass = makeObject<RenderPass>();

    m_adornerLayer = makeObject<UIAdornerLayer>(this);

    m_drawElementListCollector->addDrawElementList(/*RendringPhase::Default, */m_renderingContext->m_elementList);
    addDrawElementListManager(m_drawElementListCollector);
}

void UIFrameRenderView::setRootElement(UIElement* element)
{
    if (m_rootElement) {
        m_rootElement->m_renderView = nullptr;
    }

    m_rootElement = element;

    if (m_rootElement) {
        m_rootElement->m_renderView = this;
    }
}

UIElement* UIFrameRenderView::rootElement() const
{
    return m_rootElement;
}

UIAdornerLayer* UIFrameRenderView::adornerLayer() const
{
    return m_adornerLayer;
}

void UIFrameRenderView::render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget)
{
    if (m_rootElement)
    {



        //FrameBuffer fb;
        //fb.renderTarget[0] = graphicsContext->renderPass()->renderTarget(0);
        //fb.depthBuffer = graphicsContext->renderPass()->depthBuffer();

        // TODO:
        detail::CameraInfo camera;
        {
            m_viewPoint->viewPixelSize = camera.viewPixelSize = Size(renderTarget->width(), renderTarget->height());	// TODO: 必要？
            m_viewPoint->viewPosition = camera.viewPosition = Vector3::Zero;
            m_viewPoint->viewDirection = camera.viewDirection = Vector3::UnitZ;
            m_viewPoint->viewMatrix = camera.viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);//Matrix();// 
            m_viewPoint->projMatrix = camera.projMatrix = Matrix::makePerspective2DLH(camera.viewPixelSize.width, camera.viewPixelSize.height, 0, 1000);
            m_viewPoint->viewProjMatrix = camera.viewProjMatrix = camera.viewMatrix * camera.projMatrix;
            m_viewPoint->viewFrustum = camera.viewFrustum = ViewFrustum(camera.viewProjMatrix);
            m_viewPoint->nearClip = camera.nearClip = 0;
            m_viewPoint->farClip = camera.farClip = 1000;
        }


        //ClearInfo clearInfo;
        //clearInfo.color = backgroundColor();
        //clearInfo.depth = 1.0f;
        //clearInfo.stencil = 0x00;
        //if (clearMode() == RenderViewClearMode::ColorAndDepth) {
        //    clearInfo.flags = ClearFlags::All;
        //}
        //else {
        //    clearInfo.flags = ClearFlags::Depth;
        //}

        // build draw elements
        {
            m_renderingContext->resetForBeginRendering();
            //m_renderingContext->setRenderTarget(0, fb.renderTarget[0]);
            //m_renderingContext->setDepthBuffer(fb.depthBuffer);
            m_renderingContext->setViewPoint(m_viewPoint);
            m_renderingContext->m_frameWindowRenderingGraphicsContext = graphicsContext;

            if (clearMode() == RenderViewClearMode::ColorAndDepth) {
                m_renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
            }
            else {
                m_renderingContext->clear(ClearFlags::Depth, backgroundColor(), 1.0f, 0x00);
            }

            m_rootElement->render(m_renderingContext);


            m_renderingContext->m_frameWindowRenderingGraphicsContext = nullptr;

            m_adornerLayer->render(m_renderingContext);

            detail::EngineDomain::effectManager()->testDraw(m_renderingContext);
            //detail::EngineDomain::effectManager()->testDraw2(graphicsContext);


            //if (m_dialog) {
            //    m_dialog->render(m_renderingContext);
            //}
        }





        m_sceneRenderingPipeline->render(graphicsContext, renderTarget/*, clearInfo*/, &camera, &elementListManagers());
    }
}

void UIFrameRenderView::invalidate(detail::UIElementDirtyFlags flags)
{
    // TODO: 今は AdonerLayer のを FrameWindow に伝えるために使用しているが、RenderView をルートとするべきかもしれない
    rootElement()->invalidate(flags, false);
}

//==============================================================================
// UIRenderView

UIRenderView::UIRenderView()
{
}

void UIRenderView::init()
{
    UIFrameRenderView::init();
}

//void UIRenderView::setDialog(UIDialog* dialog)
//{
//    m_dialog = dialog;
//    m_rootElement->invalidateStyle();  // TODO: 仮。とりあえずの動作テスト用
//}

void UIRenderView::onUpdateFrame(float elapsedSeconds)
{
    rootElement()->updateFrame(elapsedSeconds);
    //if (m_dialog) { // TODO: このあたりは VisualTree に任せたい
    //    m_dialog->updateFrame(elapsedSeconds);
    //}
}

void UIRenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    rootElement()->updateStyleHierarchical(styleContext, finalStyle);
	adornerLayer()->updateStyleHierarchical(styleContext, finalStyle);
    //if (m_dialog) { // TODO: このあたりは VisualTree に任せたい
    //    m_dialog->updateStyleHierarchical(styleContext, finalStyle);
    //}
}

void UIRenderView::onUpdateUILayout(UILayoutContext* layoutContext, const Rect& finalGlobalRect)
{
    rootElement()->updateLayout(layoutContext, Rect(0, 0, finalGlobalRect.getSize()));
    rootElement()->updateFinalLayoutHierarchical(layoutContext, finalGlobalRect);
    adornerLayer()->measureLayout(layoutContext, finalGlobalRect.getSize());
    adornerLayer()->arrangeLayout(layoutContext, finalGlobalRect);

    //if (m_dialog) {
    //    m_dialog->updateLayout(Rect(0, 0, finalGlobalRect.getSize()));
    //    m_dialog->updateFinalLayoutHierarchical(finalGlobalRect);
    //}
}

UIElement* UIRenderView::onLookupMouseHoverElement(const Point& frameClientPosition)
{
	UIElement* element = adornerLayer()->lookupMouseHoverElement(frameClientPosition);
	if (element) {
		return element;
	}

    //if (m_dialog) {
    //    return m_dialog->lookupMouseHoverElement(frameClientPosition);
    //}
    //else {
        return rootElement()->lookupMouseHoverElement(frameClientPosition);
    //}
}

} // namespace ln

