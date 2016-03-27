/*
	[2015/12/1]
		Scene �� TileMapNode ���� SpriteRenderer �g�������_�o�b�t�@�ɒ��ڏ������ނ����߂����̂ŁA
		���z�N���X�Ƃ��Ă����B
*/
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMapModel.h>

LN_NAMESPACE_BEGIN

struct TileMapVertex
{
	Vector3		Position;
	Vector2		TexUV;

	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		return elements;
	}
	static const int ElementCount = 2;
};

//=============================================================================
// TileMapRenderer
//=============================================================================

static const byte_t g_TileMapRenderer_fx_Data[] =
{
#include "TileMapRenderer.fx.h"
};
static const size_t g_TileMapRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_TileMapRenderer_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileMapRenderer::TileMapRenderer(GraphicsManager* manager)
	: m_graphicsManager(manager)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
	m_shader.shader = LN_NEW Shader();
	m_shader.shader->Initialize(m_graphicsManager, g_TileMapRenderer_fx_Data, g_TileMapRenderer_fx_Len);
	m_shader.pass = m_shader.shader->GetTechniques()[0]->GetPasses()[0];
	m_shader.varWorldMatrix = m_shader.shader->FindVariable(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.shader->FindVariable(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.shader->FindVariable(_T("g_texture"));
	//m_shader.varPixelStep = m_shader.shader->FindVariable(_T("g_pixelStep"));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileMapRenderer::~TileMapRenderer()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
	LN_SAFE_RELEASE(m_shader.shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::SetTransform(const Matrix& world, const Matrix& viewProj)
{
	m_shader.varWorldMatrix->SetMatrix(world);
	m_shader.varViewProjMatrix->SetMatrix(viewProj);
	m_viewProj = viewProj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::Draw(RenderingContext2* context, SpriteRenderer* spriteRenderer, TileMap* tileMap, const RectF& boundingRect, const ViewFrustum& cameraFrustum)
{
	LN_CHECK_ARGS_RETURN(tileMap != nullptr);
	m_renderingContext = context;
	m_spriteRenderer = spriteRenderer;

	// TODO: ���_�Ɛ��ʕ���
	Plane plane(Vector3(0, 0, 0), Vector3(0, 0, -1));
	Vector3 corners[8];
	Vector3 pt;
	cameraFrustum.GetCornerPoints(corners);

	// TileMap �̕��ʂƃJ�����̎����䂩��`�悷��ׂ��͈͂����߂�
	float l, t, r, b;
	for (int i = 0; i < 4; ++i)
	{
		plane.Intersects(corners[i], corners[i+4], &pt);
		if (i == 0)
		{
			l = pt.X;
			t = pt.Y;
			r = pt.X;
			b = pt.Y;
		}
		else
		{
			l = std::min(l, pt.X);
			r = std::max(r, pt.X);
			t = std::max(t, pt.Y);
			b = std::min(b, pt.Y);
		}
	}

	//printf("%f %f %f %f \n", l, t, r, b);

	// TODO: ���[�J�����W���^�C���}�b�v�z��C���f�b�N�X�ւ̍��W�ϊ�

	// TODO: �����܂̂Ƃ��� 3d �p
	const Size& tileSize = tileMap->GetTileSet()->GetTileSize();

	// �e�[�u���z��̂ǂ��͈̔͂�`�悵�����H
	BoundingRect renderRange =
	{
		l / tileSize.Width,
		t / tileSize.Height,
		r / tileSize.Width,
		b / tileSize.Height,
	};
	//Rect renderRange = Rect::MakeFromBounds(
	//	(int)l / tileSize.Width,
	//	(int)b / tileSize.Height,
	//	(int)r / tileSize.Width,
	//	(int)t / tileSize.Height);
	//renderRange.X = (int)l / tileSize.Width;
	//renderRange.Y = (int)t / tileSize.Height;
	//renderRange.Width = renderRange.X + ((int)r / tileSize.Width);
	//renderRange.Height = renderRange.Y - ((int)b / tileSize.Height);


	Begin();
	for (TileLayer* layer : *tileMap->GetLayers())
	{
		BoundingRect clipd = renderRange;
		// TODO: 3D �� Y0 �����[�Ƃ���
		clipd.top = layer->GetSize().Height - clipd.top;
		clipd.bottom = layer->GetSize().Height - clipd.bottom;
		//clipd.Y = layer->GetSize().Height + clipd.Y;

		// ���[�v���Ȃ�
		// TODO:
		if (1)
		{
			const Size& size = layer->GetSize();
			if (clipd.left < 0) clipd.left = 0;
			if (clipd.top < 0) clipd.top = 0;
			if (clipd.right > size.Width) clipd.right = size.Width;		// �`�悵�����Ƃ��� +1
			if (clipd.bottom > size.Height) clipd.bottom = size.Height;	// �`�悵�����Ƃ��� +1
		}


		//printf("%d %d %d %d \n", renderRange.GetLeft(), renderRange.GetTop(), renderRange.GetRight(), renderRange.GetBottom());
		//printf("%d %d %d %d \n", clipd.GetLeft(), clipd.GetTop(), clipd.GetRight(), clipd.GetBottom());

		DrawLayer(layer, boundingRect, tileMap->GetTileSet(), clipd);
	}
	End();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::Begin()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::End()
{
	m_spriteRenderer->Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::DrawTile(
	const Vector3& position,
	const Vector2& size,
	Texture* texture,
	const Rect& srcRect)
{
	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet, const BoundingRect& renderRange)
{
	LN_CHECK_ARGS_RETURN(layer != nullptr);
	LN_CHECK_ARGS_RETURN(tileSet != nullptr);

	int allocedTileCount = 0;
	if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
	{
		allocedTileCount = m_vertexBuffer->GetVertexCount() / 4;
	}

	int tileCount = (renderRange.right - renderRange.left) * (renderRange.bottom - renderRange.top);
	if (tileCount > allocedTileCount)
	{
		LN_SAFE_RELEASE(m_vertexBuffer);
		LN_SAFE_RELEASE(m_indexBuffer);
		m_vertexBuffer = LN_NEW VertexBuffer(m_graphicsManager, TileMapVertex::Elements(), TileMapVertex::ElementCount, tileCount * 4, nullptr, DeviceResourceUsage_Dynamic);
		m_indexBuffer = LN_NEW IndexBuffer(m_graphicsManager, tileCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

		// �C���f�b�N�X�o�b�t�@�͓r���ŕς��Ȃ��̂Ő�ɖ��߂Ă���
		ByteBuffer* buf = m_indexBuffer->Lock();
		uint16_t* ib = (uint16_t*)buf->GetData();
		for (int iTile = 0; iTile < tileCount; ++iTile)
		{
			int i = iTile * 6;
			int v = iTile * 4;
			ib[i + 0] = v + 0;
			ib[i + 1] = v + 1;
			ib[i + 2] = v + 2;
			ib[i + 3] = v + 2;
			ib[i + 4] = v + 1;
			ib[i + 5] = v + 3;
		}
		m_indexBuffer->Unlock();
	}






	const Size& tileSize = tileSet->GetTileSize();

	// boundingRect �S�̂𖄂߂�悤�ɕ`�悷��ׂ��Z���͈͂����߂�
	//int ox = boundingRect.X / tileSize.Width;
	//int oy = boundingRect.Y / tileSize.Height;
	//int w = (boundingRect.Width + tileSize.Width) / tileSize.Width;
	//int h = (boundingRect.Height + tileSize.Height) / tileSize.Height;

	Texture* tileSetTexture = tileSet->GetImageSource();
	SizeF invSize(
		1.0f / tileSetTexture->GetSize().Width,
		1.0f / tileSetTexture->GetSize().Height);

	Vector3 pos;
	Vector2 size;
	Texture* texture;
	Rect srcRect;
	const Size& layerSize = layer->GetSize();

	ByteBuffer* buf = m_vertexBuffer->Lock();
	TileMapVertex* vb = (TileMapVertex*)buf->GetData();

	int plotCount = 0;
	Vector3 offset(0/*layerSize.X * tileSize.Width*/, layerSize.Height * tileSize.Height, 0);
	Vector3 stepX(tileSize.Width, 0, 0);
	Vector3 stepY(0, -tileSize.Height, 0);	// 3D �Ȃ̂ŉ�������
	for (int y = renderRange.top; y < renderRange.bottom; ++y)
	{
		Vector3 pos = offset + stepY * y;
		for (int x = renderRange.left; x < renderRange.right; ++x)
		{
			//if (x < layerSize.Width && y < layerSize.Height)
			{
				tileSet->LookupTileImage(layer->GetTileId(x, y), &texture, &srcRect);
				//float pl = (float)x;
				//float pt = (float)y;
				//float pr = (float)x + tileSize.Width;
				//float pb = (float)y + tileSize.Height;
				float tl = ((float)srcRect.GetLeft()) * invSize.Width;
				float tt = ((float)srcRect.GetTop()) * invSize.Height;
				float tr = ((float)srcRect.GetRight()) * invSize.Width;
				float tb = ((float)srcRect.GetBottom()) * invSize.Height;
				//pos.Set(x * tileSize.Width, y * tileSize.Height, 0);
				//size.Set(srcRect.Width, srcRect.Height);
				//DrawTile(pos, size, texture, srcRect);
				vb[plotCount + 0].Position = pos;
				vb[plotCount + 0].TexUV.Set(tl, tt);
				vb[plotCount + 1].Position = pos + stepY;
				vb[plotCount + 1].TexUV.Set(tl, tb);
				vb[plotCount + 2].Position = pos + stepX;
				vb[plotCount + 2].TexUV.Set(tr, tt);
				vb[plotCount + 3].Position = pos + stepX + stepY;
				vb[plotCount + 3].TexUV.Set(tr, tb);
				plotCount += 4;

				//vb[plotCount + 0].Position.Set(0, 10, 0);
				//vb[plotCount + 0].TexUV.Set(tl, tt);
				//vb[plotCount + 1].Position.Set(0, 0, 0);
				//vb[plotCount + 1].TexUV.Set(tl, tb);
				//vb[plotCount + 2].Position.Set(-10, 10, 0);
				//vb[plotCount + 2].TexUV.Set(tr, tt);
				//vb[plotCount + 3].Position.Set(-10, 0, 0);
				//vb[plotCount + 3].TexUV.Set(tr, tb);
			}
			pos += stepX;
		}
	}

	//printf("---\n");
	//Vector3::Transform(vb[0].Position, m_viewProj).Print();
	//Vector3::Transform(vb[1].Position, m_viewProj).Print();
	//Vector3::Transform(vb[2].Position, m_viewProj).Print();
	//Vector3::Transform(vb[3].Position, m_viewProj).Print();

	m_vertexBuffer->Unlock();





	//m_renderingContext->Flush();


	// TODO: �����e�N�X�`��
	// (�ƌ����Ă��A���ۂ�1layer��1�e�N�X�`���ł����Ǝv���BRGSS����������Ȃ����B�ŏ��� BitBlt ��1�̃e�N�X�`���ɂ܂Ƃ߂Ă��܂��Ă���������)
	m_shader.varTexture->SetTexture(tileSetTexture);
	//m_shader.varViewProjMatrix->SetMatrix(m_viewProj);



	//RenderState s = m_renderingContext->GetRenderState();
	//s.Culling = CullingMode_None;
	//m_renderingContext->SetRenderState(s);

	m_renderingContext->SetVertexBuffer(m_vertexBuffer);
	m_renderingContext->SetIndexBuffer(m_indexBuffer);
	m_renderingContext->SetShaderPass(m_shader.pass);
	m_renderingContext->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, tileCount * 2);
	
	//m_renderingContext->Flush();
}
//
////=============================================================================
//// SpriteTileMapRenderer
////=============================================================================
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SpriteTileMapRenderer::SpriteTileMapRenderer(GraphicsManager* manager)
//	: m_spriteRenderer(nullptr)
//{
//	m_spriteRenderer = SpriteRenderer::Create(MaxSpriteCount, manager);
//	m_spriteRenderer->SetViewPixelSize(Size(640, 480));	// TODO
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SpriteTileMapRenderer::~SpriteTileMapRenderer()
//{
//	LN_SAFE_RELEASE(m_spriteRenderer);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetTransform(const Matrix& matrix)
//{
//	m_spriteRenderer->SetTransform(matrix);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
//{
//	m_spriteRenderer->SetViewProjMatrix(view, proj);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetRenderState(const RenderState& state)
//{
//	m_spriteRenderer->SetRenderState(state);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::Begin()
//{
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::End()
//{
//	m_spriteRenderer->Flush();
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::DrawTile(
//	const Vector3& position,
//	const Vector2& size,
//	Texture* texture,
//	const Rect& srcRect)
//{
//	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
//}


LN_NAMESPACE_END
