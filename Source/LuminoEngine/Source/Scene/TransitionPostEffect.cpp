
#include "Internal.h"
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Scene/TransitionPostEffect.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TransitionPostEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TransitionPostEffect, PostEffect);

//------------------------------------------------------------------------------
RefPtr<TransitionPostEffect> TransitionPostEffect::Create()
{
	return NewObject<TransitionPostEffect>();
}

//------------------------------------------------------------------------------
TransitionPostEffect::TransitionPostEffect()
	: PostEffect()
	, m_freezeRequested(false)
	, m_factor(0)
{
}

//------------------------------------------------------------------------------
TransitionPostEffect::~TransitionPostEffect()
{
}

//------------------------------------------------------------------------------
void TransitionPostEffect::Initialize()
{
	PostEffect::Initialize();

	{
		static const byte_t ShaderData[] =
		{
#include "Resource/TransitionPostEffectWithoutMask.lnfx.h"
		};
		m_withoutMaskShader.shader = NewObject<Shader>(detail::EngineDomain::GetGraphicsManager(), ShaderData, LN_ARRAY_SIZE_OF(ShaderData), true);
		m_withoutMaskShader.varFactor = m_withoutMaskShader.shader->FindVariable(_T("g_Factor"));
		m_withoutMaskShader.varFreezedTexture = m_withoutMaskShader.shader->FindVariable(_T("g_FreezedTexture"));
	}

	m_factor = 1.0f;
}

//------------------------------------------------------------------------------
void TransitionPostEffect::Transition(float duration, Texture* mask, int vague)
{
	m_freezeRequested = true;
	m_factor = 0;
}

//------------------------------------------------------------------------------
void TransitionPostEffect::OnRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	m_factor += 0.016;
	if (m_factor > 1.0f) m_factor = 1.0f;


	bool refreshed = false;
	if (m_primaryTarget == nullptr)
	{
		m_primaryTarget = NewObject<RenderTargetTexture>(destination->GetSize(), 1, TextureFormat::R8G8B8X8);
		m_savedTarget = NewObject<RenderTargetTexture>(destination->GetSize(), 1, TextureFormat::R8G8B8X8);
		refreshed = true;
	}
	else if (m_primaryTarget->GetSize() != destination->GetSize())
	{
		// TODO: Resize
		LN_NOTIMPLEMENTED();
		refreshed = true;
	}


	if (m_freezeRequested)
	{
		if (refreshed)
		{
			// �N������ȂǁA�����_�����O�^�[�Q�b�g�쐬��1����`�悵�Ă��Ȃ��̂ɑJ�ڂ��悤�Ƃ��Ă���B
			// ���̂Ƃ��̓r���[�̔w�i�F���g���B

			// TODO: scoped �܂��� Blit �݂����� RT ���ڎw��� Clear
			RefPtr<RenderTargetTexture> oldTarget = context->GetRenderTarget(0);
			context->SetRenderTarget(0, m_savedTarget);
			context->Clear(ClearFlags::Color, GetOwnerLayer()->GetOwnerViewport()->GetViewBackgroundColor());
		}
		else
		{
			std::swap(m_primaryTarget, m_savedTarget);
		}

		m_freezeRequested = false;
	}


	Shader* shader = nullptr;
	if (0)
	{
		// �}�X�N�e�N�X�`���g�p
		LN_NOTIMPLEMENTED();
	}
	else
	{
		// �}�X�N�e�N�X�`���s�g�p
		m_withoutMaskShader.varFactor->SetFloat(m_factor);
		m_withoutMaskShader.varFreezedTexture->SetTexture(m_savedTarget);
		shader = m_withoutMaskShader.shader;
	}


	// TODO: scoped
	RefPtr<Shader> oldShader = context->GetShader();
	context->SetShader(shader);

	// �G�t�F�N�g�K�p�ς݂̕`�挋�ʂ�O��̕`�挋�ʂƂ��Ė���o���Ă���
	context->Blit(source, m_primaryTarget, Matrix::Identity);

	context->SetShader(oldShader);
	context->Blit(m_primaryTarget, destination, Matrix::Identity);
}

LN_NAMESPACE_END
