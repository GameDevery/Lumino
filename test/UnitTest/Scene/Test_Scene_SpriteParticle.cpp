#include <TestConfig.h>


class Test_Scene_SpriteParticle : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		Engine::SetFixedDeltaTime(1.0f / 60);
	}
	virtual void TearDown()
	{
		Engine::SetFixedDeltaTime(0.0f);
	}
};


//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Default)
{
	auto particleModel1 = SpriteParticleModel::Create();
	particleModel1->SetTexture(Texture2D::Create(LN_LOCALFILE("TestData/Particle1.png")));
	auto particle1 = SpriteParticle::Create3D(particleModel1);

	// Create ����A�����\������Ă���B
	Engine::UpdateFrame();
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Basic_1.png")));

	// 1�b��A������B
	for (int i = 0; i < 60; ++i)
	{
		Engine::UpdateFrame();
	}
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Basic_2.png")));
}
