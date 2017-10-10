
#pragma once
#include <Lumino/Rendering/SceneRenderer.h>

LN_NAMESPACE_BEGIN
namespace detail {

class LightClusters
{
public:
	LightClusters();
	void init();
	void beginMakeClusters(const Matrix& view, const Matrix& proj, const Vector3& cameraPos, float nearClip, float farClip);
	void endMakeClusters();

private:

	// Texture2D �� 1 �s���Ƃ��ď������ނ��߁Afloat4 �̔{���T�C�Y�ł���K�v������
	struct LightInfo
	{
		Vector4 posAndRange;	// xyz=pos, w=range
		Vector4	spotDirection;	// xyz=dir, w=NotUse
		Vector4	spotAngle;		// x > 0 is spot light. x=cos(OuterCone), y=1.0/cos(ConeDifference), zw=NotUse
		Color	color;
	};

	static const int		ClusterWidth = 16;
	static const int		ClusterHeight = 16;
	static const int		ClusterDepth = 32;
	std::vector<Color32>	m_clustersData;		// TODO: Texture3D ���܂� setData �� getMappedData ���T�|�[�g���Ă��Ȃ��̂ŁB�ł���� getMappedData �ɂ��̂܂܏������݂���
	std::vector<int>		m_clustersAddCount;	// ����N���X�^�ɏ����ꂽ�f�[�^�̐�
	Ref<tr::Texture3D>		m_clustersTexture;

	static const int		MaxLights = 64;
	List<LightInfo>			m_lightInofs;		// m_lightInfoTexture �ɏ������ށBTODO: Texture2D �� float4 �������݂������ƃT�|�[�g������K�v�Ȃ��B
	Ref<Texture2D>			m_lightInfoTexture;
};

class ClusteredShadingGeometryRenderingPass
	: public RenderingPass2
{
public:
	ClusteredShadingGeometryRenderingPass();
	virtual ~ClusteredShadingGeometryRenderingPass();
	void initialize();

	virtual Shader* getDefaultShader() const override;

protected:
	virtual ShaderPass* selectShaderPass(Shader* shader) override;

private:
	Ref<Shader>		m_defaultShader;
};

class ClusteredShadingSceneRenderer
	: public SceneRenderer
{
public:
	ClusteredShadingSceneRenderer();
	virtual ~ClusteredShadingSceneRenderer();
	void initialize(GraphicsManager* manager);

protected:
	virtual void collect() override;
	virtual void prepare() override;
	virtual void onCollectLight(DynamicLightInfo* light) override;
	virtual void onShaderPassChainging(ShaderPass* pass) override;
};

} // namespace detail
LN_NAMESPACE_END

