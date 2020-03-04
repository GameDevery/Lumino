﻿#include "Common.hpp"

class Test_Graphics_LowLevelRendering : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		m_shader1 = Shader::create(LN_ASSETFILE("simple.vsh"), LN_ASSETFILE("simple.psh"));

		m_vertexDecl1 = makeObject<VertexLayout>();
		m_vertexDecl1->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	}

	virtual void TearDown() {}

	Ref<Shader> m_shader1;
	Ref<VertexLayout> m_vertexDecl1;
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, BasicTriangle)
{
	// # 時計回り (左ねじ) で描画できること
	{
		m_shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

		Vector4 v[] = { Vector4(0, 0.5, 0, 1), Vector4(0.5, -0.25, 0, 1), Vector4(-0.5, -0.25, 0, 1), };
		auto vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

        // RenderPass は Swap の数だけ作ってもいいし、1つを使いまわしても良い。
        // たくさんの RT に描画するときでも 1つだけ RenderPass を作れば良い。
        // ただし、その場合は RenderPass 切り替えのたびにキャッシュを検索する処理が入るのでパフォーマンスが悪くなる。
        // 常に RT,Depth,RenderPass をセットにして、RenderPass は生成後変更しないようにするとパフォーマンスがよくなる。
        auto renderPass = makeObject<RenderPass>();

        for (int i = 0; i < 5; i++)
        {
            auto target = TestEnv::mainWindowSwapChain()->currentBackbuffer();

            renderPass->setRenderTarget(0, target);
            renderPass->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			auto ctx = TestEnv::beginFrame();
			ctx->beginRenderPass(renderPass);
            ctx->setVertexLayout(m_vertexDecl1);
            ctx->setVertexBuffer(0, vertexBuffer);
            ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
            ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
            ctx->drawPrimitive(0, 1);
			ctx->endRenderPass();
			TestEnv::endFrame();
			ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-BasicTriangle.png"), target);
        }
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, Clear)
{
    m_shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

    Vector4 v[] = { Vector4(0, 0.5, 0, 1), Vector4(0.5, -0.25, 0, 1), Vector4(-0.5, -0.25, 0, 1), };
    auto vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		ctx->beginRenderPass(TestEnv::renderPass());
        ctx->setVertexLayout(m_vertexDecl1);
        ctx->setVertexBuffer(0, vertexBuffer);
        ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
        ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        ctx->drawPrimitive(0, 1);                           // (ちゃんと全体に clear 効くか確認 & Vulkan 警告回避のため、) 適当に描いてから
		ctx->clear(ClearFlags::All, Color::Blue, 1.0f, 0);  // clear
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Clear-1.png"), cbb);
	}

	//* [ ] Viewport や Scissor の影響を受けず、全体をクリアできること。
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		ctx->beginRenderPass(TestEnv::renderPass());
		ctx->setViewportRect(Rect(0, 0, 10, 10));
		ctx->setScissorRect(Rect(0, 0, 10, 10));
        ctx->setVertexLayout(m_vertexDecl1);
        ctx->setVertexBuffer(0, vertexBuffer);
        ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
        ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        ctx->drawPrimitive(0, 1);
		ctx->clear(ClearFlags::All, Color::Green, 1.0f, 0);
		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Clear-2.png"), cbb);
	}

	//* [ ] 複数 RT 設定時は index 0 だけクリアされること。
	{
		auto renderPass = makeObject<RenderPass>();
		auto t1 = makeObject<RenderTargetTexture>(32, 32, TextureFormat::RGBA8, false);
		auto t2 = makeObject<RenderTargetTexture>(32, 32, TextureFormat::RGBA8, false);

		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
        ctx->setVertexLayout(m_vertexDecl1);
        ctx->setVertexBuffer(0, vertexBuffer);
        ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
        ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);

		// 両方 Blue でクリアして、
		renderPass->setRenderTarget(0, t1);
		ctx->beginRenderPass(renderPass);
        ctx->drawPrimitive(0, 1);
		ctx->clear(ClearFlags::Color, Color::Blue, 1.0f, 0);
		ctx->endRenderPass();
		renderPass->setRenderTarget(0, t2);
		ctx->beginRenderPass(renderPass);
        ctx->drawPrimitive(0, 1);
		ctx->clear(ClearFlags::Color, Color::Blue, 1.0f, 0);
		ctx->endRenderPass();

		// 2つ set して Red で clear
		renderPass->setRenderTarget(0, t1);
		renderPass->setRenderTarget(1, t2);
		ctx->beginRenderPass(renderPass);
        ctx->drawPrimitive(0, 1);
		ctx->clear(ClearFlags::Color, Color::Red, 1.0f, 0);
		ctx->endRenderPass();

		TestEnv::endFrame();

		// Red, Blue
		ASSERT_EQ(true, TestEnv::equalsBitmapFile(detail::TextureInternal::readData(t1, nullptr), LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Clear-3.png"), 100));
		ASSERT_EQ(true, TestEnv::equalsBitmapFile(detail::TextureInternal::readData(t2, nullptr), LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Clear-4.png"), 100));
	}


    //* [ ] RenderPass の begin/end だけでクリアできること
    {
        auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::Blue, 1.0f, 0);
        ctx->beginRenderPass(crp);
        ctx->endRenderPass();
        TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Clear-1.png"), cbb);
    }
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, VertexBuffer)
{
	auto buffer1 = m_shader1->findConstantBuffer("ConstBuff");
	buffer1->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

	struct Param
	{
		GraphicsResourceUsage usage;
		GraphicsResourcePool pool;
	};
	std::array<Param, 3> params =
	{
		Param{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::None },
		Param{ GraphicsResourceUsage::Static, GraphicsResourcePool::Managed },
		Param{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::Managed },
		//{ GraphicsResourceUsage::Static, GraphicsResourcePool::None },
	};

	// test static and dynamic
	for (int i = 0; i < params.size(); i++)
	{
		auto usage = params[i].usage;
		auto pool = params[i].pool;

		auto vb1 = makeObject<VertexBuffer>(sizeof(Vector4) * 3, usage);
		auto vb2 = makeObject<VertexBuffer>(sizeof(Vector4) * 3, usage);
		vb1->setResourcePool(pool);
		vb2->setResourcePool(pool);


		// * [ ] まだ一度もレンダリングに使用されていないバッファを、更新できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

			Vector4 v1[] = {
				Vector4(0, 0.5, 0, 1),
				Vector4(0.5, -0.25, 0, 1),
				Vector4(-0.5, -0.25, 0, 1),
			};
			memcpy(vb1->map(MapMode::Write), v1, vb1->size());

			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->drawPrimitive(0, 1);

			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-VertexBuffer-2.png"), cbb);
		}

		// * [ ] 一度レンダリングに使用されたバッファを、再更新できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			Vector4 v2[] = {
				Vector4(0, 1, 0, 1),
				Vector4(1, -1, 0, 1),
				Vector4(-1, -1, 0, 1),
			};
			memcpy(vb1->map(MapMode::Write), v2, vb1->size());

			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->drawPrimitive(0, 1);
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-VertexBuffer-3.png"), cbb);
		}

		// * [ ] まだ一度もレンダリングに使用されていないバッファを、拡張できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			Vector4 v2[] = {
				Vector4(-0.5, 0.5, 0, 1),
				Vector4(0.5, 0.5, 0, 1),
				Vector4(-0.5, -0.5, 0, 1),
				Vector4(0.5, -0.5, 0, 1),
			};

			vb2->resize(sizeof(Vector4) * 4);
			ASSERT_EQ(sizeof(Vector4) * 4, vb2->size());

			memcpy(vb2->map(MapMode::Write), v2, vb2->size());

			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);

			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-VertexBuffer-4.png"), cbb);
		}

		// * [ ] 一度レンダリングに使用されたバッファを、拡張できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			Vector4 v2[] = {
				Vector4(-0.5, 0.5, 0, 1),
				Vector4(0.5, 0.5, 0, 1),
				Vector4(-0.5, -0.5, 0, 1),
				Vector4(0.5, -0.5, 0, 1),
				Vector4(-0.5, -1, 0, 1),
			};

			vb2->resize(sizeof(Vector4) * 5);
			ASSERT_EQ(sizeof(Vector4) * 5, vb2->size());

			memcpy(vb2->map(MapMode::Write), v2, vb2->size());

			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 3);

			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-VertexBuffer-5.png"), cbb);
		}
	}


	// TODO: 部分 lock
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, MultiStreamVertexBuffer)
{
	auto shader1 = Shader::create(LN_ASSETFILE("MultiStreamVertexBuffer-1.vsh"), LN_ASSETFILE("MultiStreamVertexBuffer-1.psh"));

	struct PosColor
	{
		Vector3 pos;
		Vector4 color;
	};
	PosColor v1[3] = { 
		{ { -1, 1, 0 }, { 0.5, 0, 0, 0 } }, 
		{ {0, 1, 0}, { 0, 1., 0, 0 } },
		{ {-1, 0, 0}, { 0, 0, 0.5, 0 } }
	};
	Vector3 uv1[3] = { { 0.5, 0, 0 },{ 0, -0.5, 0 }, { 0, 0, 0.5 } };
	Vector4 uv2[3] = { { 0, 0, 0, 1 },{ 0, 0, 0, 1 },{ 0, 0, 0, 1 } };

	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vb2 = makeObject<VertexBuffer>(sizeof(uv1), uv1, GraphicsResourceUsage::Static);
	auto vb3 = makeObject<VertexBuffer>(sizeof(uv2), uv2, GraphicsResourceUsage::Static);
	auto vd1 = makeObject<VertexLayout>();
	vd1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vd1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);
	vd1->addElement(1, VertexElementType::Float3, VertexElementUsage::TexCoord, 0);
	vd1->addElement(2, VertexElementType::Float4, VertexElementUsage::TexCoord, 1);

	auto ctx = TestEnv::beginFrame();
    auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
	auto crp = TestEnv::renderPass();
	crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->beginRenderPass(crp);
	ctx->setVertexBuffer(0, vb1);
	ctx->setVertexBuffer(1, vb2);
	ctx->setVertexBuffer(2, vb3);
	ctx->setVertexLayout(vd1);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	ctx->drawPrimitive(0, 1);

	ctx->endRenderPass();
	TestEnv::endFrame();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-MultiStreamVertexBuffer-1.png"), cbb);
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, IndexBuffer)
{
	auto buffer1 = m_shader1->findConstantBuffer("ConstBuff");
	buffer1->findParameter("g_color")->setVector(Vector4(0, 0, 1, 1));

	struct Param
	{
		GraphicsResourceUsage usage;
		GraphicsResourcePool pool;
	};
	std::array<Param, 3> params =
	{
		//{ GraphicsResourceUsage::Static, GraphicsResourcePool::None },
		Param{ GraphicsResourceUsage::Static, GraphicsResourcePool::Managed },
		Param{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::None },
		Param{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::Managed },
	};

	// test static and dynamic
	for (int i = 0; i < params.size(); i++)
	{
		auto usage = params[i].usage;
		auto pool = params[i].pool;

		Vector4 vertices[] = {
			Vector4(0, 0.5, 0, 1),
			Vector4(0, 0, 0, 1),
			Vector4(0.5, -0.25, 0, 1),
			Vector4(0, 0, 0, 1),
			Vector4(-0.5, -0.25, 0, 1),
		};
		auto vb1 = makeObject<VertexBuffer>(sizeof(Vector4) * 5, vertices, usage);
		auto ib1 = makeObject<IndexBuffer>(3, IndexBufferFormat::UInt16, usage);
		ib1->setResourcePool(pool);

		// * [ ] まだ一度もレンダリングに使用されていないバッファを、更新できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			uint16_t indices[] = { 0, 2, 4 };
			memcpy(ib1->map(MapMode::Write), indices, ib1->bytesSize());

			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setIndexBuffer(ib1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->drawPrimitiveIndexed(0, 1);
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-IndexBuffer-1.png"), cbb);
		}

		// * [ ] 一度レンダリングに使用されたバッファを、再更新できること
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

			uint16_t indices[] = { 1, 2, 4 };
			memcpy(ib1->map(MapMode::Write), indices, ib1->bytesSize());

			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(m_vertexDecl1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setIndexBuffer(ib1);
			ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
			ctx->drawPrimitiveIndexed(0, 1);
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-IndexBuffer-2.png"), cbb);
		}

		// * [ ] フォーマット変更 16 -> 32
		{
			if (usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None) {
				// un supported
			}
			else
			{
				auto ctx = TestEnv::beginFrame();
                auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
				auto crp = TestEnv::renderPass();
				crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
				ib1->setFormat(IndexBufferFormat::UInt32);

				ctx->beginRenderPass(crp);
				ctx->setVertexLayout(m_vertexDecl1);
				ctx->setVertexBuffer(0, vb1);
				ctx->setIndexBuffer(ib1);
				ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
				ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
				ctx->drawPrimitiveIndexed(0, 1);
				ctx->endRenderPass();
				TestEnv::endFrame();
                ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-IndexBuffer-2.png"), cbb);	// ↑と同じ結果
			}
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, ViewportAndScissor)
{
	m_shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

	Vector4 v[] = {
		Vector4(0, 0.5, 0, 1),
		Vector4(0.5, -0.25, 0, 1),
		Vector4(-0.5, -0.25, 0, 1),
	};
	auto vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	//* [ ] Viewport
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

		ctx->setViewportRect(Rect(0, 0, 80, 60));		// 左上
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ctx->setViewportRect(Rect(80, 60, 80, 60));		// 右下
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-ViewportAndScissor-1.png"), cbb);

		ctx->setViewportRect(Rect::Empty);		// reset
	}

	//* [ ] Scissor
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

		ctx->setScissorRect(Rect(0, 0, 80, 60));		// 左上
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ctx->setScissorRect(Rect(80, 60, 80, 60));		// 右下
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-ViewportAndScissor-2.png"), cbb);

		ctx->setScissorRect(Rect(0, 0, 160, 120));		// reset
	}

	//* [ ] Viewport+Scissor (Experimental) ... OpenGL では Viewport が優先だった。
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

		ctx->setViewportRect(Rect(0, 0, 80, 60));		// 左上
		ctx->setScissorRect(Rect(40, 30, 80, 60));		// 中央
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-ViewportAndScissor-3.png"), cbb);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, ConstantBuffer)
{
	auto shader1 = Shader::create(LN_ASSETFILE("simple.vsh"), LN_ASSETFILE("ConstantBufferTest-1.psh"));
	auto buffer1 = shader1->findConstantBuffer("ConstBuff");
	auto buffer2 = shader1->findConstantBuffer("ConstBuff2");

	Vector4 v[] = {
		Vector4(-1, 1, 0, 1),
		Vector4(0, 1, 0, 1),
		Vector4(-1, 0, 0, 1),
	};
	auto vertexBuffer = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);


	auto renderAndCapture = [&]() {
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
		ctx->drawPrimitive(0, 1);
		ctx->endRenderPass();
		TestEnv::endFrame();
		return TestEnv::capture(cbb)->getPixel32(0, 0);
	};

	// * [ ] float
	{
		buffer1->findParameter("g_type")->setInt(1);
		buffer1->findParameter("g_color1")->setFloat(0.5);	// 赤っぽくする
		ASSERT_EQ(true, renderAndCapture().r > 100);		// 赤っぽくなっているはず
	}
	// * [ ] float2
	{
		buffer1->findParameter("g_type")->setInt(2);
		buffer1->findParameter("g_color2")->setVector(Vector4(1, 0, 0, 1));
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}
	// * [ ] float3
	{
		buffer1->findParameter("g_type")->setInt(3);
		buffer1->findParameter("g_color3")->setVector(Vector4(0, 1, 0, 1));
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	// * [ ] float4
	{
		buffer1->findParameter("g_type")->setInt(4);
		buffer1->findParameter("g_color4")->setVector(Vector4(0, 0, 1, 1));
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}

	float ary1[3] = { 0, 0.5, 0 };
	Vector4 ary2[3] = { {1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1} };

	// * [ ] float[]
	{
		buffer1->findParameter("g_type")->setInt(11);
		buffer1->findParameter("g_float1ary3")->setFloatArray(ary1, 3);
		ASSERT_EQ(true, renderAndCapture().r > 100);

        // TODO: Vulkan で、array は要素が常に 16byte アライメントされる。
        //   float g_float1ary3[3];	// offset:112
        //   float2 g_float2ary3[3];	// offset:160
        // 差は 48、3 で割ると 16
        //  float g_float1ary3[4];	// offset:112
        //  float2 g_float2ary3[4];	// offset:176
        // 
	}
	// * [ ] float2[]
	{
		buffer1->findParameter("g_type")->setInt(12);
		buffer1->findParameter("g_float2ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	// * [ ] float3[]
	{
		buffer1->findParameter("g_type")->setInt(13);
		buffer1->findParameter("g_float3ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	// * [ ] float4[]
	{
		buffer1->findParameter("g_type")->setInt(14);
		buffer1->findParameter("g_float4ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}


#if 0
	//* [ ] 座標変換したときに一般的な使い方ができるか
	{
		auto pos = Vector4(1, 0, 0, 1);
		auto mat = Matrix::makeRotationY(-Math::PIDiv2);
		auto r = Vector4::transform(pos, mat);	// (0, 0, 1)
		buffer1->findParameter("g_type")->setInt(99);
		buffer1->findParameter("g_color4")->setVector(pos);
		buffer2->findParameter("g_mat44")->setMatrix(mat);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] 座標変換したときに一般的な使い方ができるか (array)
	{
		auto pos = Vector4(1, 0, 0, 1);
		auto mat = Matrix::makeRotationY(-Math::PIDiv2);
		auto r = Vector4::transform(pos, mat);	// (0, 0, 1)
		Matrix ary[3] = { {}, mat, {} };
		buffer1->findParameter("g_type")->setInt(100);
		buffer1->findParameter("g_color4")->setVector(pos);
		buffer2->findParameter("g_mat44ary3")->setMatrixArray(ary, 3);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] mul で想定通り座標変換できること
	{
		auto m = Matrix::makeRotationY(-Math::PI / 2);
		auto v = Vector4::transform(Vector4(1, 0, 0, 1), m);
		buffer1->findParameter("g_type")->setInt(35);
		buffer2->findParameter("g_mat44")->setMatrix(m);
		auto c = renderAndCapture();
		ASSERT_EQ(true, c.r < 10);   // expect 0
		ASSERT_EQ(true, c.g < 10);   // expect 0
		ASSERT_EQ(true, c.b > 200);  // expect 255
	}
	//* [ ] mul で想定通り座標変換できること
	{
		auto m = Matrix::makeRotationY(-Math::PI / 2);
		Matrix ary[3] = { {}, m, {} };
		//auto v = Vector4::transform(Vector4(1, 0, 0, 1), m);
		buffer1->findParameter("g_type")->setInt(36);
		buffer2->findParameter("g_mat44ary3")->setMatrixArray(ary, 3);
		auto c = renderAndCapture();
		ASSERT_EQ(true, c.r < 10);   // expect 0
		ASSERT_EQ(true, c.g < 10);   // expect 0
		ASSERT_EQ(true, c.b > 200);  // expect 255
	}
#endif
#if 0	// FIXME: AMD Radeon(TM) HD 8490 で転置されてしまった。
		// 以下、Gforce や macOS では成功するが、Radeon では失敗する。何かパラメータがあるのか？
		// ひとまず、↑の mul では配列かどうかにかかわらず想定通りに座標変換できたのでこのまま行ってみる。
	//* [ ] array しても転置されないこと
	{
        auto mat = Matrix();
        mat(0, 2) = 1;
        mat(2, 0) = 0;
        Matrix ary[3] = { {}, mat, {} };

        buffer2->findParameter("g_mat44")->setMatrix(mat);
        buffer2->findParameter("g_mat44ary3")->setMatrixArray(ary, 3);

		buffer1->findParameter("g_type")->setInt(101);
		auto c = renderAndCapture();
		ASSERT_EQ(true, c.r > 200); // expect 255
		ASSERT_EQ(true, c.g > 200); // expect 255
		ASSERT_EQ(true, c.b < 10);  // expect 0
	}

	//* [ ] float3x4 (Vector4[3])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
		m(0, 1) = 1;	// 転置とかされることなく、この配列アクセスで値が取り出せる
		buffer1->findParameter("g_type")->setInt(21);
		buffer2->findParameter("g_mat34")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}
	//* [ ] float2x2 (Vector2[2])
	{
		Matrix m(
			0, 0, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
		buffer1->findParameter("g_type")->setInt(22);
		buffer2->findParameter("g_mat22")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	//* [ ] float4x3 (Vector3[4])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0);
		buffer1->findParameter("g_type")->setInt(23);
		buffer2->findParameter("g_mat43")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] float4x4 (Vector4[4])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 0, 0);
		buffer1->findParameter("g_type")->setInt(24);
		buffer2->findParameter("g_mat44")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}

	//* [ ] float3x4[3] (Vector4[3][,,])
	{
		Matrix m[3] = {
			{},
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			},
			{} };
		m[1](0, 1) = 1;
		buffer1->findParameter("g_type")->setInt(31);
		buffer2->findParameter("g_mat34ary3")->setMatrixArray(m, 3);
        auto c = renderAndCapture();
        ASSERT_EQ(true, c.r < 10); // expect 0
        ASSERT_EQ(true, c.g > 200);// expect 255
		ASSERT_EQ(true, c.b < 10); // expect 0
	}

	//* [ ] float4x4 (Vector4[4][,,])
	{
		Matrix m[3] = {
			{},
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 1,
				0, 0, 0, 0,
			},
		{} };
		buffer1->findParameter("g_type")->setInt(34);
		buffer2->findParameter("g_mat44ary3")->setMatrixArray(m, 3);
        auto c = renderAndCapture();
        ASSERT_EQ(true, c.r > 200);// expect 255
        ASSERT_EQ(true, c.g < 10); // expect 0
        ASSERT_EQ(true, c.b < 10); // expect 0
	}
#endif
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, Texture)
{
	auto shader1 = Shader::create(LN_ASSETFILE("TextureTest-1.vsh"), LN_ASSETFILE("TextureTest-1.psh"));
	
	auto vertexDecl1 = makeObject<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

	struct Vertex
	{
		Vector2 uv;
		Vector3 pos;
	};
	Vertex v[] = {
		{ { 0, 0 }, { -1, 1, 0 }, },
		{ { 1, 0 }, { 0, 1, 0 }, },
		{ { 0, 1 }, { -1, 0, 0 }, },
		{ { 1, 1 }, { 0, 0, 0 }, },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto tex1 = makeObject<Texture2D>(2, 2);
	auto bmp1 = tex1->map(MapMode::Write);
	bmp1->setPixel32(0, 0, ColorI(255, 0, 0, 255));
	bmp1->setPixel32(1, 0, ColorI(255, 0, 255, 255));
	bmp1->setPixel32(0, 1, ColorI(0, 255, 0, 255));
	bmp1->setPixel32(1, 1, ColorI(0, 0, 255, 255));

	shader1->findParameter("g_texture1")->setTexture(tex1);


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
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-TextureTest-1.png"), cbb);
	}
}

#if 0   // 一部のモバイル環境では 3D テクスチャが使えないので、直近では対応予定なし
//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, Texture3D)
{
	auto shader1 = Shader::create(LN_ASSETFILE("Texture3DTest-1.vsh"), LN_ASSETFILE("Texture3DTest-1.psh"));

	auto vertexDecl1 = makeObject<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::TexCoord, 0);

	struct Vertex
	{
		Vector3 pos;
		Vector3 uv;
	};
	Vertex v[] = {
		{ { -1, 1, 0 }, { 0, 0, 0.5 }, },	// 0.5 で中央の face からサンプリングする
		{ { 0, 1, 0 }, { 1, 0, 0.5 }, },
		{ { -1, 0, 0 }, { 0, 1, 0.5 }, },
		{ { 0, 0, 0 }, { 1, 1, 0.5 }, },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto tex1 = makeObject<Texture3D>(2, 2, 3);
	auto bmp1 = tex1->map(MapMode::Write);
	bmp1->setPixel32(0, 0, 1, ColorI(255, 0, 0, 255));
	bmp1->setPixel32(1, 0, 1, ColorI(255, 0, 255, 255));
	bmp1->setPixel32(0, 1, 1, ColorI(0, 255, 0, 255));
	bmp1->setPixel32(1, 1, 1, ColorI(0, 0, 255, 255));

	shader1->findParameter("g_texture1")->setTexture(tex1);

	auto ctx = TestEnv::graphicsContext();
	TestEnv::resetGraphicsContext(ctx);
	ctx->setVertexLayout(vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setIndexBuffer(nullptr);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	// * [ ] default
	{
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		ctx->drawPrimitive(0, 2);

		ASSERT_SCREEN(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-Texture3D-1.png"));
	}
}
#endif

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, SamplerState)
{
	auto shader1 = Shader::create(LN_ASSETFILE("TextureTest-1.vsh"), LN_ASSETFILE("TextureTest-1.psh"));

	auto vertexDecl1 = makeObject<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

	struct Vertex
	{
		Vector2 uv;
		Vector3 pos;
	};
	Vertex v[] = {
		{ { 0, 0 },{ -1, 1, 0 }, },
		{ { 2, 0 },{ 0, 1, 0 }, },
		{ { 0, 2 },{ -1, 0, 0 }, },
		{ { 2, 2 },{ 0, 0, 0 }, },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto tex1 = makeObject<Texture2D>(2, 2);
	auto bmp1 = tex1->map(MapMode::Write);
	bmp1->setPixel32(0, 0, ColorI(255, 0, 0, 255));
	bmp1->setPixel32(1, 0, ColorI(255, 0, 255, 255));
	bmp1->setPixel32(0, 1, ColorI(0, 255, 0, 255));
	bmp1->setPixel32(1, 1, ColorI(0, 0, 255, 255));

	shader1->findParameter("g_texture1")->setTexture(tex1);

	// * [ ] default (Point, Reprat)
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
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-SamplerState-1.png"), cbb);
	}

	// * [ ] Linear, Clamp
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

		auto sampler = makeObject<SamplerState>();
		sampler->setFilterMode(TextureFilterMode::Linear);
		sampler->setAddressMode(TextureAddressMode::Clamp);
		tex1->setSamplerState(sampler);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		ctx->drawPrimitive(0, 2);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-SamplerState-2.png"), cbb);
	}
}

//------------------------------------------------------------------------------
//## RenderState 関係のテスト。設定が Native layer まで渡ることを確認する。
TEST_F(Test_Graphics_LowLevelRendering, RenderStateTest)
{
	auto shader1 = Shader::create(LN_ASSETFILE("SimplePosColor.vsh"), LN_ASSETFILE("SimplePosColor.psh"));

	auto vertexDecl1 = makeObject<VertexLayout>();
	vertexDecl1->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	struct Vertex
	{
		Vector3 pos;
		Color color;
	};
	Vertex v1[] = {	// 深度テスト用に少し奥に出しておく
		{ { -1, 1, 0.5 }, Color::Red },
		{ { 0, 1, 0.5 }, Color::Red },
		{ { -1, 0, 0.5 }, Color::Red },
		{ { 0, 0, 0.5 }, Color::Red },
	};
	auto vb1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

	Vertex v2[] = {
		{ { -0.5, 0.5, 0 }, Color::Blue },
		{ { 0.5, 0.5, 0 }, Color::Blue },
		{ { -0.5, -0.5, 0 }, Color::Blue },
		{ { 0.5, -0.5, 0 }, Color::Blue },
	};
	auto vb2 = makeObject<VertexBuffer>(sizeof(v2), v2, GraphicsResourceUsage::Static);

	Vertex v3[] = {	// 裏面テスト用
		{ { 0, 0, 0 }, Color::Green },
		{ { 0, -1, 0 }, Color::Green },
		{ { 1, 0, 0 }, Color::Green },
		{ { 1, -1, 0 }, Color::Green },
	};
	auto vb3 = makeObject<VertexBuffer>(sizeof(v3), v3, GraphicsResourceUsage::Static);

	// * [ ] check BlendState (RGB Add blend)
	{
		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto crp = TestEnv::renderPass();
		crp->setClearValues(ClearFlags::All, Color::Gray, 1.0f, 0);
		ctx->beginRenderPass(crp);
		ctx->setVertexLayout(vertexDecl1);
		ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

		BlendStateDesc state1;
		state1.renderTargets[0].blendEnable = true;
		state1.renderTargets[0].sourceBlend = BlendFactor::One;
		state1.renderTargets[0].destinationBlend = BlendFactor::One;
		state1.renderTargets[0].blendOp = BlendOp::Add;

		ctx->setBlendState(BlendStateDesc());
		ctx->setVertexBuffer(0, vb1);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		ctx->drawPrimitive(0, 2);
		
		ctx->setBlendState(state1);
		ctx->setVertexBuffer(0, vb2);
		ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		ctx->drawPrimitive(0, 2);

		ctx->endRenderPass();
		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-1.png"), cbb);

		//ctx->setBlendState(BlendStateDesc());	// 戻しておく
	}
	// * [ ] check RasterizerState
	{
		RasterizerStateDesc state1;
		state1.cullMode = CullMode::Front;

		RasterizerStateDesc state2;
		state2.cullMode = CullMode::Back;

		RasterizerStateDesc state3;
		state3.cullMode = CullMode::None;

		RasterizerStateDesc state4;
		state4.fillMode = FillMode::Wireframe;

		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setRasterizerState(state1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤は描かれない
			ctx->setVertexBuffer(0, vb3);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 緑は描かれる
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-1.png"), cbb);
		}

		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setRasterizerState(state2);
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤は描かれる
			ctx->setVertexBuffer(0, vb3);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 緑は描かれない
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-2.png"), cbb);
		}

		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setRasterizerState(state3);
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤は描かれる
			ctx->setVertexBuffer(0, vb3);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 緑は描かれる
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-3.png"), cbb);
		}

		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setRasterizerState(state4);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-4.png"), cbb);
		}

		//ctx->setRasterizerState(RasterizerStateDesc());	// 戻しておく
	}

	// * [ ] check DepthStencilState
	{
		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 青 (z=0)
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤 (z=0.5)
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-1.png"), cbb);
		}

		{
			DepthStencilStateDesc state1;
			state1.depthTestFunc = ComparisonFunc::Always;

			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setDepthStencilState(state1);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 青 (z=0)
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤 (z=0.5)
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-2.png"), cbb);
		}

		{
			DepthStencilStateDesc state2;
			state2.depthWriteEnabled = false;

			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setDepthStencilState(state2);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 青 (z=0)
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤 (z=0.5)
			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-3.png"), cbb);
		}

		//ctx->setDepthStencilState(DepthStencilStateDesc());	// 戻しておく
	}

	// * [ ] check DepthStencilState (stencil test)
	{
		// ステンシル書き込み (参照値0xFF が型抜きの hole と考える)
		DepthStencilStateDesc state1;
		state1.stencilEnabled = true;							// true にしないと書き込まれない
		state1.frontFace.stencilPassOp = StencilOp::Replace;	// 描画できたところに参照値を書き込む
		state1.frontFace.stencilFunc = ComparisonFunc::Always;	// 常に成功（常に上書き）

		{
			auto ctx = TestEnv::beginFrame();
            auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
			ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);
			ctx->setVertexLayout(vertexDecl1);
			ctx->setDepthStencilState(state1);
			ctx->setVertexBuffer(0, vb1);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 赤 (z=0.5)

			// ステンシルテスト
			DepthStencilStateDesc state2;
			state2.stencilEnabled = true;
			state2.frontFace.stencilFunc = ComparisonFunc::Equal;	// 0xFF(デフォルト値)と等しければ成功 → カラーバッファ書き込み
			ctx->setDepthStencilState(state2);
			ctx->setVertexBuffer(0, vb2);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
			ctx->drawPrimitive(0, 2);	// 青 (z=0)

			ctx->endRenderPass();
			TestEnv::endFrame();
            ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderStateTest-4-1.png"), cbb);
		}

		//ctx->setDepthStencilState(DepthStencilStateDesc());	// 戻しておく

		// TODO: ステンシル書き込み時に描画はしない
	}
}

//------------------------------------------------------------------------------
//## RenderTarget
TEST_F(Test_Graphics_LowLevelRendering, RenderTarget)
{
    //* [ ] 特に OpenGL を使用している場合に、RT をサンプリングすると上下反転しないことを確認する。
    {
        auto shader2 = Shader::create(LN_ASSETFILE("TextureTest-1.vsh"), LN_ASSETFILE("TextureTest-1.psh"));

        Vector4 v1[] = {
            Vector4(0, 0.5, 0, 1),
            Vector4(0.5, -0.25, 0, 1),
            Vector4(-0.5, -0.25, 0, 1),
        };
        auto vertexBuffer1 = makeObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

        struct Vertex
        {
            Vector2 uv;
            Vector3 pos;
        };
        Vertex v[] = {
            { { 0, 0 }, { -1, 1, 0 }, },
            { { 1, 0 }, { 1, 1, 0 }, },
            { { 0, 1 }, { -1, -1, 0 }, },
            { { 1, 1 }, { 1, -1, 0 }, },
        };
        auto vertexBuffer2 = makeObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
        auto vertexDecl2 = makeObject<VertexLayout>();
        vertexDecl2->addElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
        vertexDecl2->addElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

        auto renderTarget1 = makeObject<RenderTargetTexture>(160, 120, TextureFormat::RGBA8, false);

		auto ctx = TestEnv::beginFrame();
        auto cbb = TestEnv::mainWindowSwapChain()->currentBackbuffer();
		auto renderPass = makeObject<RenderPass>();
        renderPass->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);

        // まず renderTarget1 へ緑色の三角形を描く
        {
            m_shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(0, 1, 0, 1));
			renderPass->setRenderTarget(0, renderTarget1);
			ctx->beginRenderPass(renderPass);
            ctx->setVertexLayout(m_vertexDecl1);
            ctx->setVertexBuffer(0, vertexBuffer1);
            ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleList);
            ctx->drawPrimitive(0, 1);
			ctx->endRenderPass();
        }

        // 次に renderTarget1 からバックバッファへ全体を描く
        {
            shader2->findParameter("g_texture1")->setTexture(renderTarget1);
			auto crp = TestEnv::renderPass();
			crp->setClearValues(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->beginRenderPass(crp);
            ctx->setVertexLayout(vertexDecl2);
            ctx->setVertexBuffer(0, vertexBuffer2);
            ctx->setShaderPass(shader2->techniques()[0]->passes()[0]);
			ctx->setPrimitiveTopology(PrimitiveTopology::TriangleStrip);
            ctx->drawPrimitive(0, 2);
			ctx->endRenderPass();
        }

		TestEnv::endFrame();
        ASSERT_RENDERTARGET(LN_ASSETFILE("Graphics/Result/Test_Graphics_LowLevelRendering-RenderTarget-1.png"), cbb);
    }
}

