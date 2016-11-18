#include <TestConfig.h>
#include <Lumino/Graphics/Mesh.h>
#include "../../../src/Graphics/GraphicsManager.h"


//==============================================================================
class Test_Graphics_Color : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Constructor)
{
	Color32 c1;
	ASSERT_EQ(Color32(0, 0, 0, 0), c1);
	Color32 c2(1, 2, 3);
	ASSERT_EQ(Color32(1, 2, 3, 255), c2);
	Color32 c3(1, 2, 3, 4);
	ASSERT_EQ(Color32(1, 2, 3, 4), c3);
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Color, Operators)
{
	Color32 c1(1, 2, 3, 4);
	Color32 c2(1, 2, 3, 4);
	Color32 c3(1, 2, 3, 5);
	ASSERT_EQ(true, c1 == c2);
	ASSERT_EQ(false, c1 == c3);
	ASSERT_EQ(true, c1 != c3);
	ASSERT_EQ(false, c1 != c2);
}

//==============================================================================
class Test_Graphics_Viewport : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Viewport, Basic)
{
	const Size& size = Engine::GetMainViewport()->GetSize();
	ASSERT_EQ(160, size.width);
	ASSERT_EQ(120, size.height);
}

//==============================================================================
class Test_Graphics_Rendering : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Basic)
{

	//if (Engine::BeginRendering())
	//{
	//	Engine::Render();

	//	auto* r = Engine::GetDefault2DLayer()->GetRenderer();
	//	r->Clear(ClearFlags::Color, Color::Red);
	//	auto tex = Texture2D::Create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));
	//	r->DrawSprite2D(Size(32, 32), tex, RectF(0, 0, 32, 32), Color::White);

	//	Engine::EndRendering();
	//}

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Basic1.png"), 99, true));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Clear)
{
	// <Test> 2D �r���[�̃N���A
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault2DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Blue);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear1.png")));
	}
	// <Test> 3D �r���[�̃N���A
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			auto* r1 = Engine::GetDefault3DLayer()->GetRenderer();
			r1->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear2.png")));
	}
	// <Test> 2D�A3D �r���[�N���A�̑g�ݍ��킹�B2D ����O�ɂȂ�
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Blue);
			Engine::GetDefault3DLayer()->GetRenderer()->Clear(ClearFlags::Color, Color::Red);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Clear3.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawLinePrimitive)
{
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->DrawLinePrimitive(
				Vector3(0, 0, 0), Color::Red,
				Vector3(50, 50, 0), Color::White);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawLinePrimitive1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSquarePrimitive)
{
	{
		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawSquarePrimitive(
				Vector3(-1, 2, 0), Vector2(0, 0), Color::Red,
				Vector3(-1, -2, 0), Vector2(0, 1), Color::Green,
				Vector3(1, 2, 0), Vector2(1, 0), Color::Blue,
				Vector3(1, -2, 0), Vector2(1, 1), Color::White);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawSquarePrimitive(
				Vector3(0, 0, 0), Vector2(0, 0), Color::Red,
				Vector3(0, 50, 0), Vector2(0, 1), Color::Green,
				Vector3(50, 0, 0), Vector2(1, 0), Color::Blue,
				Vector3(50, 50, 0), Vector2(1, 1), Color::White);
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSquarePrimitive1.png")));
	}
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawMesh)
{
	{
		auto mesh = RefPtr<StaticMeshModel>::MakeRef();
		mesh->InitializeSphere(detail::GraphicsManager::GetInstance(), 2, 4, 4, MeshCreationFlags::None);

		Engine::BeginFrameUpdate();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
			Engine::EndRendering();
		}
		Engine::EndFrameUpdate();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, Blit)
{
	// <Test> �f�t�H���g�̃����_�[�^�[�^�[�Q�b�g�ւ̓]��
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(tex);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
	// <Test> �ʂ̃����_�[�^�[�Q�b�g�ւ̓]��
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
		auto rt1 = RenderTarget::Create(tex->GetSize());
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(tex, rt1, nullptr);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.Blit1.png")));	// �܂����̎��_�ł͊D�F

		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->Blit(rt1);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawText_)
{
	{
		auto font = Font::GetDefault();
		float w = Engine::GetMainViewport()->GetSize().width;

		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetFont(font);
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::White);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text1"), RectF(0, 0, w, 100), StringFormatFlags::LeftAlignment);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text2"), RectF(0, 0, w, 100), StringFormatFlags::CenterAlignment);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawText_(_T("Text3"), RectF(0, 0, w, 100), StringFormatFlags::RightAlignment);
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawText1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawSprite)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("../Scene/TestData/Sprite1.png"));

	// <Test>
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* r = Engine::GetDefault2DLayer()->GetRenderer();
			r->SetBlendMode(BlendMode::Alpha);

			r->DrawSprite(Vector3(48, 0, 10), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);	// ��O
			r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(32, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(32, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(0, 1, 0, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(0, 0, 1, 1), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(96, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0.5), SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(128, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0), SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(0, 64, 0), Size(16, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(16, 64, 0), Size(32, 16), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(16, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(64, 64, 0), Size(16, 16), Vector2::Zero, tex, RectF(16, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(80, 64, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 16, 32, 16), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(16, 16, 16, 16), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(80, 80, 0), Size(16, 16), Vector2::Zero, tex, RectF(0, 0, 16, 16), Color::White, SpriteBaseDirection::Basic2D);

			r->DrawSprite(Vector3(96, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(128, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 16), Color::White, SpriteBaseDirection::Basic2D);

			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite1.png")));
	}

	// <Test> Sprite �� BlendMode
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* r = Engine::GetDefault2DLayer()->GetRenderer();

			r->SetBlendMode(BlendMode::Normal); 
			r->DrawSprite(Vector3(0, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(0, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(64, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			r->DrawSprite(Vector3(0, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			
			r->SetBlendMode(BlendMode::Normal);
			r->DrawSprite(Vector3(16, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Alpha);
			r->DrawSprite(Vector3(80, 0, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Add);
			r->DrawSprite(Vector3(16, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
			
			r->SetBlendMode(BlendMode::Subtract);
			r->DrawSprite(Vector3(80, 32, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			r->SetBlendMode(BlendMode::Multiply);
			r->DrawSprite(Vector3(16, 64, 0), Size(32, 32), Vector2::Zero, tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite2.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawRectangle)
{
	// <Test> �P�F�h��Ԃ�
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Red);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(10, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(110, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Blue);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(10, 70, 30, 40));
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
	// <Issue> ������x�������Ƃ��ł���
	{
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Red);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(10, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(110, 20, 30, 40));
			Engine::GetDefault2DLayer()->GetRenderer()->SetBrush(ColorBrush::Blue);
			Engine::GetDefault2DLayer()->GetRenderer()->DrawRectangle(Rect(10, 70, 30, 40));
			Engine::EndRendering();
		}
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawRectangle1.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawFrameRectangle)
{
	// <Test> BorderFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(Rect(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Tile);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(Rect(0, 160, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Tile);
		brush2->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush2->SetBorderThickness(ThicknessF(0, 2, 4, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(Rect(0, 160, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* g = Engine::GetDefault2DLayer()->GetRenderer();
			g->Clear(ClearFlags::All, Color::Gray);

			// �����ψ�
			g->SetBrush(brush1);
			g->DrawRectangle(Rect(0, 0, 43, 63));

			// ����������
			g->SetBrush(brush2);
			g->DrawRectangle(Rect(60, 0, 43, 63));

			// �k�� + �����ψ�
			g->SetBrush(brush3);
			g->DrawRectangle(Rect(0, 80, 6, 4));

			// �k�� + ����������
			g->SetBrush(brush2);
			g->DrawRectangle(Rect(60, 80, 6, 4));

			Engine::EndRendering();

			ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle1.png")));
		}
	}
	// <Test> BorderFrame + Stretch
	// <Test> BoxFrame + Stretch
	// <Test> BoxFrame + Tile
	{
		auto brush1 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush1->SetSourceRect(Rect(0, 192, 32, 32));
		brush1->SetWrapMode(BrushWrapMode::Stretch);
		brush1->SetImageDrawMode(BrushImageDrawMode::BorderFrame);
		brush1->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush2 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush2->SetSourceRect(Rect(0, 192, 32, 32));
		brush2->SetWrapMode(BrushWrapMode::Stretch);
		brush2->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush2->SetBorderThickness(ThicknessF(8, 8, 8, 8));

		auto brush3 = TextureBrush::Create(LN_LOCALFILE("TestData/Window.png"));
		brush3->SetSourceRect(Rect(0, 192, 32, 32));
		brush3->SetWrapMode(BrushWrapMode::Tile);
		brush3->SetImageDrawMode(BrushImageDrawMode::BoxFrame);
		brush3->SetBorderThickness(ThicknessF(8, 8, 8, 8));


		if (Engine::BeginRendering())
		{
			Engine::Render();

			auto* g = Engine::GetDefault2DLayer()->GetRenderer();
			g->Clear(ClearFlags::All, Color::Gray);
			g->SetBrush(brush1);
			g->DrawRectangle(RectF(0, 0, 43, 63));
			g->SetBrush(brush2);
			g->DrawRectangle(RectF(50, 0, 43, 63));
			g->SetBrush(brush3);
			g->DrawRectangle(RectF(100, 0, 43, 63));
			Engine::EndRendering();
		}

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawFrameRectangle2.png")));
	}
}
