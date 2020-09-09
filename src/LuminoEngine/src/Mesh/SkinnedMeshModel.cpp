﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "PmxImporter.hpp"	// TODO: 依存したくない
#include "MeshManager.hpp"
#include "CCDIKSolver.hpp"

namespace ln {

#if 0
//==============================================================================
// SkinnedMeshBone

const String& SkinnedMeshBone::name() const
{
	return m_data->Name;
}

void SkinnedMeshBone::postInitialize(SkinnedMeshModel* owner, int depth)
{
    m_depth = depth;

    for (SkinnedMeshBone* bone : m_children)
    {
        bone->postInitialize(owner, m_depth + 1);
    }
}

void SkinnedMeshBone::addChildBone(SkinnedMeshBone* bone)
{
    if (LN_REQUIRE(bone != nullptr)) return;
    if (LN_REQUIRE(bone->m_parent == nullptr)) return;
    m_children.add(bone);
    bone->m_parent = this;
}

void SkinnedMeshBone::updateGlobalTransform(bool hierarchical)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	//m_combinedMatrix = m_localTransform;
	// TODO: * ではなく一気に作ったほうがはやいかも
    m_combinedMatrix =
        //Matrix::makeTranslation(-m_data->OrgPosition) *
        Matrix::makeScaling(m_localTransform.scale) *
        Matrix::makeRotationQuaternion(m_localTransform.rotation) *
        Matrix::makeTranslation(m_localTransform.translation);// *
        //Matrix::makeTranslation(m_data->OrgPosition);


															 // 親からの平行移動量
	m_combinedMatrix.translate(m_data->getOffsetFromParent());

	//m_combinedMatrix =
	//	Matrix::MakeTranslation(-m_core->OrgPosition) *
	//	Matrix::makeRotationQuaternion(m_localTransform.rotation) *
	//	Matrix::MakeTranslation(m_localTransform.translation) *
	//	Matrix::MakeTranslation(m_core->OrgPosition);

	// 親行列と結合
	if (m_parent != nullptr)
	{
		m_combinedMatrix *= m_parent->getCombinedMatrix();
	}

	// 子ボーン更新
	if (hierarchical)
	{
		for (SkinnedMeshBone* bone : m_children)
		{
			bone->updateGlobalTransform(hierarchical);
		}
	}
}
#endif

//==============================================================================
// MeshBone

MeshNode* MeshBone::node() const
{
	return m_skeleton->m_model->m_nodes[m_node];
}

//==============================================================================
// MeshArmature

MeshArmature::MeshArmature()
{
}

bool MeshArmature::init(SkinnedMeshModel* model)
{
	if (!Object::init()) return false;

	m_model = model;
	return true;
}

MeshBone* MeshArmature::bone(int index) const
{
	return m_bones[index];
}

//const Matrix& MeshArmature::boneGlobalMatrix(int index) const
//{
//}

void MeshArmature::addBone(int linkNode, const Matrix& inverseInitialMatrix)
{
	auto bone = makeObject<MeshBone>();
	bone->m_skeleton = this;
	bone->m_node = linkNode;
	bone->m_inverseInitialMatrix = inverseInitialMatrix;
	m_bones.add(bone);

	if (bone->node()->parentNodeIndex() >= 0) {
		m_rootBones.add(bone);
	}
	bone->node()->m_boneNode = true;
}

void MeshArmature::updateSkinningMatrices(SkinnedMeshModel* model)
{
	if (!m_skinningMatricesTexture || m_skinningMatricesTexture->height() != m_bones.size()) {
		m_skinningMatricesTexture = makeObject<Texture2D>(4, m_bones.size(), TextureFormat::RGBA32F);
		m_skinningMatricesTexture->setResourceUsage(GraphicsResourceUsage::Dynamic);
	}

	Bitmap2D* bitmap = m_skinningMatricesTexture->map(MapMode::Write);
	Matrix* data = reinterpret_cast<Matrix*>(bitmap->data());

	const auto& nodes = model->meshNodes();
	for (int i = 0; i < m_bones.size(); i++) {
		const auto& bone = m_bones[i];
		
		// GLTF
		//data[i] = model->nodeGlobalTransform(bone->m_node) * bone->m_inverseInitialMatrix;
		// PMX
		data[i] = bone->m_inverseInitialMatrix * model->nodeGlobalTransform(bone->m_node);
		// ↑どっちが正しい？
		// Three.js のコードを見ると、GLTFLoader では読み取った inverseBindMatrices を Skeleton クラスのコンストラクタに
		// そのまま渡している。転置とかはしていない。
		// Skeleton コンストラクタは boneInverses が省略されると、bone が持っている World行列を単純に inverse して、
		// boneInverses を作っている。calculateInverses: function () {　あたり。
		// というか「逆行列をかけて元に戻す」の計算順序的には GLTF の方が正しい。

		//// TODO: test
		//if (i == 0) {
		//	data[i] = Matrix::makeRotationZ(0.3);
		//}
		//data[i] = Matrix::Identity;
	}

	m_skinningMatricesTexture->unmap();
}

//==============================================================================
// SkinnedMeshModel

Ref<SkinnedMeshModel> SkinnedMeshModel::load(const StringRef& filePath)
{
	return detail::EngineDomain::meshManager()->createSkinnedMeshModel(filePath, 1.0f);
}

SkinnedMeshModel::SkinnedMeshModel()
    : StaticMeshModel(detail::InternalMeshModelType::SkinnedMesh)
{

	std::fill(m_humanoidBoneNodeIndices.begin(), m_humanoidBoneNodeIndices.end(), -1);
}

MeshNode* SkinnedMeshModel::findHumanoidBone(HumanoidBones boneKind) const
{
	int index = humanoidBoneIndex(boneKind);
	if (index >= 0)
		return m_nodes[index];
	else
		return nullptr;
}

void SkinnedMeshModel::addSkeleton(MeshArmature* skeleton)
{
	m_skeletons.add(skeleton);
}

AnimationController* SkinnedMeshModel::animationController() const
{
	return m_animationController;
}

void SkinnedMeshModel::beginUpdate()
{
    // 全てのローカルトランスフォームをリセットする
    //		リセットしておかないと、IKで問題が出る。
    //		(IKはその時点のLocalTransformに対して処理を行うため、回転角度がどんどん増えたりする)
    //		なお、一連の更新の最後で行っているのは、アニメーションからの更新を外部で行っているため。
    // TODO: できれば一連の処理の中で必ず通るところに移動したい
	resetNodeLocalTransforms();

}

// 姿勢更新① (ユーザー更新の前に確定する必要があるもの)
//		・ボーンのグローバル行列更新
//		・剛体更新 (フレーム位置を剛体位置へ)
void SkinnedMeshModel::preUpdate()
{


	updateBoneTransformHierarchy();

#ifdef SMESH_MIG
	if (m_needResetMorph)
	{
		m_meshResources[0]->morphBase->apply(m_meshResources[0]);
	}
#endif
}

// 姿勢更新② (ユーザー更新の後、描画の前に行う必要があるもの)
//		・モーフ更新
//		・IK更新
//		・剛体更新 (剛体位置をフレーム位置へ)
//		・スキニング行列の作成
void SkinnedMeshModel::postUpdate()
{
#ifdef SMESH_MIG
	updateMorph();

#endif
	// IK 更新
	updateIK();

	updateBoneTransformHierarchy();

#ifdef SMESH_MIG
	// 付与適用
	updateBestow();

#if 1
	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->updateBeforePhysics();
	}

	if (m_physicsWorld)
	{
		m_physicsWorld->stepSimulation(0.016f);
	}

	for (detail::MmdSkinnedMeshRigidBody* body : m_rigidBodyList)
	{
		body->updateAfterPhysics();
	}
#endif
#endif

	// スキニング行列の作成
	updateSkinningMatrices();
}

void SkinnedMeshModel::updateBoneTransformHierarchy()
{
	updateNodeTransforms();

	//for (SkinnedMeshBone* bone : m_rootBoneList)
	//{
	//	bone->updateGlobalTransform(true);
	//}
}

void SkinnedMeshModel::updateIK()
{
    detail::CCDIKSolver ik;
    ik.owner = this;
	ik.m_skeleton = m_skeletons[0];
    ik.UpdateTransform();
}

void SkinnedMeshModel::updateSkinningMatrices()
{
	for (auto& skeleton : m_skeletons) {
		skeleton->updateSkinningMatrices(this);
	}
#if 0
	// スキニング行列の作成
	for (int i = 0; i < m_allBoneList.size(); i++)
	{
		/*
			初期姿勢は、スキニングしなくても同じ姿勢。
			つまり、頂点スキニングで初期姿勢にしたいときは Matrix::Identity のボーン行列を渡す。

			ボーンは最初からオフセットが入ってるけど、
			それをスキニングに適用すると姿勢が崩れてしまう。
			そのため、初期オフセットを打ち消す処理が必要。それが getInitialTranstormInv()。

			ID3DXSkinInfo::GetBoneOffsetMatrix() で
			取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) がこれにあたるものっぽい。
			サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
		*/
		m_skinningMatrices[i] = m_allBoneList[i]->getCore()->getInitialTranstormInv() * m_allBoneList[i]->getCombinedMatrix();
		//m_skinningMatrices[i] = m_allBoneList[i]->getCombinedMatrix();

		m_skinningLocalQuaternions[i] = Quaternion::makeFromRotationMatrix(m_skinningMatrices[i]);//m_allBoneList[i]->m_localTransform.rotation;//

	}

	// スキニングテクスチャ更新
	if (!m_skinningMatricesTexture.isNull())
	{
		m_skinningMatricesTexture->setMappedData(&m_skinningMatrices[0]);
		m_skinningLocalQuaternionsTexture->setMappedData(&m_skinningLocalQuaternions[0]);
	}
#endif
}


void SkinnedMeshModel::writeSkinningMatrices(Matrix* matrixesBuffer, Quaternion* localQuaternionsBuffer)
{
  //  for (int i = 0; i < m_allBoneList.size(); i++)
  //  {
		//matrixesBuffer[i] = m_allBoneList[i]->m_data->getInitialTranstormInv() * m_allBoneList[i]->getCombinedMatrix();
  //      localQuaternionsBuffer[i] = Quaternion::makeFromRotationMatrix(matrixesBuffer[i]);
  //  }
}

//int SkinnedMeshModel::getAnimationTargetElementCount() const
//{
//	return 0;//m_allBoneList.size();
//}
//
//const String& SkinnedMeshModel::getAnimationTargetElementName(int index) const
//{
//	return String::Empty;//m_allBoneList[index]->name();
//}
//
//AnimationValueType SkinnedMeshModel::getAnimationTargetElementValueType(int index) const
//{
//	return AnimationValueType::Transform;
//}
//
//void SkinnedMeshModel::setAnimationTargetElementValue(int index, const AnimationValue& value)
//{
//	//if (value.type() == AnimationValueType::Transform)
//	//{
//	//	(*m_allBoneList[index]->localTransformPtr()) = value.getTransform();
//	//}
//}

// 左手座標系状、Z+を正面としたときの、各ボーンの位置関係を検証する。
void SkinnedMeshModel::verifyHumanoidBones()
{
	updateNodeTransforms();

	struct Pair { HumanoidBones left; HumanoidBones right; };
	Pair sidePairs[] = {
		{ HumanoidBones::LeftShoulder, HumanoidBones::RightShoulder },
		{ HumanoidBones::LeftUpperArm, HumanoidBones::RightUpperArm },
		{ HumanoidBones::LeftLowerArm, HumanoidBones::RightLowerArm },
		{ HumanoidBones::LeftHand, HumanoidBones::RightHand },

		{ HumanoidBones::LeftUpperLeg, HumanoidBones::RightUpperLeg },
		{ HumanoidBones::LeftLowerLeg, HumanoidBones::RightLowerLeg },
		{ HumanoidBones::LeftFoot, HumanoidBones::RightFoot },
		{ HumanoidBones::LeftToes, HumanoidBones::RightToes },

		{ HumanoidBones::LeftThumbProximal, HumanoidBones::RightThumbProximal },
		{ HumanoidBones::LeftThumbIntermediate, HumanoidBones::RightThumbIntermediate },
		{ HumanoidBones::LeftThumbDistal, HumanoidBones::RightThumbDistal },
		{ HumanoidBones::LeftIndexProximal, HumanoidBones::RightIndexProximal },
		{ HumanoidBones::LeftIndexIntermediate, HumanoidBones::RightIndexIntermediate },
		{ HumanoidBones::LeftIndexDistal, HumanoidBones::RightIndexDistal },
		{ HumanoidBones::LeftMiddleProximal, HumanoidBones::RightMiddleProximal },
		{ HumanoidBones::LeftMiddleIntermediate, HumanoidBones::RightMiddleIntermediate },
		{ HumanoidBones::LeftMiddleDistal, HumanoidBones::RightMiddleDistal },
		{ HumanoidBones::LeftRingProximal, HumanoidBones::RightRingProximal },
		{ HumanoidBones::LeftRingIntermediate, HumanoidBones::RightRingIntermediate },
		{ HumanoidBones::LeftRingDistal, HumanoidBones::RightRingDistal },
		{ HumanoidBones::LeftLittleProximal, HumanoidBones::RightLittleProximal },
		{ HumanoidBones::LeftLittleIntermediate, HumanoidBones::RightLittleIntermediate },
		{ HumanoidBones::LeftLittleDistal, HumanoidBones::RightLittleDistal },
	};

	for (const auto& pair : sidePairs) {
		auto* left = findHumanoidBone(pair.left);
		auto* right = findHumanoidBone(pair.right);
		if (left && right) {
			const auto& leftPos = left->globalMatrix().position();
			const auto& rightPos = right->globalMatrix().position();
			if (leftPos.x <= rightPos.x) {
				// OK
			}
			else {
				LN_WARNING("VerifyHumanoidBones error.");
			}
		}
	}
}



//==============================================================================
// AnimationController

AnimationController::AnimationController()
{
}

bool AnimationController::init(SkinnedMeshModel* model)
{
	if (!Object::init()) return false;
	m_model = model;

	m_core = makeObject<AnimationMixerCore>(this);
	m_core->addLayer(makeObject<AnimationLayer>(m_core));
	return true;
}

void AnimationController::advanceTime(float elapsedTime)
{
	m_core->advanceTime(elapsedTime);
}

detail::AnimationTargetElementBlendLink* AnimationController::onRequireBinidng(const AnimationTrackTargetKey& key)
{
	auto tb = m_bindings.findIf([&](const auto& x) { return AnimationTrackTargetKey::equals(x->key, key); });
	if (tb) {
		return *tb;
	}

	int index = -1;
	
	// まず HumanoidBones を検索
	if (key.bone != HumanoidBones::None) {
		index = m_model->humanoidBoneIndex(key.bone);
		if (index >= 0) {
			std::cout << "map:" << (int)key.bone << std::endl;
		}
	}

	// 無ければ名前検索
	if (index < 0) {
		index = m_model->findNodeIndex(key.name);
	}

	if (index >= 0) {
		auto binding = makeRef<detail::AnimationTargetElementBlendLink>(AnimationValueType::Transform);
		binding->key = key;
		binding->targetIndex = index;
		m_bindings.add(binding);
		return binding;
	}
	else {
		return nullptr;
	}
}

void AnimationController::onUpdateTargetElement(const detail::AnimationTargetElementBlendLink* binding)
{

	m_model->meshNodes()[binding->targetIndex]->setTransform(binding->rootValue.getTransform());

	//auto& s = binding->rootValue.getTransform().scale;
	//if (s.x != 1 || s.y != 1 || s.z != 1) {
	//	printf("");
	//}

	//if (binding->name == u"左腕") {
	//	m_model->meshNodes()[binding->targetIndex]->resetLocalTransform();
	//	//std::cout << binding->name << std::endl;
	//}
	//if (binding->name == u"左腕") {
	//	Matrix m = Matrix::makeRotationQuaternion(binding->rootValue.getTransform().rotation);
	//	auto v = Vector3::transformCoord(Vector3::UnitZ, m);
	//	printf("%f\t%f\t%f\n", v.x, v.y, v.z);
	//	//result.rotation = Quaternion::makeFromEulerAngles(Vector3(0, 0, time));
	//}

}

//==============================================================================
// MeshDiag

void MeshDiag::printNodes(const SkinnedMeshModel* model)
{
	bool hasRotationOrScale = false;

	for (const auto& node : model->m_nodes) {
		std::cout << node->name() << std::endl;
		const auto& t = node->initialLocalTransform();
		printf("  %f, %f, %f, %f\n", t.m[0][0], t.m[0][1], t.m[0][2], t.m[0][3]);
		printf("  %f, %f, %f, %f\n", t.m[1][0], t.m[1][1], t.m[1][2], t.m[1][3]);
		printf("  %f, %f, %f, %f\n", t.m[2][0], t.m[2][1], t.m[2][2], t.m[2][3]);
		printf("  %f, %f, %f, %f\n", t.m[3][0], t.m[3][1], t.m[3][2], t.m[3][3]);

		

		if (!Math::nearEqual(t.m[0][0], 1.0f) || !Math::nearEqual(t.m[0][1], 0.0f) || !Math::nearEqual(t.m[0][2], 0.0f) ||
			!Math::nearEqual(t.m[1][0], 0.0f) || !Math::nearEqual(t.m[1][1], 1.0f) || !Math::nearEqual(t.m[1][2], 0.0f) ||
			!Math::nearEqual(t.m[2][0], 0.0f) || !Math::nearEqual(t.m[2][1], 0.0f) || !Math::nearEqual(t.m[2][2], 1.0f)) {
			hasRotationOrScale = true;

			//auto a = Vector3::transformCoord(Vector3(0, 0, 0), t);
			//printf("");
		}
	}
	std::cout << "hasRotationOrScale: " << hasRotationOrScale << std::endl;
}

void MeshDiag::clearBoneInitialRotations(SkinnedMeshModel* model)
{
	model->updateNodeTransforms();

	if (1) {

		for (const auto& node : model->m_nodes) {
			//if (node->m_boneNode) {
				// 回転や拡縮も適用してグローバルな pos を求め、それを GlobalTransform に再設定することで位置情報のみにする
				auto pos = Vector3::transformCoord(Vector3(0, 0, 0), node->globalMatrix());

				if (1) {
					pos.x *= -1.0f;
				}

				node->setGlobalTransform(Matrix::makeTranslation(pos));
			//}
		}

		for (const auto& node : model->m_nodes) {
			//if (node->m_boneNode) {
				if (node->parentNodeIndex() >= 0) {
					// 親からの相対位置を InitialLocalTransform とする。
					// 普通は逆行列を求めるが、回転は行わなくなるので、移動ベクトルを反転するだけでよい。
					const auto& self = node->globalMatrix();
					const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
					const auto relPos = self.position() - parent.position();
					node->setInitialLocalTransform(Matrix::makeTranslation(relPos));
				}
				else {
					// 親が無ければ GlobalTransform をそのまま InitialLocalTransform とする。
					node->setInitialLocalTransform(node->globalMatrix());
				}
			//}
		}

		for (const auto& skeleton : model->skeletons()) {
			for (const auto& bone : skeleton->m_bones) {
				MeshNode* node = bone->node();
				auto t = node->globalMatrix();

				// 逆行列で inverseInitialMatrix とする。
				// 回転は行わなくなるので、移動ベクトルを反転するだけでよい。
				bone->m_inverseInitialMatrix = Matrix::makeTranslation(-t.position());
			}
		}
	}
	else {




		for (const auto& skeleton : model->skeletons()) {
			for (const auto& bone : skeleton->m_bones) {
				MeshNode* node = bone->node();
				auto t = node->globalMatrix();

				// 逆行列で InitialLocalTransform とする。
				// ただ回転は行わないので、移動ベクトルを反転するだけでよい。
				bone->m_inverseInitialMatrix = Matrix::makeTranslation(-t.position());

				//if (node->parentNodeIndex() >= 0) {
				//	// 親からの相対位置を InitialLocalTransform とする。
				//	const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
				//	const auto relPos = t.position() - parent.position();
				//	node->setInitialLocalTransform(Matrix::makeTranslation(relPos));
				//}
				//else {
				//	node->setInitialLocalTransform(Matrix::makeTranslation(t.position()));
				//}
			}
		}


		for (const auto& node : model->m_nodes) {
			if (node->m_boneNode) {
				auto t = node->globalMatrix();

				// 作業用変数として、回転成分を消して再設定
				//t.setRow(0, Vector4(1, 0, 0, 1));
				//t.setRow(1, Vector4(0, 1, 0, 1));
				//t.setRow(2, Vector4(0, 0, 1, 1));
				//node->setGlobalTransform(t);

				// 逆行列で InitialLocalTransform とする。
				// ただ回転は行わないので、移動ベクトルを反転するだけでよい。
				node->setInitialLocalTransform(Matrix::makeTranslation(-t.position()));

				if (node->parentNodeIndex() >= 0) {
					// 親からの相対位置を InitialLocalTransform とする。
					const auto& parent = model->m_nodes[node->parentNodeIndex()]->globalMatrix();
					const auto relPos = t.position() - parent.position();
					node->setInitialLocalTransform(Matrix::makeTranslation(relPos));

				}
			}
		}
	}
}


} // namespace ln

