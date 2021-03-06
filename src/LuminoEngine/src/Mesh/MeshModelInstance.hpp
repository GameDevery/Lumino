﻿#pragma once
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>

namespace ln {
class MeshModel;
namespace detail {
class MeshModelInstance;
class SkeletonInstance;

class BoneInstance
	: public RefObject
{
public:
	BoneInstance(SkeletonInstance* owner, int boneIndex);

	SkeletonInstance* owner() const { return m_owner; }
	MeshNode* node() const;
	const Matrix& combinedTransform() const;

private:
	SkeletonInstance* m_owner;
	int m_boneIndex;
};

class SkeletonInstance
	: public RefObject
{
public:
	SkeletonInstance(MeshModelInstance* owner, int skeletonIndex);

	MeshModelInstance* owner() const { return m_owner; }
	const Ref<MeshSkeleton>& skeletonModel() const;
	const List<Ref<BoneInstance>>& bones() const { return m_bones; }
	const Ref<Texture2D>& skinningMatricesTexture() const { return m_skinningMatricesTexture; }

	void updateSkinningMatrices();

private:
	MeshModelInstance* m_owner;
	int m_skeletonIndex;
	List<Ref<BoneInstance>> m_bones;
	Ref<Texture2D> m_skinningMatricesTexture;
};

// MeshModel の描画にかかわる動的なデータ
class MeshModelInstance
	: public RefObject
{
public:


	MeshModelInstance(MeshModel* model);

	const Ref<MeshModel>& model() const { return m_model; }
	const List<Ref<SkeletonInstance>>& skeletons() const { return m_skeletons; }

	void updateSkinningMatrices();

private:
	Ref<MeshModel> m_model;
	List<Ref<SkeletonInstance>> m_skeletons;
};

} // namespace detail
} // namespace ln
