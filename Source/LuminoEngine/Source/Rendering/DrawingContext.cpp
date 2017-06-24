
#include "../Internal.h"
#include "ShapesRenderFeature.h"
#include "../Graphics/GraphicsManager.h"
#include <Lumino/Rendering/DrawingContext.h>

LN_NAMESPACE_BEGIN

class DrawElement_DrawShapesRendererCommandList : public detail::DrawElement
{
public:
	detail::ShapesRendererCommandList*	m_commandList = nullptr;

	detail::ShapesRendererCommandList* GetGCommandList(DrawList* owner)
	{
		if (m_commandList == nullptr)
		{
			// ���̃R�}���h���X�g�� ShapesRenderFeature ���ŉ�������
			// TODO: ���ꂾ�ƕ`��t���[���X�L�b�v���ɑΉ��ł��Ȃ�
			m_commandList = owner->getManager()->getShapesRendererCommandListCache()->queryCommandList();
		}
		return m_commandList;
	}

	//virtual void makeElementInfo(detail::DrawElementList* oenerList, const detail::CameraInfo& cameraInfo, detail::ElementInfo* outInfo) override
	//{
	//	// ���[���h�s��͍��Ȃ��B��A�� Glyphs ��`�悷����ɔC����B
	//	// (�X�v���C�g�Ɠ������A�ł��邾����x�ɕ`�悷��)
	//	outInfo->viewProjMatrix = &cameraInfo.viewProjMatrix;
	//	outInfo->WorldMatrix = Matrix::Identity;//getTransform(oenerList);
	//	outInfo->WorldViewProjectionMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;// outInfo->WorldMatrix * cameraInfo.viewMatrix * cameraInfo.projMatrix;	// TODO: viewProj �͂܂Ƃ߂���
	//	outInfo->affectedLights = getAffectedDynamicLightInfos();
	//}

	virtual void drawSubset(const DrawArgs& e) override
	{
		auto* r = e.context->beginShapesRenderer();
		r->executeCommand(m_commandList);
		m_commandList = nullptr;
	}
	virtual void reportDiag(RenderDiag* diag) override { diag->callCommonElement("DrawShapesRendererCommandList"); }
};

//==============================================================================
// DrawingContext
//==============================================================================

//------------------------------------------------------------------------------
DrawingContext::DrawingContext()
{
}

//------------------------------------------------------------------------------
DrawingContext::~DrawingContext()
{
}

//------------------------------------------------------------------------------
void DrawingContext::initialize()
{
	DrawList::initialize(detail::EngineDomain::getGraphicsManager());
}

//------------------------------------------------------------------------------
void DrawingContext::drawTexture(const Rect& destRect, Texture* texture, const Rect& sourceRect)
{
	drawSprite(Vector3(destRect.x, destRect.y, 0), destRect.getSize(), Vector2(0, 0), texture, sourceRect, Color::White, SpriteBaseDirection::Basic2D, BillboardType::None, nullptr);
}

int g_g_calls_drawBoxBackground = 0;
//------------------------------------------------------------------------------
void DrawingContext::drawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius)
{
	Brush* brush = getBrush();
	if (brush != nullptr &&
		(brush->getImageDrawMode() == BrushImageDrawMode::BoxFrame || brush->getImageDrawMode() == BrushImageDrawMode::BorderFrame))
	{
		drawFrameRectangle(rect);
	}
	else
	{
		g_g_calls_drawBoxBackground++;
		m_currentStateFence++;
		auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, getManager()->getInternalContext()->m_shapesRenderer, nullptr);
		auto* list = ptr->GetGCommandList(this);
		list->addDrawBoxBackground(rect, cornerRadius);
	}
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxBorder(
	const Rect& rect, const ThicknessF& thickness, const CornerRadius& cornerRadius,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	BorderDirection borderDirection)
{
	m_currentStateFence++;
	auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, getManager()->getInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);

	list->addDrawBoxBorder2(
		rect, thickness,
		leftColor, topColor, rightColor, bottomColor,
		cornerRadius, (borderDirection == BorderDirection::Inside));
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxBorder(
	const Rect& rect, const ThicknessF& thickness,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	float ltRad, float rtRad, float lbRad, float rbRad, BorderDirection borderDirection,
	const Color& shadowColor, float shadowBlur, float shadowWidth, ShadowDirection shadowDirection)
{
	m_currentStateFence++;
	auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, getManager()->getInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->addDrawBoxBorder(
		rect.x, rect.y, rect.width, rect.height, thickness.Left, thickness.Top, thickness.Right, thickness.Bottom,
		leftColor, topColor, rightColor, bottomColor,
		ltRad, rtRad, lbRad, rbRad,
		shadowColor, shadowBlur, shadowWidth, (shadowDirection == ShadowDirection::Inside), (borderDirection == BorderDirection::Inside));
}

//------------------------------------------------------------------------------
void DrawingContext::drawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, ShadowDirection shadowDirection)
{
	m_currentStateFence++;
	auto* ptr = resolveDrawElement<DrawElement_DrawShapesRendererCommandList>(detail::DrawingSectionId::NanoVG, getManager()->getInternalContext()->m_shapesRenderer, nullptr);
	auto* list = ptr->GetGCommandList(this);
	list->addDrawBoxShadow(rect, cornerRadius, color, blur, width, (shadowDirection == ShadowDirection::Inside));
}

LN_NAMESPACE_END

