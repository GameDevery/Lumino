
#include "../Internal.h"
#include <Lumino/Imaging/BitmapPainter.h>
#include "PainterEngine.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// PainterEngine
//=============================================================================

static const byte_t g_Painter_fx_Data[] =
{
#include "Resource/Painter.fx.h"
};
static const size_t g_Painter_fx_Len = LN_ARRAY_SIZE_OF(g_Painter_fx_Data);
	

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PainterEngine::PainterEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PainterEngine::~PainterEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::Create(GraphicsManager* manager)
{
	m_vertexBuffer.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateVertexBuffer(
		PainterVertex::Elements(), PainterVertex::ElementCount, 1024, NULL, DeviceResourceUsage_Dynamic));
	m_indexBuffer.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateIndexBuffer(
		1024, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);

	//RefPtr<ByteBuffer> code(FileUtils::ReadAllBytes(_T("D:/Proj/Lumino/src/Graphics/Resource/Painter.fx")));
	ShaderCompileResult r;
	m_shader.Shader.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateShader(g_Painter_fx_Data, g_Painter_fx_Len, &r));
	m_shader.Technique = m_shader.Shader->GetTechnique(0);
	m_shader.Pass = m_shader.Technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.Shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("g_texture"));
	m_shader.varViewportSize = m_shader.Shader->GetVariableByName(_T("g_viewportSize"));

	m_renderer = manager->GetGraphicsDevice()->GetDeviceObject()->GetRenderer();

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);


	//-----------------------------------------------------
	// �_�~�[�e�N�X�`��

	m_dummyTexture.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateTexture(Size(32, 32), 1, TextureFormat_R8G8B8A8), false);
	Device::IGraphicsDevice::ScopedLockContext lock(manager->GetGraphicsDevice()->GetDeviceObject());
	Imaging::BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void PainterEngine::SetTransform(const Matrix& matrix)
//{
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::SetViewProjMatrix(const Matrix& matrix)
{
	m_shader.varViewProjMatrix->SetMatrix(matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void PainterEngine::SetViewPixelSize(const Size& size)
//{
//	if (m_shader.varViewportSize != NULL) {
//		m_shader.varViewportSize->SetVector(Vector4((float)size.Width, (float)size.Height, 0, 0));
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawFillRectangle(const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode)
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSizeInv(1.0f / srcTexture->GetSize().Width, 1.0f / srcTexture->GetSize().Height);
	RectF uvSrcRect(srcRect);
	uvSrcRect.X			*= texSizeInv.Width;
	uvSrcRect.Width		*= texSizeInv.Width;
	uvSrcRect.Y			*= texSizeInv.Height;
	uvSrcRect.Height	*= texSizeInv.Height;

	// �g�k���[�h
	if (wrapMode == BrushWrapMode_Stretch) 
	{
		InternalDrawRectangleStretch(rect, uvSrcRect);

		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_renderer->SetVertexBuffer(m_vertexBuffer);
		m_shader.varTexture->SetTexture(srcTexture);
		m_shader.Pass->Apply();
		m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
	}
	// �^�C�����O���[�h
	else if (wrapMode == BrushWrapMode_Tile) {
		LN_THROW(0, NotImplementedException);
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawFrameRectangle(const RectF& rect, float frameWidth, Device::ITexture* srcTexture, const Rect& srcRect_)
{
	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	Rect srcRect = srcRect_;
	if (srcRect.Width == INT_MAX) {
		srcRect.Width = srcTexture->GetSize().Width;
	}
	if (srcRect.Height == INT_MAX) {
		srcRect.Height = srcTexture->GetSize().Height;
	}

	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSize(srcTexture->GetSize().Width, srcTexture->GetSize().Height);
	texSize.Width = 1.0f / texSize.Width;
	texSize.Height = 1.0f / texSize.Height;
	RectF uvSrcRect(srcRect.X * texSize.Width, srcRect.Y * texSize.Height, srcRect.Width * texSize.Width, srcRect.Height * texSize.Height);
	

	float frameWidthH = frameWidth;
	float frameWidthV = frameWidth;
	float uvFrameWidthH = frameWidth * texSize.Width;
	float uvFrameWidthV = frameWidth * texSize.Height;
	int frameWidthHI = (int)frameWidth;	// �^�ϊ��񐔂����炷���߁A���炩���� int �����Ă���
	int frameWidthVI = (int)frameWidth;

	// ���������������߁A�g�������߂Ȃ���΂Ȃ�Ȃ�
	if (rect.Width < frameWidthH * 2)
	{
		float ratio = rect.Width / (frameWidthH * 2);	// ���̕����牽 % �ɂȂ邩
		frameWidthH *= ratio;
		uvFrameWidthH *= ratio;
		frameWidthHI = ceil( ratio * frameWidthHI);
	}
	// �c�������������߁A�g�������߂Ȃ���΂Ȃ�Ȃ�
	if (rect.Height < frameWidthV * 2)
	{
		float ratio = rect.Height / (frameWidthV * 2);	// ���̕����牽 % �ɂȂ邩
		frameWidthV *= ratio;
		uvFrameWidthV *= ratio;
		frameWidthVI = ceil(ratio * frameWidthVI);
	}

	RectF outerRect = rect;
	RectF innerRect(outerRect.X + frameWidthH, outerRect.Y + frameWidthV, outerRect.Width - frameWidthH * 2, outerRect.Height - frameWidthV * 2);
	RectF outerUVRect = uvSrcRect;
	RectF innerUVRect(outerUVRect.X + uvFrameWidthH, outerUVRect.Y + uvFrameWidthV, outerUVRect.Width - uvFrameWidthH * 2, outerUVRect.Height - uvFrameWidthV * 2);
	Rect  outerSrcRect = srcRect;
	Rect  innerSrcRect(outerSrcRect.X + frameWidthHI, outerSrcRect.Y + frameWidthVI, outerSrcRect.Width - frameWidthHI * 2, outerSrcRect.Height - frameWidthVI * 2);

	// ����	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		outerRect.GetTop(),		frameWidthH,	frameWidthV),
		Rect(outerSrcRect.GetLeft(),	outerSrcRect.GetTop(),	frameWidthHI,	frameWidthVI),
		RectF(outerUVRect.GetLeft(),	outerUVRect.GetTop(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// ��	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(innerRect.GetLeft(),		outerRect.GetTop(),		innerRect.Width,	frameWidth),
		Rect(innerSrcRect.GetLeft(),	outerSrcRect.GetTop(),	innerSrcRect.Width,	frameWidthVI),
		RectF(innerUVRect.GetLeft(),	outerUVRect.GetTop(),	innerUVRect.Width,	uvFrameWidthV),
		srcTexture);

	// �E��	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		outerRect.GetTop(),		frameWidthH,	frameWidthV),
		Rect(innerSrcRect.GetRight(),	outerSrcRect.GetTop(),	frameWidthHI,	frameWidthVI),
		RectF(innerUVRect.GetRight(),	outerUVRect.GetTop(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// �E	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		innerRect.GetTop(),		frameWidthH,		innerRect.Height),
		Rect(innerSrcRect.GetRight(),	innerSrcRect.GetTop(),	frameWidthHI,	innerSrcRect.Height),
		RectF(innerUVRect.GetRight(),	innerUVRect.GetTop(),	uvFrameWidthH,	innerUVRect.Height),
		srcTexture);

	// �E��	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		innerRect.GetBottom(),		frameWidthH,	frameWidthV),
		Rect(innerSrcRect.GetRight(),	innerSrcRect.GetBottom(),	frameWidthHI,	frameWidthVI),
		RectF(innerUVRect.GetRight(),	innerUVRect.GetBottom(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// ��	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(innerRect.GetLeft(),		innerRect.GetBottom(),		innerRect.Width,	frameWidthV),
		Rect(innerSrcRect.GetLeft(),	innerSrcRect.GetBottom(),	innerSrcRect.Width,	frameWidthVI),
		RectF(innerUVRect.GetLeft(),	innerUVRect.GetBottom(),	innerUVRect.Width,	uvFrameWidthV),
		srcTexture);

	// ����	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		innerRect.GetBottom(),		frameWidthH,	frameWidthV),
		Rect(outerSrcRect.GetLeft(),	innerSrcRect.GetBottom(),	frameWidthHI,	frameWidthVI),
		RectF(outerUVRect.GetLeft(),	innerUVRect.GetBottom(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// ��	������
	//		���@��
	//		������
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		innerRect.GetTop(),		frameWidthH,	innerRect.Height),
		Rect(outerSrcRect.GetLeft(),	innerSrcRect.GetTop(),	frameWidthHI,	innerSrcRect.Height),
		RectF(outerUVRect.GetLeft(),	innerUVRect.GetTop(),	uvFrameWidthH,	innerUVRect.Height),
		srcTexture);

	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_renderer->SetIndexBuffer(m_indexBuffer);
	m_shader.varTexture->SetTexture(srcTexture);
	m_shader.Pass->Apply();
	m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 16);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect)
{
	if (rect.IsEmpty()) { return; }		// ��`���Ԃ�Ă���̂ŏ����K�v�͂Ȃ�

	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float uvWidth = srcUVRect.Width;
	float uvHeight = srcUVRect.Height;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	PainterVertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 1);	// �E��
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 2);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 2);	// �E��
	m_vertexCache.Add(v);
}

//-----------------------------------------------------------------------------
//�@Note: srcRect �́Arect �̒��ɂ����̃^�C������ׂ��邩���v�Z���邽�߂Ɏg�p����
//-----------------------------------------------------------------------------
void PainterEngine::InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Device::ITexture* srcTexture)
{
	if (rect.IsEmpty()) { return; }		// ��`���Ԃ�Ă���̂ŏ����K�v�͂Ȃ�

	float uvX = srcUVRect.X;
	float uvY = srcUVRect.Y;
	float uvWidth = srcUVRect.Width;
	float uvHeight = srcUVRect.Height;
	float blockCountW = (rect.Width / srcRect.Width);	// �������ɂ����̃^�C������ׂ��邩 (�[�����܂�)
	float blockCountH = (rect.Height / srcRect.Height);	// �c�����ɂ����̃^�C������ׂ��邩 (�[�����܂�)

	float lu = uvX;
	float ru = uvX + uvWidth * blockCountW;
	float tv = uvY;
	float bv = uvY + uvHeight * blockCountH;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	PainterVertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1);	// �E��
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1.0f + blockCountH);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// �E��
	m_vertexCache.Add(v);
}

} // namespace Graphics
} // namespace Lumino
