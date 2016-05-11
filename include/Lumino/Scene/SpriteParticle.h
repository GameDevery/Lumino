
#pragma once
#include <Lumino/Math/Random.h>
#include "VisualNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

namespace detail
{

struct ParticleData
{
	Vector3		position;


	Vector3		startPosition;
	Vector3		velocity;
	Vector3		acceleration;

	Quaternion	rotation;
	ColorF		color;
	float		size = 0.f;
	float		spawnTime = -1.f;	// 負値の場合は非アクティブ
	float		endTime = 0.f;
	float		lastTime = 0.0f;
	float		zDistance;			// Zソート用作業変数
};

struct SpriteParticleModelInstance
{
	//RefPtr<SpriteParticleModel>	m_owner;
	Array<ParticleData>			m_particles;
	Array<int>					m_particleIndices;
	int							m_activeCount = 0;
	double						m_time = 0;
	double						m_lastSpawnTime = 0;	// 最後に放出した時間 (m_oneSpawnDeltaTime の倍数になる)
};

} // namespace detail

class SpriteParticleModel;
using SpriteParticleModelPtr = RefPtr<SpriteParticleModel>;

/**
	@brief
*/
class SpriteParticleModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static SpriteParticleModelPtr Create();

	void SetTexture(Texture* texture);
	Texture* GetTexture() const { return m_texture; }

protected:
	SpriteParticleModel();
	virtual ~SpriteParticleModel();
	void Initialize(GraphicsManager* manager);

LN_INTERNAL_ACCESS:
	void Commit();
	std::shared_ptr<detail::SpriteParticleModelInstance> CreateInstane();
	void UpdateInstance(std::shared_ptr<detail::SpriteParticleModelInstance>& instance, float deltaTime);
	void SpawnParticle(detail::ParticleData* data, float spawnTime);
	void UpdateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition);
	void Render(RenderingContext* context, std::shared_ptr<detail::SpriteParticleModelInstance>& instance, const Vector3& viewPosition);

public:	// TODO:
	float MakeRandom(detail::ParticleData* data, float minValue, float maxValue);
	
	GraphicsManager*	m_manager;
	VertexBuffer*		m_vertexBuffer;	// TODO: このあたりは Manager に置いて、全体で共有した方がメモリ効率よい
	IndexBuffer*		m_indexBuffer;
	Texture*			m_texture;
	Randomizer			m_rand;

	int			m_spawnRate;	// 1秒間に放出するパーティクル数
	Vector3		m_minPosition;
	Vector3		m_maxPosition;
	Vector3		m_minVelocity;
	Vector3		m_maxVelocity;
	Vector3		m_minAcceleration;
	Vector3		m_maxAcceleration;

	
	float		m_lifeTimeMin;
	float		m_lifeTimeMax;

	float		m_fadeInRatio;
	float		m_fadeOutRatio;

	float		m_emitterDuration;		// エミッタのパーティクル放出時間

	int			m_maxParticleCount;
	float		m_oneSpawnDeltaTime;

};


class SpriteParticle;
using SpriteParticlePtr = RefPtr<SpriteParticle>;

/**
	@brief
*/
class SpriteParticle
	: public VisualNode
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static SpriteParticlePtr Create3D(SpriteParticleModel* model);

protected:
	SpriteParticle();
	virtual ~SpriteParticle();
	void Initialize(SceneGraph* owner, SpriteParticleModel* model);

	virtual void OnUpdateFrame(float deltaTime) override;
	virtual void DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex) override;

private:
	SpriteParticleModel*					m_model;
	std::shared_ptr<detail::SpriteParticleModelInstance>	m_instance;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
