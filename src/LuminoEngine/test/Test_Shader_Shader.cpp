﻿#include "Common.hpp"

//==============================================================================
//# Testing Shader
class Test_Shader_Shader : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Shader_Shader, IndependentSamplerState)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/IndependentSamplerState.fx"));

    auto vertexDecl1 = makeObject<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
    vertexDecl1->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);

    struct Vertex
    {
        Vector3 pos;
        Vector2 uv;
    };
    Vertex v[] = {
        { { -1, 1, 0 }, { 0, 0 }, },	// 0.5 で中央の face からサンプリングする
        { { 0, 1, 0 }, { 1, 0 }, },
        { { -1, 0, 0 }, { 0, 1 }, },
        { { 0, 0, 0 }, { 1, 1 }, },
    };
    auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto tex1 = makeObject<Texture2D>(2, 2, TextureFormat::RGBA8);
    auto bmp1 = tex1->map(MapMode::Write);
    bmp1->setPixel32(0, 0, ColorI(255, 0, 0, 255));
    bmp1->setPixel32(1, 0, ColorI(255, 0, 255, 255));
    bmp1->setPixel32(0, 1, ColorI(0, 255, 0, 255));
    bmp1->setPixel32(1, 1, ColorI(0, 0, 255, 255));

    // TODO: まだ SamplerState 直接指定をサポートしていないので Texture に対してセットする方法でテストケースだけ用意しておく。
    // 後でサポートするとき、shader1->findParameter("mySamplerState")->setSamplerState(samplerState); とかに書き換える。
    auto samplerState = makeObject<SamplerState>();
    samplerState->setFilterMode(TextureFilterMode::Linear);
    tex1->setSamplerState(samplerState);

    shader1->findParameter("_Texture")->setTexture(tex1);


    // * [ ] default
    {
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setIndexBuffer(nullptr);
		ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
        ctx->drawPrimitive(0, 2);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-IndependentSamplerState-1.png"), cbb);
    }
}


//------------------------------------------------------------------------------
//## UniformBuffer
TEST_F(Test_Shader_Shader, UniformBuffer)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/UniformBufferTest-1.fx"));

    auto vertexDecl1 = makeObject<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
    auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    // VertexShader からのみ参照されるパラメータ
    shader1->findParameter("_Color1")->setVector(Vector4(1, 0, 0, 1));

    // PixelShader からのみ参照されるパラメータ
    shader1->findParameter("_Color2")->setVector(Vector4(0, 0, 1, 1));

	auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto crp = TestEnv::renderPass();
    crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->beginRenderPass(crp);
    ctx->setVertexLayout(vertexDecl1);
    ctx->setVertexBuffer(0, vb1);
    ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
    ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    ctx->drawPrimitive(0, 1);
	ctx->endRenderPass();
	TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-UniformBuffer-1.png"), cbb);
}

//------------------------------------------------------------------------------
//## MultiTechMultiTexture
TEST_F(Test_Shader_Shader, MultiTechMultiTexture)
{
    auto shader1 = Shader::create(LN_ASSETFILE("Shader/MultiTechMultiTexture-1.fx"));

    auto vertexDecl1 = makeObject<VertexLayout>();
    vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

    Vector3 v[] = {
        { 0, 0.5, 0 },
        { 0.5, -0.25, 0 },
        { -0.5, -0.25, 0 },
    };
    auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

    auto t1 = Texture2D::create(2, 2, TextureFormat::RGBA8);
    t1->clear(Color::Red);
    shader1->findParameter("_Texture1")->setTexture(t1);

    auto t2 = Texture2D::create(2, 2, TextureFormat::RGBA8);
    t2->clear(Color::Green);
    shader1->findParameter("_Texture2")->setTexture(t2);

	// _Texture1 のみ (赤)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

		ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-1.png"), cbb);
	}

	// _Texture2 のみ (緑)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

		ctx->setShaderPass(shader1->techniques()[1]->passes()[0]);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-2.png"), cbb);
	}

	// _Texture1 + _Texture2 (黄)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        auto crp = TestEnv::renderPass();
        crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

		ctx->setShaderPass(shader1->techniques()[2]->passes()[0]);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-MultiTechMultiTexture-3.png"), cbb);
	}
}

//------------------------------------------------------------------------------
//## シェーダ側とホスト側で頂点レイアウトの過不足がある場合のテスト。必要な部分さえあれば描画は可能。
TEST_F(Test_Shader_Shader, NotProvidedVertexAttribute)
{
	auto shader1 = Shader::create(LN_ASSETFILE("Shader/NotProvidedVertexAttribute-1.fx"));

	auto vertexDecl1 = makeObject<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

	Vector3 v[] = {
		{ 0, 0.5, 0 },
		{ 0.5, -0.25, 0 },
		{ -0.5, -0.25, 0 },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
    auto crp = TestEnv::renderPass();
    crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->beginRenderPass(crp);
	ctx->setVertexLayout(vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
	ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	ctx->drawPrimitive(0, 1);
	ctx->endRenderPass();
	TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Shader/Result/Test_Shader_Shader-NotProvidedVertexAttribute-1.png"), cbb);
}
