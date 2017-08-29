﻿/*
	Unityのパーティクルでエフェクトを演出
	http://qiita.com/yando/items/b6b5c6922b312f1afbd1

	[UE4] パーティクル システムのリファレンス
	https://docs.unrealengine.com/latest/JPN/Engine/Rendering/ParticleSystems/Reference/index.html
*/
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Rendering/RenderingContext.h>
#include <Lumino/Mesh/Mesh.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Particle.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// ParticleData
//==============================================================================
namespace detail {

void ParticleData::makeTrailPointData(const ParticleData& src, float currentTime, float trailTime)
{
	position = src.position;

	startPosition = src.startPosition;
	positionVelocity = src.positionVelocity;
	positionAccel = src.positionAccel;

	m_axis = src.positionAccel;
	m_angle = src.m_angle;
	m_angleVelocity = src.m_angleVelocity;
	m_angleAccel = src.m_angleAccel;
	m_forwardPosition = src.m_forwardPosition;
	m_forwardVelocity = src.m_forwardVelocity;
	m_forwardAccel = src.m_forwardAccel;

	size = src.size;
	sizeVelocity = src.sizeVelocity;
	sizeAccel = src.sizeAccel;

	rotation = src.rotation;
	color = src.color;
	colorVelocity.set(0, 0, 0, -(color.a / trailTime));	// 現在の a 値から、trailTime かけて 0 にしたい

	spawnTime = currentTime;
	endTime = currentTime + trailTime;
	lastTime = spawnTime;
	zDistance = src.zDistance;			// Zソート用作業変数
	ramdomBaseValue = src.ramdomBaseValue;
	currentDirection = src.currentDirection;

	m_isTrailPoint = true;
}

} // namespace detail

//==============================================================================
// SpriteParticleModelInstance
//==============================================================================
namespace detail {

//------------------------------------------------------------------------------
void SpriteParticleModelInstance::beginUpdate(float deltaTime)
{
	m_time += deltaTime;
	m_inactiveFindIndex = 0;
	m_mayActiveCount = m_activeCount;
}

//------------------------------------------------------------------------------
// 取得したものは必ず Spawn しなければならない
detail::ParticleData* SpriteParticleModelInstance::getNextFreeParticleData()
{
	bool spawned = false;
	for (; m_inactiveFindIndex < m_activeCount; ++m_inactiveFindIndex)
	{
		int idx = m_particleIndices[m_inactiveFindIndex];
		detail::ParticleData& data = m_particles[idx];

		// 今回の更新で消える ParticleData があればそこを使いまわす
		if (m_owner->m_loop)
		{
			if (data.isActive() && data.endTime < m_time)
			{
				spawned = true;
				return &data;
			}
		}
	}

	if (!spawned && m_inactiveFindIndex < m_particleIndices.getCount())
	{
		int idx = m_particleIndices[m_inactiveFindIndex];
		detail::ParticleData& data = m_particles[idx];
		m_inactiveFindIndex++;
		m_mayActiveCount++;
		return &data;
	}

	// max まで作られている。これ以上作れない
	return nullptr;
}

//------------------------------------------------------------------------------
void SpriteParticleModelInstance::spawnTrailPoint(detail::ParticleData* sourceData)
{
	detail::ParticleData* newData = getNextFreeParticleData();

	if (newData != nullptr)
	{
		newData->makeTrailPointData(*sourceData, m_time, m_owner->m_trailTime);

		// この関数 spawnTrailPoint は、更新ループ中から呼ばれる。新して欲しい数を1つ増やす。
		//m_mayActiveCount++;
	}
}


} // namespace detail

//==============================================================================
// SpriteParticleModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteParticleModel, Object);

//------------------------------------------------------------------------------
SpriteParticleModelPtr SpriteParticleModel::create()
{
	auto ptr = SpriteParticleModelPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance());
	return ptr;
}

//------------------------------------------------------------------------------
SpriteParticleModel::SpriteParticleModel()
	: m_manager(nullptr)
	, m_mesh(nullptr)
	//, m_texture(nullptr)
	, m_material(nullptr)
	, m_shapeType(ParticleEmitterShapeType::Sphere)
	, m_shapeParam(1, 1, 1)
	, m_sourceDataType(ParticleSourceDataType::Sprite)
	, m_particleDirection(ParticleDirectionType::Billboard)
	, m_spawnRate(1)
	, m_burstCount(1)
	, m_minRandomBaseValue(0.0f)
	, m_maxRandomBaseValue(1.0f)
	, m_minLifeTime(1.0f)
	, m_maxLifeTime(1.0f)
	, m_fadeInRatio(0.2f)
	, m_fadeOutRatio(0.8f)
	, m_movementType(ParticleMovementType::Physical)
	, m_minSize(1.0f)
	, m_maxSize(1.0f)
	, m_minSizeVelocity(0.0f)
	, m_maxSizeVelocity(0.0f)
	, m_minSizeAccel(0.0f)
	, m_maxSizeAccel(0.0f)
	, m_lengthScale(1.0f)
	, m_emitterDuration(1.0f)
	, m_positionRandomSource(ParticleRandomSource::Self)
	, m_velocityRandomSource(ParticleRandomSource::Self)
	, m_accelRandomSource(ParticleRandomSource::Self)
	, m_sizeRandomSource(ParticleRandomSource::Self)
	, m_sizeVelocityRandomSource(ParticleRandomSource::Self)
	, m_sizeAccelRandomSource(ParticleRandomSource::Self)
	, m_maxParticles(100)
	, m_loop(true)
	, m_trailType(ParticlTrailType::None)
	, m_trailTime(0.0f)
	, m_oneSpawnDeltaTime(0)
{
}

//------------------------------------------------------------------------------
SpriteParticleModel::~SpriteParticleModel()
{
}

//------------------------------------------------------------------------------
bool SpriteParticleModel::initialize(detail::GraphicsManager* manager)
{
	LN_BASE_INITIALIZE(Object);
	m_manager = manager;
	return true;
}

//------------------------------------------------------------------------------
//void SpriteParticleModel::setTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_texture, texture);
//}

//------------------------------------------------------------------------------
void SpriteParticleModel::setMaterial(Material* material)
{
	m_material = material;
}

//------------------------------------------------------------------------------
Material* SpriteParticleModel::getMaterial() const
{
	return m_material;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::SetSubParticle(SpriteParticleModel* particle)
{
	if (LN_CHECK_ARG(particle != this)) return;	// this は NG。無限再帰する
	m_childModel = particle;
	m_sourceDataType = ParticleSourceDataType::Particle;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::commit()
{
	if (m_mesh != nullptr) return;	// commit済み

	// 1 つ放出する最小時間
	m_oneSpawnDeltaTime = 1.0f / m_spawnRate;

	// 瞬間最大パーティクル数
	//m_maxParticleCount = (int)ceil(m_maxLifeTime * (float)m_spawnRate);

	m_mesh = Ref<MeshResource>::makeRef();
	m_mesh->initialize(m_manager, MeshCreationFlags::DynamicBuffers);
	m_mesh->resizeVertexBuffer(m_maxParticles * 4);
	m_mesh->resizeIndexBuffer(m_maxParticles * 6);
}

//------------------------------------------------------------------------------
Ref<detail::SpriteParticleModelInstance> SpriteParticleModel::createInstane()
{
	auto ptr = Ref<detail::SpriteParticleModelInstance>::makeRef();
	ptr->m_owner = this;
	ptr->m_particles.resize(m_maxParticles);
	ptr->m_particleIndices.resize(m_maxParticles);
	for (int i = 0; i < m_maxParticles; ++i)
	{
		ptr->m_particleIndices[i] = i;
	}
	return ptr;
}

//------------------------------------------------------------------------------
void SpriteParticleModel::updateInstance(detail::SpriteParticleModelInstance* instance, float deltaTime, const Matrix& emitterTransform)
{
	if (LN_CHECK_STATE(m_oneSpawnDeltaTime > 0.0f)) return;

	instance->beginUpdate(deltaTime);

	if (m_sourceDataType == ParticleSourceDataType::Particle)
	{
		int iData = 0;
		for (; iData < instance->m_activeCount; ++iData)
		{
			int idx = instance->m_particleIndices[iData];
			detail::ParticleData& data = instance->m_particles[idx];
			if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

			m_childModel->updateInstance(data.m_childInstance, deltaTime, emitterTransform);
		}
	}





	// create new particles
	{
		while (instance->m_lastSpawnTime <= instance->m_time)
		{
			detail::ParticleData* data = instance->getNextFreeParticleData();
			if (data != nullptr)
			{
				for (int i = m_burstCount; i >= 0; i--)
				{
					spawnParticle(emitterTransform, data, instance->m_lastSpawnTime);
				}
			}

			instance->m_lastSpawnTime += m_oneSpawnDeltaTime;
		}
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::spawnParticle(const Matrix& emitterTransform, detail::ParticleData* data, float spawnTime)
{
	data->ramdomBaseValue = m_rand.getFloatRange(m_minRandomBaseValue, m_maxRandomBaseValue);

	if (m_movementType == ParticleMovementType::Physical)
	{
		Vector3 localPosition = Vector3::Zero;
		Vector3 localFront = Vector3::UnitZ;

		switch (m_shapeType)
		{
		default:
		case ParticleEmitterShapeType::Sphere:
			localFront.x = makeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront.y = makeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront.z = makeRandom(data, -1.0, 1.0, ParticleRandomSource::Self);
			localFront = Vector3::safeNormalize(localFront, Vector3::UnitZ);
			break;
		case ParticleEmitterShapeType::Cone:
		{
			// まず、XZ 平面で Y+ を前方として角度制限付きの位置を求める。
			float r = makeRandom(data, 0.0f, m_shapeParam.x, ParticleRandomSource::Self);
			Vector3 vec;
			vec.y = sinf(r);	// TODO: Asm::sincos
			vec.z = cosf(r);

			// 次に、Y 軸周りの回転を行う。回転角度は 360度 ランダム。
			r = makeRandom(data, 0.0f, Math::PI * 2, ParticleRandomSource::Self);
			localFront.x = sinf(r) * vec.y;
			localFront.y = vec.z;
			localFront.z = cosf(r) * vec.y;
			break;
		}
		case ParticleEmitterShapeType::Box:
			localPosition.x = makeRandom(data, -m_shapeParam.x, m_shapeParam.x, m_positionRandomSource);
			localPosition.y = makeRandom(data, -m_shapeParam.y, m_shapeParam.y, m_positionRandomSource);
			localPosition.z = makeRandom(data, -m_shapeParam.z, m_shapeParam.z, m_positionRandomSource);
			localFront = Vector3::UnitY;
			break;
		}

		Vector3 worldFront = Vector3::transformCoord(localFront, emitterTransform);
		data->position = localPosition + localFront * makeRandom(data, m_forwardPosition);
		data->positionVelocity = localFront * makeRandom(data, m_forwardVelocity);
		data->positionAccel = localFront * makeRandom(data, m_forwardAccel);

		data->position.transformCoord(emitterTransform);
		//TODO: 回転だけのtransformCoord
		//data->positionVelocity.transformCoord(emitterTransform);
		//data->positionAccel.transformCoord(emitterTransform);
	}
	else if (m_movementType == ParticleMovementType::Radial)
	{
		data->m_axis.x = makeRandom(data, m_axis.minValue.x, m_axis.maxValue.x, m_axis.randomSource);
		data->m_axis.y = makeRandom(data, m_axis.minValue.y, m_axis.maxValue.y, m_axis.randomSource);
		data->m_axis.z = makeRandom(data, m_axis.minValue.z, m_axis.maxValue.z, m_axis.randomSource);

		Vector3 yaxis = data->m_axis;
		Vector3 xaxis = Vector3::cross(Vector3::UnitY, yaxis);
		Vector3 zaxis = Vector3::cross(xaxis, yaxis);




		data->m_angle = makeRandom(data, m_angle.minValue, m_angle.maxValue, m_angle.randomSource);
		data->m_angleVelocity = makeRandom(data, m_angleVelocity.minValue, m_angleVelocity.maxValue, m_angleVelocity.randomSource);
		data->m_angleAccel = makeRandom(data, m_angleAccel.minValue, m_angleAccel.maxValue, m_angleAccel.randomSource);
		data->m_forwardPosition = makeRandom(data, m_forwardPosition.minValue, m_forwardPosition.maxValue, m_forwardPosition.randomSource);
		data->m_forwardVelocity = makeRandom(data, m_forwardVelocity.minValue, m_forwardVelocity.maxValue, m_forwardVelocity.randomSource);
		data->m_forwardAccel = makeRandom(data, m_forwardAccel.minValue, m_forwardAccel.maxValue, m_forwardAccel.randomSource);


		data->position = zaxis * data->m_forwardPosition;
		data->positionVelocity = zaxis * data->m_forwardVelocity;
		data->positionAccel = zaxis * data->m_forwardAccel;
	}
	else
	{
		LN_UNREACHABLE();
	}

	data->startPosition = data->position;
	data->spawnTime = spawnTime;
	data->lastTime = spawnTime;
	data->endTime = data->spawnTime + m_maxLifeTime;	// TODO: Rand

	data->size = makeRandom(data, m_minSize, m_maxSize, m_sizeRandomSource);
	data->sizeVelocity = makeRandom(data, m_minSizeVelocity, m_maxSizeVelocity, m_sizeVelocityRandomSource);
	data->sizeAccel = makeRandom(data, m_minSizeAccel, m_maxSizeAccel, m_sizeAccelRandomSource);

	//data->currentDirection = Vector3::safeNormalize(data->position - prevPos, data->positionVelocity);

	// TODO
	data->color = Color::White;

	data->m_isTrailPoint = false;

	// SubParticle 作成
	if (m_sourceDataType == ParticleSourceDataType::Particle)
	{
		data->m_childInstance = m_childModel->createInstane();
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::simulateOneParticle(detail::ParticleData* data, double time, const Vector3& viewPosition, const Vector3& viewDirection, detail::SpriteParticleModelInstance* instance)
{
	float localTime = time - data->spawnTime;
	float deltaTime = time - data->lastTime;

	{
		if (data->m_isTrailPoint)
		{
			if (time >= data->endTime)
			{
				// 消去
				data->spawnTime = -1.0f;
				//data->position = Vector3::Zero;
			}
			else
			{
				float pre = data->color.a;

				data->color.a = Math::clamp01(data->color.a + (data->colorVelocity.a * deltaTime));
			}
			//data->color.a = 0.1;
		}
		else
		{
			// トレイルを残すなら更新前の data のコピーを作る
			if (data->spawnTime < data->lastTime &&
				time < data->endTime)	// 今回の更新を経てもまだ生きていること
			{
				if (m_trailType == ParticlTrailType::Point)
				{
					instance->spawnTrailPoint(data);
				}
			}


			//// TODO: この辺で newPos と pos の差からトレイルを引いたりできる

			//data->position = newPos;
			Vector3 prevPos = data->position;

			if (m_movementType == ParticleMovementType::Physical)
			{
				data->positionVelocity += data->positionAccel * deltaTime;
				data->position += data->positionVelocity * deltaTime;
			}
			else if (m_movementType == ParticleMovementType::Radial)
			{
				data->m_angleVelocity += data->m_angleAccel * deltaTime;
				data->m_angle += data->m_angleVelocity * deltaTime;

				//data->m_forwardVelocity += data->m_forwardAccel * deltaTime;
				//data->m_forwardPosition += data->m_forwardVelocity * deltaTime;
				//data->positionVelocity += data->positionAccel * deltaTime;
				//Vector3 pos = data->startPosition + data->positionVelocity * deltaTime;

				float t = time - data->spawnTime;
				Vector3 pos = data->startPosition + data->positionVelocity * t + 0.5 * data->positionAccel * t * t;

				Matrix mat = Matrix::makeRotationAxis(data->m_axis, data->m_angle) * instance->m_worldTransform;
				data->position = Vector3::transformCoord(pos, mat);
			}
			else
			{
				LN_UNREACHABLE();
			}

			data->sizeVelocity += data->sizeAccel * deltaTime;
			data->size += data->sizeVelocity * deltaTime;

			data->currentDirection = Vector3::normalize(data->position - prevPos);
			if (data->currentDirection.isNaNOrInf()) data->currentDirection = Vector3::safeNormalize(data->positionVelocity, Vector3::UnitY);
			

			if (time >= data->endTime)
			{
				data->m_childInstance = nullptr;

				if (m_loop)
				{
					data->spawnTime = -1.0f;
					//data->position = Vector3::Zero;
				}
				else
				{
					// ループ再生しない場合は、非アクティブにしないことで次の Spawn を行わないようにする。
				}
			}

			//if (data->position == Vector3::Zero)
			//{
			//	printf("");
			//}


			float a = 1.0f;

			float lifeSpan = data->endTime - data->spawnTime;
			a *= Math::clamp01(localTime / (lifeSpan * m_fadeInRatio));
			a *= Math::clamp01((data->endTime - time) / (lifeSpan * m_fadeOutRatio));
			data->color.a = a;
		}


		data->lastTime = time;

		// Z 距離は視点からの距離ではなく、視点平面からの距離でなければ正しくソートできない
		data->zDistance = Vector3::dot(data->position - viewPosition, viewDirection);
	}
}

//------------------------------------------------------------------------------
float SpriteParticleModel::makeRandom(detail::ParticleData* data, const RadomRangeValue<float>& value)
{
	return makeRandom(data, value.minValue, value.maxValue, value.randomSource);
}

//------------------------------------------------------------------------------
Vector3 SpriteParticleModel::makeRandom(detail::ParticleData* data, const RadomRangeValue<Vector3>& value)
{
	return Vector3(
		makeRandom(data, value.minValue.x, value.maxValue.x, value.randomSource),
		makeRandom(data, value.minValue.y, value.maxValue.y, value.randomSource),
		makeRandom(data, value.minValue.z, value.maxValue.z, value.randomSource));
}

//------------------------------------------------------------------------------
float SpriteParticleModel::makeRandom(detail::ParticleData* data, float minValue, float maxValue, ParticleRandomSource source)
{
	if (source == ParticleRandomSource::ByBaseValue)
	{
		return Math::lerp(minValue, maxValue, data->ramdomBaseValue);
	}
	else if (source == ParticleRandomSource::ByBaseValueInverse)
	{
		return Math::lerp(minValue, maxValue, data->ramdomBaseValue);
	}
	else
	{
		return m_rand.getFloatRange(minValue, maxValue);
	}
}

//------------------------------------------------------------------------------
void SpriteParticleModel::render(DrawList* context, detail::SpriteParticleModelInstance* instance, const Matrix& emitterTransform, const Vector3& viewPosition, const Vector3& viewDirection, const Matrix& viewInv, Material* material)
{

	// 更新処理
	for (int i = 0; i < instance->m_mayActiveCount; ++i)
	{
		int idx = instance->m_particleIndices[i];
		detail::ParticleData& data = instance->m_particles[idx];
		simulateOneParticle(&data, instance->m_time, viewPosition, viewDirection, instance);	// パーティクル1つ分のシミュレート
	}

	// Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthBackToFront
	{
	public:
		List<detail::ParticleData>* spriteList;

		bool operator()(int left, int right)
		{
			const detail::ParticleData& lsp = spriteList->getAt(left);
			const detail::ParticleData& rsp = spriteList->getAt(right);

			// どちらか一方でも非アクティブなら spawnTime の降順にする。そうすると、負値が後ろに集まる。
			if (!lsp.isActive() || !rsp.isActive())
			{
				return lsp.spawnTime > rsp.spawnTime;
			}

			// z距離降順
			return lsp.zDistance > rsp.zDistance;
		}
	};

	// ソート実施。
	// ここで非アクティブなものは std::remove のようにリストの後ろに移動し、Zソートも同時に行われる。
	// 少なくとも、前回アクティブだった数+今回の生成で増えた数をソート範囲にする。
	int sortRange = instance->m_mayActiveCount;
	SpriteCmpDepthBackToFront cmp;
	cmp.spriteList = &instance->m_particles;
	std::stable_sort(instance->m_particleIndices.begin(), instance->m_particleIndices.begin() + sortRange, cmp);


	if (sortRange > 0)
	{
		if (m_sourceDataType == ParticleSourceDataType::Sprite)
		{
			Matrix transform = viewInv;
			transform.m41 = 0.0f;
			transform.m42 = 0.0f;
			transform.m43 = 0.0f;

			// 頂点バッファ・インデックスバッファに反映して描画する
			Vertex* vb = reinterpret_cast<Vertex*>(m_mesh->getVertexBuffer(MeshResource::VB_BasicVertices)->getMappedData());
			uint16_t* ib = reinterpret_cast<uint16_t*>(m_mesh->getIndexBuffer()->getMappedData());

			//SpriteParticleVertex* vb = (SpriteParticleVertex*)m_vertexBuffer->lock()->GetData();	
			//uint16_t* ib = (uint16_t*)m_indexBuffer->lock()->GetData();
			int iData = 0;
			int count = instance->m_particleIndices.getCount();
			for (; iData < count; ++iData)
			{
				int idx = instance->m_particleIndices[iData];
				detail::ParticleData& data = instance->m_particles[idx];
				if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

				const Vector3& pos = data.position;
				float hs = data.size / 2;

				if (m_particleDirection == ParticleDirectionType::MovementDirection &&
					!data.currentDirection.isNaNOrInf() &&
					data.currentDirection != Vector3::Zero)
				{
					// 進行方向に対する右方向
					Vector3 r = Vector3::cross(Vector3::normalize(viewPosition - data.position), data.currentDirection);

					Vector3 fd = data.currentDirection * m_lengthScale;
					vb[(iData * 4) + 0].position = pos - (fd * hs) + r * hs;	// 後方右
					vb[(iData * 4) + 1].position = pos + (fd * hs) + r * hs;	// 前方右
					vb[(iData * 4) + 2].position = pos - (fd * hs) - r * hs;	// 後方左
					vb[(iData * 4) + 3].position = pos + (fd * hs) - r * hs;	// 前方左
				}
				else if (m_particleDirection == ParticleDirectionType::Horizontal)
				{
					Vector3 pp(data.position.x, 0, data.position.z);
					Vector3 pv(viewPosition.x, 0, viewPosition.z);


					Vector3 fd = Vector3::normalize(pp - pv);
					Vector3 r = Vector3::cross(Vector3::UnitY, fd);


					vb[(iData * 4) + 0].position = pos - (fd * hs) + r * hs;	// 後方右
					vb[(iData * 4) + 1].position = pos + (fd * hs) + r * hs;	// 前方右
					vb[(iData * 4) + 2].position = pos - (fd * hs) - r * hs;	// 後方左
					vb[(iData * 4) + 3].position = pos + (fd * hs) - r * hs;	// 前方左
				}
				else
				{
					// Z- 正面
					vb[(iData * 4) + 0].position.set(-hs, hs, 0.0f);	// 左上
					vb[(iData * 4) + 1].position.set(-hs, -hs, 0.0f);	// 左下
					vb[(iData * 4) + 2].position.set(hs, hs, 0.0f);		// 右上
					vb[(iData * 4) + 3].position.set(hs, -hs, 0.0f);	// 右下
					// 視点へ向ける
					vb[(iData * 4) + 0].position.transformCoord(transform);
					vb[(iData * 4) + 1].position.transformCoord(transform);
					vb[(iData * 4) + 2].position.transformCoord(transform);
					vb[(iData * 4) + 3].position.transformCoord(transform);

					vb[(iData * 4) + 0].position += pos;
					vb[(iData * 4) + 1].position += pos;
					vb[(iData * 4) + 2].position += pos;
					vb[(iData * 4) + 3].position += pos;
				}

				vb[(iData * 4) + 0].uv.set(0, 0);
				vb[(iData * 4) + 1].uv.set(0, 1);
				vb[(iData * 4) + 2].uv.set(1, 0);
				vb[(iData * 4) + 3].uv.set(1, 1);

				vb[(iData * 4) + 0].color = data.color;
				vb[(iData * 4) + 1].color = data.color;
				vb[(iData * 4) + 2].color = data.color;
				vb[(iData * 4) + 3].color = data.color;


				ib[(iData * 6) + 0] = (iData * 4) + 0;
				ib[(iData * 6) + 1] = (iData * 4) + 1;
				ib[(iData * 6) + 2] = (iData * 4) + 2;
				ib[(iData * 6) + 3] = (iData * 4) + 2;
				ib[(iData * 6) + 4] = (iData * 4) + 1;
				ib[(iData * 6) + 5] = (iData * 4) + 3;
			}
			//m_vertexBuffer->unlock();
			//m_indexBuffer->unlock();

			//LN_NOTIMPLEMENTED();
			//context->drawPrimitiveIndexed(m_vertexDeclaration, m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, iData * 2);
			instance->m_activeCount = iData;
			m_mesh->m_attributes.resize(1);
			m_mesh->m_attributes[0].PrimitiveNum = instance->m_activeCount * 2;
			context->drawMesh(m_mesh, 0, material);

		}
		else if (m_sourceDataType == ParticleSourceDataType::Particle)
		{
			int iData = 0;
			int count = instance->m_particleIndices.getCount();
			for (; iData < count; ++iData)
			{
				int idx = instance->m_particleIndices[iData];
				detail::ParticleData& data = instance->m_particles[idx];
				if (data.spawnTime < 0.0f) break;	// 非アクティブが見つかったら終了

				Matrix mat = Matrix::makeTranslation(data.position);
				m_childModel->render(context, data.m_childInstance, mat, viewPosition, viewDirection, viewInv, m_childModel->getMaterial());
				
			}
			instance->m_activeCount = iData;
		}

	}

	//instance->m_lastSpawnTime += m_oneSpawnDeltaTime * spawnCount;

}

//==============================================================================
// ParticleEmitterComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ParticleEmitterComponent, VisualComponent);

//------------------------------------------------------------------------------
ParticleEmitterComponent::ParticleEmitterComponent()
	: m_model(nullptr)
{
}

//------------------------------------------------------------------------------
ParticleEmitterComponent::~ParticleEmitterComponent()
{
}

//------------------------------------------------------------------------------
bool ParticleEmitterComponent::initialize(SpriteParticleModel* model)
{
	LN_BASE_INITIALIZE(VisualComponent);
	if (LN_CHECK_ARG(model != nullptr)) return false;

	m_model = model;
	m_model->commit();
	m_instance = m_model->createInstane();

	// TODO: なんか良くないやり方な気がする・・・	共有マテリアルは変更禁止にしたほうがいいと思う
	// TODO: main にはんえいされない
	//m_materialList->SetAt(0, m_model->getMaterial());
	//m_materialList->m_mainMaterial = m_model->getMaterial();

	m_material = newObject<Material>();
	return true;
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::onUpdateFrame(float deltaTime)
{
	m_instance->m_worldTransform = getOwnerObject()->transform.getWorldMatrix();
	m_model->updateInstance(m_instance, deltaTime, m_instance->m_worldTransform);
}

//------------------------------------------------------------------------------
void ParticleEmitterComponent::onRender2(RenderingContext* renderer)
{
	Vector3 dir = renderer->getRenderView()->m_cameraInfo.viewDirection;//->getCurrentCamera()->getDirectionInternal();
	m_model->render(
		renderer,
		m_instance,
		getOwnerObject()->transform.getWorldMatrix(),
		renderer->getRenderView()->m_cameraInfo.viewPosition,
		dir,
		Matrix::makeInverse(renderer->getRenderView()->m_cameraInfo.viewMatrix),
		m_material);
}

//==============================================================================
// ParticleEmitter3DComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ParticleEmitter3DComponent, ParticleEmitterComponent);

//------------------------------------------------------------------------------
Ref<ParticleEmitter3DComponent> ParticleEmitter3DComponent::create(SpriteParticleModel* model)
{
	auto ptr = Ref<ParticleEmitter3DComponent>::makeRef();
	ptr->initialize(model);
	//detail::EngineDomain::getDefaultSceneGraph3D()->getRootNode()->addChild(ptr);
	return ptr;

}

//------------------------------------------------------------------------------
ParticleEmitter3DComponent::ParticleEmitter3DComponent()
{
}

//------------------------------------------------------------------------------
ParticleEmitter3DComponent::~ParticleEmitter3DComponent()
{
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
