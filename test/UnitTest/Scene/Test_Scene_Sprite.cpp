#include <TestConfig.h>
#include <Lumino/Scene/TextBlock.h>

class Test_Scene_Sprite : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Basic)
{
	// <Test> ���ʂ̕`��
	// <Test> �s�����x�̐ݒ�
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));

		auto sprite1 = Sprite2D::Create(tex);
		sprite1->SetPosition(0, 0);

		auto sprite2 = Sprite2D::Create(tex);
		sprite2->SetPosition(32, 0);
		sprite2->SetOpacity(0.5);

		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Basic.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, BlendMode)
{
	auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
	auto sprite1 = Sprite2D::Create(tex);
	auto sprite2 = Sprite2D::Create(tex);
	auto sprite3 = Sprite2D::Create(tex);
	auto sprite4 = Sprite2D::Create(tex);
	auto sprite5 = Sprite2D::Create(tex);

	sprite1->SetPosition(0, 0);
	sprite1->SetBlendMode(BlendMode::Normal);

	sprite2->SetPosition(32, 0);
	sprite2->SetBlendMode(BlendMode::Alpha);

	sprite3->SetPosition(64, 0);
	sprite3->SetBlendMode(BlendMode::Add);

	sprite4->SetPosition(96, 0);
	sprite4->SetBlendMode(BlendMode::Subtract);

	sprite5->SetPosition(128, 0);
	sprite5->SetBlendMode(BlendMode::Multiply);

	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.BlendMode.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Anchor)
{
	// <Test> ���_�̎w��
	{
		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));

		// ���㌴�_
		auto sprite1 = Sprite2D::Create(tex);
		sprite1->SetPosition(0, 0);
		sprite1->SetAnchorPoint(0, 0);

		// �������_
		auto sprite2 = Sprite2D::Create(tex);
		sprite2->SetPosition(32, 32);
		sprite2->SetAnchorPoint(Vector2(0.5, 0.5));

		// �E�����_
		auto sprite3 = Sprite2D::Create(tex);
		sprite3->SetPosition(64, 64);
		sprite3->SetAnchorPoint(1, 1);

		// ���������_
		auto sprite4 = Sprite2D::Create(tex);
		sprite4->SetPosition(32, 120);
		sprite4->SetAnchorPoint(0.5, 1);

		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_Sprite.Anchor.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, DrawCallCount)
{
	// <Test> �X�e�[�g������ł����1�x�̃h���[�R�[���ɂ܂Ƃ߂���B
	{
		// 1�x�����āA������Ԃ̂Ƃ��̕`�搔���o���Ă���
		Engine::UpdateFrame();
		TestEnv::WaitRendering();
		int defaultCount = EngineDiag::GetGraphicsDeviceDrawCount();

		auto tex = Texture2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		auto sprite1 = Sprite2D::Create(tex);
		auto sprite2 = Sprite2D::Create(tex);
		auto sprite3 = Sprite2D::Create(tex);

		Engine::UpdateFrame();
		TestEnv::WaitRendering();
		ASSERT_EQ(defaultCount + 1, EngineDiag::GetGraphicsDeviceDrawCount());
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite, Issues_Volkoff)
{
	// <Issues> �f�t�H���g�̃T���v���X�e�[�g�̌J��Ԃ����[�h�� Repert �ɂȂ�B
	{
		auto sprite1 = Sprite2D::Create(LN_LOCALFILE("TestData/Sprite1.png"));
		sprite1->SetSrcRect(32, 0, 32, 32);
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_1.png")));
	}
	// <Issues> 2D �ł� Z �\�[�g�̊���J�����ʒu����̒��������ł͂Ȃ��A�X�N���[������̋����łȂ���΂Ȃ�Ȃ��B
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Red);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2D::Create(tex1);
		auto s2 = Sprite2D::Create(tex2);
		s1->SetPosition(10, 20, 100);
		s2->SetPosition(15, 25, 100);	// �X�N���[���� Z ���ʂɕ��s�Ȃ�AZ �������Ƃ��͂��Ƃ����������̂���Ɏ�O�ɂȂ�B
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_2.png")));
	}
	// <Issues> 2D �ł� Z �\�[�g�̊���J�����ʒu����̒��������ł͂Ȃ��A�X�N���[������̋����łȂ���΂Ȃ�Ȃ��B

	// <Issues> �e�N�X�`����ύX�ł��邱�ƁB
	{
		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		tex1->Clear(Color32::Green);
		tex2->Clear(Color32::Blue);
		auto s1 = Sprite2D::Create(tex1);
		Engine::UpdateFrame();			// 1�x�`��
		s1->SetTexture(tex2);			// ���Ƀe�N�X�`����ύX����
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Scene_Sprite.Issues_Volkoff_3.png")));
	}
}


#if 0



class Test_Scene_Sprite3D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite3D, Basic)
{
	// <Test> ���ʂ̕`��
	// <Test> �s�����x�̐ݒ�
	{
		Engine::UpdateFrame();
		int defaultCount = EngineDiag::GetGraphicsDeviceDrawCount();

		auto tex1 = Texture2D::Create(32, 32);
		auto tex2 = Texture2D::Create(32, 32);
		auto sprite1 = Sprite3D::Create(1, 1, tex1);
		auto sprite2 = Sprite3D::Create(1, 1, tex2);
		auto sprite3 = Sprite3D::Create(1, 1, tex1);
		sprite1->SetPosition(0, 0);
		sprite2->SetPosition(10, 10);
		sprite3->SetPosition(20, 20);

		Engine::UpdateFrame();
		int defaultCount2 = EngineDiag::GetGraphicsDeviceDrawCount();
		//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_Sprite3D.Basic.png")));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_Sprite3D, ViewFrustumCulling)
{
	// <Test> �����䉻�����O
	{
		Engine::UpdateFrame();
		int count1 = EngineDiag::GetVisualNodeDrawCount();

		auto tex1 = Texture2D::Create(32, 32);
		auto sprite1 = Sprite3D::Create(1, 1, tex1);
		auto sprite2 = Sprite3D::Create(1, 1, tex1);

		Engine::UpdateFrame();
		int count2 = EngineDiag::GetVisualNodeDrawCount();
		ASSERT_EQ(count1 + 2, count2);

		sprite2->SetPosition(-100, 0);
		Engine::UpdateFrame();
		int count3 = EngineDiag::GetVisualNodeDrawCount();
		ASSERT_EQ(count1 + 1, count3);	// ���S�ɔ͈͊O�Ȃ̂ŕ`�悳��Ȃ�

	}
}

class Test_Scene_TextBlock2D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Scene_TextBlock2D, Basic)
{
	{
		auto text = TextBlock2D::Create(_T("Lumino"));
		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_TextBlock2D.Basic1.png")));
	}
	{
		auto text1 = TextBlock2D::Create();
		text1->SetText(_T("upper left"));
		text1->SetPosition(0, 0, 0);
		text1->SetAnchorPoint(0, 0);

		auto text2 = TextBlock2D::Create();
		text2->SetText(_T("upper right"));
		text2->SetPosition(160, 0, 0);
		text2->SetAnchorPoint(1, 0);

		auto text3 = TextBlock2D::Create();
		text3->SetText(_T("lower left"));
		text3->SetPosition(0, 120, 0);
		text3->SetAnchorPoint(0, 1);

		auto text4 = TextBlock2D::Create();
		text4->SetText(_T("lower right"));
		text4->SetPosition(160, 120, 0);
		text4->SetAnchorPoint(1, 1);

		auto text5 = TextBlock2D::Create();
		text5->SetText(_T("center"));
		text5->SetPosition(80, 60, 0);
		text5->SetAnchorPoint(0.5, 0.5);

		Engine::UpdateFrame();
		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_Scene_TextBlock2D.Basic2.png")));
	}
}
#endif
