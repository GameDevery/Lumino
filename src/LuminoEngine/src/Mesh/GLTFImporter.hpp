﻿
#pragma once
#include "MeshImporter.hpp"
namespace tinygltf {
class Model;
class Node;
class Mesh;
class Material;
class Texture;
class Skin;
class Animation;
}

namespace ln {
class AnimationClip;
class SkinnedMeshModel;

namespace detail {

class GLTFImporter
    : public MeshImporter
{
public:
	bool m_flipZ;
	bool m_flipX;
	bool m_faceFlip = false;
	bool m_clearBoneRotation;

	GLTFImporter();
    bool onImportAsStaticMesh(MeshModel* model, const AssetPath& assetPath) override;
	bool onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath) override;

	const List<Ref<AnimationClip>> animationClips() const { return m_animationClips; }

private:
	// ファイルからのデータ読み込み用。以下、ほとんど glTF 用なので importer 側にもっていってもいいかも。必要なデータを前もって集めておいて、バッファをまとめて確保するのに使う。
	struct VertexBufferView
	{
		VertexElementType type;
		VertexElementUsage usage;
		int usageIndex;
		const void* data;
		//size_t byteOffset;
		size_t count;	// vertex count. not byte size. (byte size = count * size(type))
		size_t byteStride;
	};

	// ファイルからのデータ読み込み用
	struct SectionView
	{
		std::vector<VertexBufferView> vertexBufferViews;
		//int indexOffset;    // (unit: index number. not byte size)
		//int indexCount;     // (unit: index number. not byte size)

		const void* indexData;  // このセクション内で 0 から始まるインデックス
		int indexElementSize;	// byte size. (1, 2, 4)
		size_t indexCount;

		int materialIndex;

		PrimitiveTopology topology;
	};

	struct MeshView
	{
		std::vector<SectionView> sectionViews;
	};

	bool openGLTFModel(const AssetPath& assetPath);
	bool readCommon(MeshModel* meshModel);
    Ref<Material> readMaterial(const tinygltf::Material& material);
	bool readNode(MeshNode* coreNode, const tinygltf::Node& node);
	Ref<MeshContainer> readMesh(const tinygltf::Mesh& mesh);
	Ref<Mesh> generateMesh(const MeshView& meshView) const;
	Ref<MeshSkeleton> readSkin(const tinygltf::Skin& skin);
	Ref<Texture> loadTexture(const tinygltf::Texture& texture);
	Ref<AnimationClip> readAnimation(const tinygltf::Animation& animation) const;

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

    AssetPath m_basedir;
	std::shared_ptr<tinygltf::Model> m_model;
	MeshModel* m_meshModel;

	List<Ref<AnimationClip>> m_animationClips;
};

} // namespace detail
} // namespace ln
