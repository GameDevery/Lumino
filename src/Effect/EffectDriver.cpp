/*
	���� Effekseer �����Ή����Ă��邪�A���㕡���̃c�[���𓯎��g�p�Ƃ��l�����
	�L���b�V���� Manager �ł͂Ȃ�������Ɏ������Ă����������s���������B
*/
#include "Internal.h"
#include "EffectDriver.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//=============================================================================
// EffectEngine
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectEngine::Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize)
{
	m_manager = manager;
	m_effectCoreCache = LN_NEW CacheManager(cacheObjectCount, cacheMemorySize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void EffectEngine::Finalize()
{
	if (m_effectCoreCache != NULL) {
		m_effectCoreCache->Finalize();
		LN_SAFE_RELEASE(m_effectCoreCache);
	}
}


//=============================================================================
// EffectInstance
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void EffectInstance::Initialize(EffectManager* manager)
//{
//	m_manager = manager;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void EffectInstance::AdvanceTime(float deltaTime)
//{
//	m_deltaTime += deltaTime;
//}

} // namespace detail
LN_NAMESPACE_END

