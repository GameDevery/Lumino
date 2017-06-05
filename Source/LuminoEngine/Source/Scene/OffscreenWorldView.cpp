﻿
#include "Internal.h"
#include <Lumino/World.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/OffscreenWorldView.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// OffscreenWorldView
//==============================================================================
//------------------------------------------------------------------------------
OffscreenWorldView::OffscreenWorldView()
{
}

//------------------------------------------------------------------------------
OffscreenWorldView::~OffscreenWorldView()
{
}

//------------------------------------------------------------------------------
void OffscreenWorldView::Initialize()
{
	Object::Initialize();
	m_cameraInfo = NewObject<CameraComponent>(CameraProjection_3D);	// TODO: proj 指定方法
}

//------------------------------------------------------------------------------
CameraComponent* OffscreenWorldView::GetCamera() const
{
	return m_cameraInfo;
}

//------------------------------------------------------------------------------
RenderTargetTexture* OffscreenWorldView::GetRenderTarget() const
{
	return m_renderTarget;
}

//------------------------------------------------------------------------------
void OffscreenWorldView::RenderWorld(World* world)
{
	DrawList* r = world->GetRenderer();
	RenderTargetTexture* backbuffer = r->GetRenderTarget(0);

	if (m_renderTarget == nullptr)
	{
		// TODO: size 指定
		m_renderTarget = NewObject<RenderTargetTexture>(SizeI(640, 480)/*backbuffer->GetSize()*/, 1, TextureFormat::R8G8B8X8);
	}
	//else if (m_renderTarget->GetSize() != backbuffer->GetSize())
	//{
	//	LN_NOTIMPLEMENTED();
	//}

	// TODO: 深度バッファやクリア方法の指定
	r->SetRenderTarget(0, m_renderTarget);
	//g->SetDepthBuffer(m_depthBuffer);
	r->Clear(ClearFlags::All, Color::White, 1.0f, 0);
	


	world->Render(m_cameraInfo, WorldDebugDrawFlags::None);	// TODO: debugdraw の指定



	// 戻す
	r->SetRenderTarget(0, backbuffer);
}

//==============================================================================
// MirrorComponent
//==============================================================================
//------------------------------------------------------------------------------
MirrorComponent::MirrorComponent()
{
}

//------------------------------------------------------------------------------
MirrorComponent::~MirrorComponent()
{
}

//------------------------------------------------------------------------------
void MirrorComponent::Initialize()
{
	VisualComponent::Initialize();

	// TODO: この辺の初期化は OnParentChanged() のようなコールバックで行う。
	// そうすれば、アタッチされた任意のワールドへ追加できる。
	m_offscreen = NewObject<OffscreenWorldView>();

	// TODO: Remove
	detail::EngineDomain::GetDefaultWorld3D()->AddOffscreenWorldView(m_offscreen);

	m_material = NewObject<Material>();
	//m_material->SetMaterialTexture(Texture2D::GetBlackTexture());
	//m_material->SetMaterialTexture(Texture2D::GetWhiteTexture());
	m_material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
}

//------------------------------------------------------------------------------
void MirrorComponent::OnRender2(DrawList* renderer)
{
	m_material->SetMaterialTexture(m_offscreen->GetRenderTarget());
	// TODO: 法泉が入っていない？
	renderer->DrawSquare(10, 10, 1, 1, Color::White, Matrix::Identity, m_material);
}


LN_NAMESPACE_END
