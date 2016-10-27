
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
	Vector3		positionVelocity;
	Vector3		positionAccel;

	float		size;
	float		sizeVelocity;
	float		sizeAccel;

	Quaternion	rotation;
	Color		color;
	float		spawnTime = -1.f;	// ���l�̏ꍇ�͔�A�N�e�B�u
	float		endTime = 0.f;
	float		lastTime = 0.0f;
	float		zDistance;			// Z�\�[�g�p��ƕϐ�
	float		ramdomBaseValue = 0.0f;
	Vector3		currentDirection;
};

struct SpriteParticleModelInstance
{
	//RefPtr<SpriteParticleModel>	m_owner;
	List<ParticleData>			m_particles;
	List<int>					m_particleIndices;
	int							m_activeCount = 0;
	double						m_time = 0;
	double						m_lastSpawnTime = 0;	// �Ō�ɕ��o�������� (m_oneSpawnDeltaTime �̔{���ɂȂ�)
};

} // namespace detail

class SpriteParticleModel;
using SpriteParticleModelPtr = RefPtr<SpriteParticleModel>;

enum class ParticleRandomSource : uint8_t
{
	Self,
	ByBaseValue,
};


enum class ParticleDirection : uint8_t
{
	Billboard,

	/** �ړ������֌X���� (�e�N�X�`��UV�͂� V+ ������i�s�����Ƃ���) */
	MovementDirection,
};

enum class ParticleEmitterShapeType
{
	Sphere,		/** �p�[�e�B�N��������ɕ��o���܂��B*/
	Cone,		/** �p�[�e�B�N�����R�[���^�ɕ��o���܂��B*/
	Box,
};

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

	/** �����ɕ\���ł���p�[�e�B�N���̍ő吔��ݒ肵�܂��B(default: 100) */
	void SetMaxParticles(int count) { m_maxParticles = count; }

	/** 1�b�Ԃɕ��o����p�[�e�B�N���̐���ݒ肵�܂��B(default: 1) */
	void SetSpawnRate(int count) { m_spawnRate = count; }

	/** �p�[�e�B�N���̐������Ԃ�ݒ肵�܂��B(default: 1.0) */
	void SetLifeTime(float time) { m_minLifeTime = m_maxLifeTime = time; }

	// 0.0f�`1.0f
	void SetRandomBaseValueRange(float minValue, float maxValue) { m_minRandomBaseValue = minValue; m_maxRandomBaseValue = maxValue; }

	void SetPositionRange(const Vector3& minValue, const Vector3& maxValue, ParticleRandomSource source = ParticleRandomSource::Self) { m_minPosition = minValue; m_maxPosition = maxValue; m_positionRandomSource = source; }

	void SetVelocity(const Vector3& value) { m_minVelocity = m_maxVelocity = value; }
	void SetAccel(const Vector3& value) { m_minAccel = m_maxAccel = value; }

	void SetSize(float value) { m_minSize = value; m_maxSize = value; }

	void SetSizeRange(float minValue, float maxValue, ParticleRandomSource source) { m_minSize = minValue; m_maxSize = maxValue; m_sizeRandomSource = source; }

	/** �p�[�e�B�N���������Ɏg�p���闐���V�[�h��ݒ肵�܂��B(default: ���݂̎��Ԓl) */
	void SetRandomSeed(int seed) { m_rand.SetSeed(seed); }

protected:
	SpriteParticleModel();
	virtual ~SpriteParticleModel();
	void Initialize(detail::GraphicsManager* manager);

LN_INTERNAL_ACCESS:
	void Commit();
	std::shared_ptr<detail::SpriteParticleModelInstance> CreateInstane();
	void UpdateInstance(std::shared_ptr<detail::SpriteParticleModelInstance>& instance, float deltaTime);
	void SpawnParticle(detail::ParticleData* data, float spawnTime);
	void SimulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition);
	void Render(RenderingContext* context, std::shared_ptr<detail::SpriteParticleModelInstance>& instance, const Vector3& viewPosition, const Matrix& viewInv);

public:	// TODO:
	float MakeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source);
	
	detail::GraphicsManager*	m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	VertexBuffer*		m_vertexBuffer;	// TODO: ���̂������ Manager �ɒu���āA�S�̂ŋ��L�������������������悢
	IndexBuffer*		m_indexBuffer;
	Texture*			m_texture;
	Randomizer			m_rand;


	////////
	ParticleEmitterShapeType	m_shapeType;
	Vector3						m_shapeParam;

	ParticleDirection	m_particleDirection;
	int					m_spawnRate;	// 1�b�Ԃɕ��o����p�[�e�B�N����
	int					m_burstCount;	// 1�x�̕��o�^�C�~���O�Ő�������p�[�e�B�N����

	float				m_minRandomBaseValue;
	float				m_maxRandomBaseValue;

	float				m_minLifeTime;
	float				m_maxLifeTime;

	float				m_fadeInRatio;
	float				m_fadeOutRatio;


	Vector3				m_minPosition;
	Vector3				m_maxPosition;
	Vector3				m_minVelocity;
	Vector3				m_maxVelocity;
	Vector3				m_minAccel;
	Vector3				m_maxAccel;

	float				m_minSize;		// TODO: Vec2�ɂ��čג����p�[�e�B�N������肽��
	float				m_maxSize;
	float				m_minSizeVelocity;
	float				m_maxSizeVelocity;
	float				m_minSizeAccel;
	float				m_maxSizeAccel;


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

	

	float		m_emitterDuration;		// �G�~�b�^�̃p�[�e�B�N�����o����
	// TODO: 0 �̏ꍇ�͍ŏ��̃t���[���ł��������A�Ƃ��B(�ԉΗp)

	ParticleRandomSource	m_positionRandomSource;
	ParticleRandomSource	m_velocityRandomSource;
	ParticleRandomSource	m_accelRandomSource;
	ParticleRandomSource	m_sizeRandomSource;
	ParticleRandomSource	m_sizeVelocityRandomSource;
	ParticleRandomSource	m_sizeAccelRandomSource;

	int						m_maxParticles;		// ���q�ő吔

	////////

	//int			m_maxParticleCount;
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
