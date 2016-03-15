/*
	Manager �Ɏ������āA1�� Cache ����������̃r���[�����L����ƁAGC �̃^�C�~���O������Ȃ�B
	�Ȃ̂ŁA�������ʂ͂ł邯�� Viewport �P�ʂŎ��B
	�������Ă����΁AViewport::Render() �̍Ō�� GC ����Ηǂ��Ȃ�B


	�L���b�V���̗e�ʂƉ���^�C�~���O�ɂ���
		����t���[���Ŏg���Ȃ������瑦����A���Ə���������������������Ȃ��B
		�Ⴆ�Εp�ɂ� Enable��Disable ���؂�ւ�����|�X�g�G�t�F�N�g�Ƃ��B

		���̃L���b�V���̎�ȗp�r�̓|�X�g�G�t�F�N�g�ł���AViewport �Ɠ����T�C�Y��
		�_�E���L���X�g�Ɏg����B
*/
#include "../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include "RenderTargetTextureCache.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// RenderTargetTextureCache
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTargetTextureCache::RenderTargetTextureCache(GraphicsManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderTargetTextureCache::~RenderTargetTextureCache()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RefPtr<RenderTarget> RenderTargetTextureCache::RequestRenderTarget(const Size& size, TextureFormat format, int mipLevel)
{
	LN_NOTIMPLEMENTED();

	// �����L�[�����
	uint64_t w = (uint64_t)size.Width;
	uint64_t h = (uint64_t)size.Height;
	uint64_t f = (uint64_t)format;
	uint64_t m = (uint64_t)mipLevel;
	uint64_t key = m << 40 | f << 32 | h << 16 | w;
	
	// �g���郌���_�[�^�[�Q�b�g��T��
	RenderTarget* renderTarget = nullptr;
	auto itr = m_renderTargetMap.find(key);
	if (itr != m_renderTargetMap.end())
	{
		for (RenderTarget* rt : itr->second)
		{
			if (rt->GetRefCount() == 1)	// Cache ���炵���Q�Ƃ���Ă��Ȃ��H
			{
				renderTarget = rt;
				break;
			}
		}
	}

	// ������Ȃ�������V��������� map �ɒǉ�����
	if (renderTarget == nullptr)
	{
		auto rt = RefPtr<RenderTarget>::MakeRef();
		rt->CreateImpl(m_manager, size, mipLevel, format);
		renderTarget = rt.DetachMove();

		m_renderTargetMap[key].push_back(renderTarget);
	}

	// ���݂̃t���[���Ŏg�p���ꂽ���Ƃ��}�[�N����
	renderTarget->m_usedCacheOnFrame = true;

	return RefPtr<RenderTarget>(renderTarget, true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderTargetTextureCache::GCRenderTargets()
{
	LN_NOTIMPLEMENTED();
}

} // namespace detail
LN_NAMESPACE_END
