
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/Effect/Common.h>
#include "../Graphics/RenderingCommand.h"

LN_NAMESPACE_BEGIN
namespace detail
{
class EffectManager;

// Effect �̊Ǘ�
class EffectEngine
	: public RefObject
{
public:
	EffectEngine() = default;
	virtual ~EffectEngine() = default;

	void Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize);
	virtual void Finalize();
	virtual VisualEffect* CreateEffectCore(const PathName& filePath) = 0;
	virtual void UpdateFrame(float elapsedTime) = 0;
	virtual void Render() = 0;

	// Render() �́A
	// Deferred �`��		: �`��R�}���h������s�����
	// Immediate �`��	: ���ڎ��s�����

protected:
	EffectManager*	m_manager = nullptr;
	CacheManager*	m_effectCoreCache = nullptr;
};

//// �G�t�F�N�g�I�u�W�F�N�g Wrapper�B�L���b�V���Ǘ������B
//class EffectCore
//	: public RefObject
//	, public ICacheObject
//{
//	LN_CACHE_OBJECT_DECL;
//public:
//	EffectCore();
//	virtual ~EffectCore() = default;
//};
//
//// �G�t�F�N�g�C���X�^���X Wrapper
//class EffectInstance
//	: public RefObject
//{
//public:
//	EffectInstance() = default;
//	virtual ~EffectInstance() = default;
//
//	void Initialize(EffectManager* manager);
//	void SetWorldMatrix(const Matrix& matrix) { m_worldMatrix = matrix; }
//	//void AdvanceTime(float deltaTime);
//
//	//virtual void Play(bool overlap) = 0;
//	virtual void Stop() = 0;
//	virtual bool IsPlaying() = 0;
//
//	virtual void UpdateFrame() = 0;	// �X�V�X���b�h
//	virtual void Draw() = 0;	// �`��X���b�h
//
//protected:
//	EffectManager*	m_manager = nullptr;
//	Matrix			m_worldMatrix;
//	//float			m_deltaTime = 0;
//};

// �ꊇ�`��̕`��R�}���h
struct SetTransformCommand : public RenderingCommand
{
	EffectEngine*	m_engine;

	void Create(EffectEngine* engine)
	{
		m_engine = engine;
	}
	void Execute() { m_engine->Render(); }
};

} // namespace detail
LN_NAMESPACE_END
