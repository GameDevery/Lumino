
#include "../Internal.h"
#include <Lumino/Graphics/BitmapPainter.h>
#include "PainterEngine.h"
#include "GraphicsHelper.h"

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
	DetachBrushData();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::Create(GraphicsManager* manager)
{
	auto* device = Helper::GetGraphicsDevice(manager);

	m_vertexBuffer.Attach(device->CreateVertexBuffer(
		PainterVertex::Elements(), PainterVertex::ElementCount, 1024, NULL, DeviceResourceUsage_Dynamic));
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		1024, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);

	//RefPtr<ByteBuffer> code(FileUtils::ReadAllBytes(_T("D:/Proj/Lumino/src/Graphics/Resource/Painter.fx")));
	ShaderCompileResult r;
	m_shader.Shader.Attach(device->CreateShader(g_Painter_fx_Data, g_Painter_fx_Len, &r));
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);
	
	m_shader.Technique = m_shader.Shader->GetTechnique(0);
	m_shader.Pass = m_shader.Technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.Shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("g_texture"));
	m_shader.varGlyphMaskSampler = m_shader.Shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varViewportSize = m_shader.Shader->GetVariableByName(_T("g_viewportSize"));

	m_renderer = device->GetRenderer();

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);


	//-----------------------------------------------------
	// �_�~�[�e�N�X�`��

	m_dummyTexture.Attach(device->CreateTexture(Size(32, 32), 1, TextureFormat_R8G8B8A8), false);
	Device::IGraphicsDevice::ScopedLockContext lock(device);
	BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();

	memset(&m_currentState.Brush, 0, sizeof(m_currentState.Brush));
	m_currentState.Opacity = 1.0f;
	m_currentState.InternalGlyphMask = m_dummyTexture;
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
void PainterEngine::Begin()
{
	//m_currentState.Brush.Type = BrushType_Unknown;
	//m_currentState.Opacity = 1.0f;
	//m_currentState.ForeColor = ColorF::Black;
	//m_currentState.InternalGlyphMask = m_dummyTexture;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::End()
{
	Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::SetBaseTransform(const Matrix& matrix)
{
}

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
void PainterEngine::SetBrush(const BrushData& data)
{
	Flush();	// �`��ݒ肪�ς��̂ł����Ńt���b�V��
	DetachBrushData();
	memcpy(&m_currentState.Brush, &data, sizeof(m_currentState.Brush));
	AttachBrushData();
	UpdateCurrentForeColor();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::SetOpacity(float opacity)
{
	Flush();	// �`��ݒ肪�ς��̂ł����Ńt���b�V��
	m_currentState.Opacity = opacity;
	UpdateCurrentForeColor();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawRectangle(const RectF& rect)
{
	// DrawGlyphRun() �ȊO�� NULL �ŌĂяo���Ă���
	SetInternalGlyphMaskTexture(NULL);

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	PainterVertex v;
	v.Color = m_currentState.ForeColor;
	v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);		v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);		v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// �E��
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// �E��
	m_vertexCache.Add(v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::Flush()
{
	if (m_indexCache.GetCount() == 0) { return; }

	Device::ITexture* srcTexture = NULL;
	if (m_currentState.Brush.Type == BrushType_Texture) {
		srcTexture = m_currentState.Brush.TextureBrush.Texture;
	}

	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	// �`�悷��
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_renderer->SetIndexBuffer(m_indexBuffer);
	m_shader.varTexture->SetTexture(srcTexture);
	m_shader.varGlyphMaskSampler->SetTexture(m_currentState.InternalGlyphMask);
	m_shader.Pass->Apply();
	m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);

	// �L���b�V���N���A
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

#if 0
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
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawFrameRectangle(const RectF& rect, float frameWidth/*, Device::ITexture* srcTexture, const Rect& srcRect_*/)
{
	Device::ITexture* srcTexture = NULL;
	Rect srcRect;
	if (m_currentState.Brush.Type == BrushType_Texture)
	{
		srcTexture = m_currentState.Brush.TextureBrush.Texture;
		srcRect = *((Rect*)m_currentState.Brush.TextureBrush.SourceRect);
	}
	else {
		// �e�N�X�`���u���V�ȊO�ŏ������Ƃ͂ł��Ȃ�
		// TODO: ���ŋ�`�������������A�Ԉ���Ă���Ƃ킩��₷������
		return;
	}

	// DrawGlyphRun() �ȊO�� NULL �ŌĂяo���Ă���
	SetInternalGlyphMaskTexture(NULL);

	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	if (srcRect.Width == INT_MAX) {
		srcRect.Width = srcTexture->GetSize().Width;
	}
	if (srcRect.Height == INT_MAX) {
		srcRect.Height = srcTexture->GetSize().Height;
	}

	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSize((float)srcTexture->GetRealSize().Width, (float)srcTexture->GetRealSize().Height);
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
		frameWidthHI = (int)ceil( ratio * frameWidthHI);
	}
	// �c�������������߁A�g�������߂Ȃ���΂Ȃ�Ȃ�
	if (rect.Height < frameWidthV * 2)
	{
		float ratio = rect.Height / (frameWidthV * 2);	// ���̕����牽 % �ɂȂ邩
		frameWidthV *= ratio;
		uvFrameWidthV *= ratio;
		frameWidthVI = (int)ceil(ratio * frameWidthVI);
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

	//m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	//m_renderer->SetVertexBuffer(m_vertexBuffer);
	//m_renderer->SetIndexBuffer(m_indexBuffer);
	//m_shader.varTexture->SetTexture(srcTexture);
	//m_shader.Pass->Apply();
	//m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 16);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Device::ITexture* glyphsTexture, Device::ITexture* strokesTexture/*, const ColorF& foreColor, const ColorF& strokeColor*/)
{
	SetInternalGlyphMaskTexture(glyphsTexture);

	SizeF texSizeInv(1.0f / glyphsTexture->GetRealSize().Width, 1.0f / glyphsTexture->GetRealSize().Height);
	for (int i = 0; i < dataCount; ++i)
	{
		RectF uvSrcRect = dataList[i].SrcPixelRect;
		uvSrcRect.X *= texSizeInv.Width;
		uvSrcRect.Width *= texSizeInv.Width;
		uvSrcRect.Y *= texSizeInv.Height;
		uvSrcRect.Height *= texSizeInv.Height;

		RectF dstRect(dataList[i].Position, dataList[i].SrcPixelRect.GetSize());
		dstRect.X += position.X;
		dstRect.Y += position.Y;
		InternalDrawRectangleStretch(dstRect, uvSrcRect);
	}

	//m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	//m_renderer->SetVertexBuffer(m_vertexBuffer);
	//m_renderer->SetIndexBuffer(m_indexBuffer);
	//m_shader.varTexture->SetTexture(glyphsTexture);
	//m_shader.Pass->Apply();
	//m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
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
	v.Color = m_currentState.ForeColor;
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
	v.Color = m_currentState.ForeColor;
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1);	// �E��
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1.0f + blockCountH);	// ����
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// �E��
	m_vertexCache.Add(v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::AttachBrushData()
{
	m_currentState.ForeColor = ColorF::White;
	if (m_currentState.Brush.Type == BrushType_SolidColor) {
		m_currentState.ForeColor.R = m_currentState.Brush.SolidColorBrush.Color[0];
		m_currentState.ForeColor.G = m_currentState.Brush.SolidColorBrush.Color[1];
		m_currentState.ForeColor.B = m_currentState.Brush.SolidColorBrush.Color[2];
		m_currentState.ForeColor.A = m_currentState.Brush.SolidColorBrush.Color[3];
	}
	else if (m_currentState.Brush.Type == BrushType_Texture) {
		LN_SAFE_ADDREF(m_currentState.Brush.TextureBrush.Texture);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DetachBrushData()
{
	if (m_currentState.Brush.Type == BrushType_Texture) {
		LN_SAFE_RELEASE(m_currentState.Brush.TextureBrush.Texture);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::SetInternalGlyphMaskTexture(Device::ITexture* mask)
{
	if (m_currentState.InternalGlyphMask.GetObjectPtr() != mask) 
	{
		Flush();
		if (mask != NULL) {
			m_currentState.InternalGlyphMask = mask;
		}
		else {
			m_currentState.InternalGlyphMask = m_dummyTexture;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::UpdateCurrentForeColor()
{
	if (m_currentState.Brush.Type == BrushType_SolidColor)
	{
		m_currentState.ForeColor.A = m_currentState.Opacity * m_currentState.Brush.SolidColorBrush.Color[3];
	}
	else {
		m_currentState.ForeColor.A = m_currentState.Opacity;
	}
}

} // namespace Graphics
} // namespace Lumino
