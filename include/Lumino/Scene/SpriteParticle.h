
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
	float		spawnTime = -1.f;	// ���l�̏ꍇ�͔�A�N�e�B�u
	float		endTime = 0.f;
	float		lastTime = 0.0f;
	float		zDistance;			// Z�\�[�g�p��ƕϐ�
};

struct SpriteParticleModelInstance
{
	//RefPtr<SpriteParticleModel>	m_owner;
	Array<ParticleData>			m_particles;
	Array<int>					m_particleIndices;
	int							m_activeCount = 0;
	double						m_time = 0;
	double						m_lastSpawnTime = 0;	// �Ō�ɕ��o�������� (m_oneSpawnDeltaTime �̔{���ɂȂ�)
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
	VertexBuffer*		m_vertexBuffer;	// TODO: ���̂������ Manager �ɒu���āA�S�̂ŋ��L�������������������悢
	IndexBuffer*		m_indexBuffer;
	Texture*			m_texture;
	Randomizer			m_rand;

	////////
	int			m_spawnRate;	// 1�b�Ԃɕ��o����p�[�e�B�N����
	Vector3		m_minPosition;
	Vector3		m_maxPosition;
	Vector3		m_minVelocity;
	Vector3		m_maxVelocity;
	Vector3		m_minAccel;
	Vector3		m_maxAccel;


	//LVector3		Axis;				///< ��]��
	//LVector3		AxisRand;           ///< ��]���̃����_����

	//lnFloat			Angle;				///< �����p�x (Axis ������̉�])
	//lnFloat			AngleRand;          ///< �����p�x�̃����_����

	//lnFloat			AngleVelocity;	    ///< �p���x (Axis ������̉�])
	//lnFloat			AngleVelocityRand;  ///< �����p�x�̃����_����

	//lnFloat			AngleAccel;			///< �p�����x
	//lnFloat			AngleAccelRand;     ///< �p�����x�̃����_����

	//lnFloat			Size;				///< �T�C�Y
	//lnFloat			SizeRand;           ///< �T�C�Y�̃����_����

	//lnFloat			SizeVelocity;	    ///< �T�C�Y�ω����x
	//lnFloat			SizeVelocityRand;   ///< �T�C�Y�ω����x�̃����_����

	//lnFloat			SizeAccel;			///< �T�C�Y�ω������x
	//lnFloat			SizeAccelRand;      ///< �T�C�Y�ω������x�̃����_����
	//LVector3		GlobalAccel;		///< �S�̂ɓK�p��������x (�d�͂Ȃ�)
	//lnFloat			GravityPower;		///< ���S (���[�J�����W�� 0, 0, 0) �ւ̈��͂̋��� (���̒l�Ő˗͂ɂȂ�)

	
	float		m_lifeTimeMin;
	float		m_lifeTimeMax;

	float		m_fadeInRatio;
	float		m_fadeOutRatio;

	float		m_emitterDuration;		// �G�~�b�^�̃p�[�e�B�N�����o����
	////////

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
