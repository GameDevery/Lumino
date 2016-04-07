/*
	[2016/1/29] Begin End Swap
		�EEnd �� Swap ��1�ɂ܂Ƃ߂Ă������B(OpenGL �Ƃ� Direct2D �͂���Ȃ���)
		�EBegin �� End �� RenderingContext �� GraphicsContext �Ɋ܂߂Ȃ��ق��������Ǝv���B

		�Q�[�����[�h�ł͂����B
			Engine::BeginRender();
			r1 = RenderingContext::GetContext();
			r1->Draw�`
			sceneGraph->Render(r1);
			Engine::EndRender();

		�c�[�����[�h�ł͂���Ȋ��������R�B
			window->BeginRender();
			r1 = RenderingContext::GetContext();
			r1->Draw�`
			sceneGraph->Render(r1);
			window->EndRender();


	�S�̍\��

		RenderingContext	Renderer	
							PrimitiveRenderer
							SpriteRenderer		
		GraphicsContext		Renderer
							GeometryRenderer
							TextRenderer


		�ŏ�ʂ� RenderingContext �� GraphicsContext�B����ȊO�̓��[�U�[�⑼�̃��W���[���Ɍ��J���Ȃ��B
		RenderingContext �� GraphicsContext �����X�e�[�g�͔r���B
		�������ł�1�� IRenderer ���g�����A�݂��̃X�e�[�g�ύX�͉e�����Ȃ��B

		�܂��A���������̖��� GraphicsContext �� Renderer �𓯎��Ɏg�������Ƃ������Ƃɂ���B
		�Ⴆ�΁AScene �̒��� GUI ��`�������Ƃ��B
		�����łȂ��Ă��A�ŏ��� GraphicsContext �� Renderer �̋@�\���������Ă��܂����Ƃ��Ă������A
		���ꂾ�Ƃ����Ԃł����g���Ȃ��֐������܂�ɂ������Ȃ邵�A�N���X�̋K�͂��傫���Ȃ�B
		�܂�y���[�U�[���C��t���Ȃ���΂Ȃ�Ȃ����Ƃ������Ă��܂��B�z

		Context �́AIRenderer �ɐݒ�ł���S�Ă̐ݒ��ێ�����B���ƃV�F�[�_�B
		Context �� Draw�` �� Flush �ŁA�A�N�e�B�u�ȃR���e�L�X�g�̐؂�ւ����s���B
		���̂Ƃ��Â��R���e�L�X�g�� Flush ���A�V�����R���e�L�X�g�̃X�e�[�g��S�ēK�p����B
		�؂�ւ��ʒm�� Context ���x���ɒʒm����B��܂��Ă���e�� Renderer �� Flush ���ĂԂ��߁B

		[2015/1/22]
			Renderer == RenderingContext �ł͂Ȃ��̂��HRenderingContext �͕K�v�Ȃ̂��H�Ǝv����������Ȃ��B
			Renderer �ɂ܂Ƃ߂Ă��܂��ƁAGraphicsContext �� Renderer �������x���̃N���X�ƂȂ�B
			GraphicsContext �͕`���̃����_�����O�^�[�Q�b�g���X�e�[�g�Ƃ��Ď����Ȃ���΂Ȃ�Ȃ����߁ARenderer ���܂���K�v������B
			Context �̒��ɕʂ� Context ���ł��Ă��܂��Ƃ������ƁB���ɕ��G�B


	[2015/1/21] SceneGraph �����ɒ჌�x�� Renderer �͌��J����H
		
		���������e�� Renderer ���B���Ă���̂́A���[�U�[�̒m��Ȃ��Ƃ���ŃX�e�[�g���ς����
		�����̂킩��ɂ������ɂȂ�Ȃ��悤�ɂ��邽�߁B


	[2015/1/21] �J�����g�� Shader ���Ǘ�����̂͒N�H
		�~GraphicsContext�B
		�~�X�� Renderer �ɊǗ�������̂͂��܂��낵���Ȃ��C������B
		�~�~PrimitiveRenderer �͂܂� SetShader �Ƃ������Ă������Ǝv�����ǁA
		�~�~GeometryRenderer �� WPF �݂����ɃV�X�e���ɂ���ĕK�����s����� Shader �������āA
		�~���̌�Ń��[�U�[�� Shader ��������������Ȃ��B
		�~�i�܂��A���[�U�[ Shader �̒��� GeometryRenderer �p�̊֐����Ă�ł�����Ă��������j

		�E�E�E�ł��A�X��Rendere�ɔC���Ă��܂��Ă����������H
		Activate �̂Ƃ� GraphicsContext ���ʓ|����K�v�Ȃ��Ƃ������Ƃ����B
		�Ƃ肠�����������̕����ŁB


	[2015/1/11] PrimitiveRenderer �Ƃ͂Ȃ���ʂ���H
		PrimitiveRenderer �͒P���Ȍ`��������E��ʂɕ`�悷�邽�߂Ɏg�p����B

	[2015/1/3] �ʒ���ɂ���
		�Ƃ肠�����˂���p�X�͑z�肵�Ȃ��B����͌y�ʂȑg�ݍ��݃��[�h�Ƃ���B
		�{���ɂ����Ƃ����̂��g�������v�]����΁AExpandFill ������ poly2tri �Œu�������郂�[�h�����B

	[2015/12/3]
		Arc
		Polygon
		PolyLine
		Ellipse
		Pie
		Point
		Rect
		RoundRect
		Text
		Image
		Path

		�EPath �ȊO�͔ėp�I�Ȗʒ�����s���������O�Œ��_����Ă��܂����ق��������B
		�E�X�g���[�N�ƃA���`�G�C���A�X�͖ʒ��肵�Ȃ���΂Ȃ�Ȃ��̂ŁA�O���̒��_�͑f�����Q�Ƃł���悤�ɂ���K�v������B

		�A�E�g���C���̃|�C���g���X�g�쐬
		(Path�ȊO)���_���X�g�쐬
		(Path)�ʒ���E���_���X�g�쐬
		Fill���_�o�b�t�@�쐬
		�`��
		Stroke���_�o�b�t�@�쐬
		�`��

	[2015/12/2] �^�C�����O�`��ɂ���
		�f�t�H���g�ł͗L���ɂ��Ȃ��B�V�F�[�_�͕�����B
		�^�C���z�u����u���V���w�肳�ꂽ�ꍇ�A�`�惂�[�h��؂�ւ���B
		��ɗL���ɂ���ɂ͂��܂�ɂ����G������B


	[2015/12/2] �R�}���h�̃L���b�V���ɂ���
		����X�e�[�g�Ԃ� Primitive �`��R�}���h�͈ꊇ�Ŏ��s�ł���B
		�܂Ƃ߂ăR�}���h���X�g�ɑ���Ƃ����̂����A�o�b�t�@�͒N�����́H
		�� �X�e�[�g�̐؂�ւ��͊���ƕp�ɂ��Ǝv����B
		�ނ��� new �̐��������Ă��܂���������Ȃ��B
		memcpy �ŃR�}���h���X�g�̈ꎞ�o�b�t�@�փR�s�[����ق����S�̓I�Ɍ��ăp�t�H�[�}���X�ǂ������B
		sizeof(Matrix) ��1�R�}���h���̕��σT�C�Y�Ƃ��� 10000 �q memcpy ����Ƃ��� 30us ���炢������B
		1000 �Ȃ� 3us�B
		���ۂɂ͂���ȂɎg��Ȃ����낤���Amemcpy �Ŗ��Ȃ��͂��B


	[2015/12/2] �N���X��
		���΂炭�ԋ󂢂āA����ς� GeometryRenderer �̂ق��������Ǝv���B
		���̃��C�u�����͂��Ȃ�჌�x����API�����J����B
		�����Ȃ��Ă���� DirectX11 �� RenderingContext �̈Ӗ��������Ȃ��Ă���B
		�����x�����Ă��Ƃō��ʉ�����Ȃ� GeometryRenderer�B
		


	���O�̌��l�^�͂��̂����肩��B
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	���̃��W���[���� Scene �� GUI �ŋ��L����邪�A
	GeometryRenderer �� Scene ���� GUI ���B
	���C�u�����Ƃ��Ă� Scene �̕����Ӗ����傫���BDraw ��� Render ���ȁA�ƁB


	Scene �ł́A���炩���� SceneNode �̃V�F�[�_�����[�U�[�V�F�[�_�Ƃ���
	�ݒ肵�����̂� OnRender() �ɓn���B



	���[�U�[�V�F�[�_�ɗv�����钸�_�錾�́A
	�EPos0
	�ETexUV0
	�EColor0

	���[�U�[�V�F�[�_�ɗv������p�����[�^�́A
	�EVIEWPORTPIXELSIZE
	�ELNRC_TONE
		UI_TONE�ANODE_TONE �Ƃ��̂ق������������H
	��Tone �̌v�Z�̓��[�U�[�V�F�[�_�ŐF�����܂�����ɍs��Ȃ���΂Ȃ�Ȃ��B
	�i��Z�����ŕ\���ł��Ȃ��̂Œ��_�錾�����ł͕s���j
	#include "lumino.fx" �Ƃ����āA�s�N�Z���V�F�[�_�� LNUI_PostPixelShader �݂����Ȋ֐���
	���[�U�[�ɌĂ�ł��炤�K�v������B

	�`�������3�B
	�E�v���~�e�B�u�̂�
	�E�g�ݍ��݃V�F�[�_���g��
	�E���[�U�[�V�F�[�_���g��

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/

#include "Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/SpriteRenderer.h>
#include "Text/TextRenderer.h"
#include "PrimitiveRenderer.h"
#include "GeometryRenderer.h"
#include "RendererImpl.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// GraphicsContext
/*		GraphicsContext �� JavaFX �̃N���X�B
*/
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext* GraphicsContext::GetContext()
{
	return GraphicsManager::GetInstance()->GetGraphicsContext();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::GraphicsContext(GraphicsManager* manager)
	: m_currentRenderer(RendererType::None)
	, m_spriteRenderer(nullptr)
	, m_textRenderer(nullptr)
{
	Renderer = manager->GetRenderer();
	m_geometryRenderer = LN_NEW GeometryRenderer();
	m_geometryRenderer->Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(m_textRenderer);
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_geometryRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Set2DRenderingMode(float minZ, float maxZ)
{
	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	Matrix proj = Matrix::Perspective2DLH((float)size.Width, (float)size.Height, minZ, maxZ);
	m_geometryRenderer->SetViewProjection(Matrix::Identity, proj, size);
	m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
	m_textRenderer->SetViewProjMatrix(proj);
	m_textRenderer->SetViewPixelSize(size);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetBrush(Brush* brush)
{
	m_geometryRenderer->SetBrush(brush);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::SetOpacity(float opacity)
{
	m_geometryRenderer->SetOpacity(opacity);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::MoveTo(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->MoveTo(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::LineTo(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->LineTo(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->BezierCurveTo(cp1, cp2, endPt, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::ClosePath()
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->ClosePath();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->DrawPoint(point, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawRectangle(const RectF& rect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->DrawRectangle(rect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->DrawEllipse(center, radius);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	TryChangeRenderingClass(RendererType::GeometryRenderer);
	m_geometryRenderer->DrawTexture(rect, texture, srcRect, color);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::DrawText(const PointF& position, const StringRef& text)
{
	TryChangeRenderingClass(RendererType::TextRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), position);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_geometryRenderer->Flush();
	m_spriteRenderer->Flush();
	m_textRenderer->Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GeometryRenderer* GraphicsContext::BeginDrawingContext()
{
	if (m_currentRenderer != RendererType::GeometryRenderer)
	{
		Flush();
		m_currentRenderer = RendererType::GeometryRenderer;
	}
	return m_geometryRenderer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer* GraphicsContext::BeginSpriteRendering()
{
	if (m_currentRenderer != RendererType::GeometryRenderer)
	{
		Flush();
		m_currentRenderer = RendererType::SpriteRenderer;
	}
	return m_spriteRenderer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GraphicsContext::TryChangeRenderingClass(RendererType dc)
{
	if (dc != m_currentRenderer)
	{
		Flush();
		m_currentRenderer = dc;
	}

}

LN_NAMESPACE_END
