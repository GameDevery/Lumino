
#include "Internal.h"
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/TransitionPostEffect.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// TransitionPostEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(TransitionPostEffect, PostEffect);

//------------------------------------------------------------------------------
Ref<TransitionPostEffect> TransitionPostEffect::create()
{
	return newObject<TransitionPostEffect>();
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
void TransitionPostEffect::initialize()
{
	PostEffect::initialize();

	{
		static const byte_t ShaderData[] =
		{
#include "Resource/TransitionPostEffectWithoutMask.lnfx.h"
		};
		m_withoutMaskShader.shader = newObject<Shader>(detail::EngineDomain::getGraphicsManager(), ShaderData, LN_ARRAY_SIZE_OF(ShaderData), true);
		m_withoutMaskShader.varFactor = m_withoutMaskShader.shader->findVariable(_T("g_Factor"));
		m_withoutMaskShader.varFreezedTexture = m_withoutMaskShader.shader->findVariable(_T("g_FreezedTexture"));
	}

	m_factor = 1.0f;
}

//------------------------------------------------------------------------------
void TransitionPostEffect::transition(float duration, Texture* mask, int vague)
{
	m_freezeRequested = true;
	m_factor = 0;
}

//------------------------------------------------------------------------------
void TransitionPostEffect::onRender(DrawList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
	// TODO: time
	m_factor += 0.016;
	if (m_factor > 1.0f) m_factor = 1.0f;


	bool refreshed = false;
	if (m_primaryTarget == nullptr)
	{
		m_primaryTarget = newObject<RenderTargetTexture>(destination->getSize(), 1, TextureFormat::R8G8B8X8);
		m_savedTarget = newObject<RenderTargetTexture>(destination->getSize(), 1, TextureFormat::R8G8B8X8);
		refreshed = true;
	}
	else if (m_primaryTarget->getSize() != destination->getSize())
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

			// TODO: scoped �܂��� blit �݂����� RT ���ڎw��� Clear
			Ref<RenderTargetTexture> oldTarget = context->getRenderTarget(0);
			context->setRenderTarget(0, m_savedTarget);
			context->clear(ClearFlags::Color, GetOwnerLayer()->getOwnerViewport()->getViewBackgroundColor());
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
		m_withoutMaskShader.varFactor->setFloat(m_factor);
		m_withoutMaskShader.varFreezedTexture->setTexture(m_savedTarget);
		shader = m_withoutMaskShader.shader;
	}


	// TODO: scoped
	Ref<Shader> oldShader = context->getShader();
	context->setShader(shader);

	// �G�t�F�N�g�K�p�ς݂̕`�挋�ʂ�O��̕`�挋�ʂƂ��Ė���o���Ă���
	context->blit(source, m_primaryTarget, Matrix::Identity);

	context->setShader(oldShader);
	context->blit(m_primaryTarget, destination, Matrix::Identity);
}

LN_NAMESPACE_END
