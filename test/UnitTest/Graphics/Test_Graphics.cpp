#include <TestConfig.h>
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
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
	const SizeI& size = Viewport::GetMainViewport()->GetSize();
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
	//	r->DrawSprite2D(SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White);

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
		Engine::UpdateFrame2();	// update camera transform
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
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSquarePrimitive1.png")));
	}
}


//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Rendering, DrawMesh)
{
	{
		auto mesh = RefPtr<StaticMeshModel>::MakeRef();
		mesh->InitializeSphere(detail::GraphicsManager::GetInstance(), 2, 4, 4, MeshCreationFlags::None);

		Engine::UpdateFrame2();	// update camera transform
		if (Engine::BeginRendering())
		{
			Engine::Render();
			Engine::GetDefault3DLayer()->GetRenderer()->DrawMesh(mesh, 0, mesh->GetMeshResource()->GetMaterial(0));
			Engine::EndRendering();
		}
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
		auto rt1 = RenderTarget::Create(Viewport::GetMainViewport()->GetSize());
		auto tex = Texture2D::Create(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawMesh1.png"));
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
		auto font = RawFont::GetDefaultFont();
		float w = Viewport::GetMainViewport()->GetSize().width;

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
	if (Engine::BeginRendering())
	{
		Engine::Render();

		auto* r = Engine::GetDefault2DLayer()->GetRenderer();
		r->DrawSprite(Vector3(48, 0, 10), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);	// ��O
		r->DrawSprite(Vector3(0, 0, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(32, 0, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

		r->DrawSprite(Vector3(0, 32, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 1), SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(32, 32, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color(0, 1, 0, 1), SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(64, 32, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color(0, 0, 1, 1), SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(96, 32, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0.5), SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(128, 32, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 32), Color(1, 0, 0, 0), SpriteBaseDirection::Basic2D);

		r->DrawSprite(Vector3(0, 64, 0), SizeF(16, 32), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(16, 64, 0), SizeF(32, 16), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(16, 80, 0), SizeF(16, 16), tex, RectF(0, 0, 32, 32), Color::White, SpriteBaseDirection::Basic2D);

		r->DrawSprite(Vector3(64, 64, 0), SizeF(16, 16), tex, RectF(16, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(80, 64, 0), SizeF(16, 16), tex, RectF(0, 16, 32, 16), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(64, 80, 0), SizeF(16, 16), tex, RectF(16, 16, 16, 16), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(80, 80, 0), SizeF(16, 16), tex, RectF(0, 0, 16, 16), Color::White, SpriteBaseDirection::Basic2D);

		r->DrawSprite(Vector3(96, 64, 0), SizeF(32, 32), tex, RectF(0, 0, 16, 32), Color::White, SpriteBaseDirection::Basic2D);
		r->DrawSprite(Vector3(128, 64, 0), SizeF(32, 32), tex, RectF(0, 0, 32, 16), Color::White, SpriteBaseDirection::Basic2D);

		Engine::EndRendering();
	}

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Graphics_Rendering.DrawSprite1.png"), 99, true));
}

