#include <TestConfig.h>

#if 0
class Test_Scene_SpriteParticle : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		material = Material::create();
		material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle1.png")));
		material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));

		oldCamPos = Camera::GetMain3DCamera()->GetPosition();
		Camera::GetMain3DCamera()->SetPosition(0, 0, 5);
	}
	virtual void TearDown()
	{
		Camera::GetMain3DCamera()->SetPosition(oldCamPos);
	}

	Vector3 oldCamPos;
	MaterialPtr material;
};


//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Default)
{
	auto particleModel1 = SpriteParticleModel::create();
	particleModel1->SetMaterial(material);

	auto particle1 = ParticleEmitter3D::create(particleModel1);
	particle1->SetBlendMode(BlendMode::Add);

	// Create ����A�t�F�[�h�C����
	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SpriteParticle.Default1.png")));

	// 0.5�b��A���ʂɕ\������Ă���
	for (int i = 0; i < 30; ++i) Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SpriteParticle.Default2.png")));

	// 1�b��A������B
	for (int i = 0; i < 29; ++i) Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Empty1.png")));

	// �ȍ~�A1�̗��q�Ń��[�v
	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SpriteParticle.Default1.png")));
	for (int i = 0; i < 30; ++i) Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_Scene_SpriteParticle.Default2.png")));
	for (int i = 0; i < 29; ++i) Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Empty1.png")));
}

#if 0
//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, PositionVelocityAccel)
{
	auto m = SpriteParticleModel::create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0));
	m->SetVelocity(Vector3(0, 5, 0));
	m->SetAccel(Vector3(0, -8, 0));
	m->SetTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	auto particle1 = SpriteParticle::Create3D(m);

	// TODO: �J�����ʒu�Œ肵�Ă����Ȃ��ƁA�����l���ς�������ɑΉ��ł��Ȃ�����

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.PositionVelocityAccel.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, RandomBaseValue)
{
	auto m = SpriteParticleModel::create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetRandomBaseValueRange(0.0f, 1.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0), ParticleRandomSource::ByBaseValue);
	m->SetSizeRange(0.5f, 3.0f, ParticleRandomSource::ByBaseValue);
	m->SetTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	auto particle1 = SpriteParticle::Create3D(m);

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.RandomBaseValue.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, Billboard)
{
	auto m = SpriteParticleModel::create();
	m->SetLifeTime(10.0f);
	m->SetSize(10.0f);
	m->SetTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle1.png")));
	auto particle1 = SpriteParticle::Create3D(m);

	// �΂ߏォ�猩���낷
	Camera::GetMain3DCamera()->SetPosition(20, 20, -20.0f);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.Billboard.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, MovementDirection)
{
	auto m = SpriteParticleModel::create();
	m->SetLifeTime(10.0f);
	m->SetSize(10.0f);
	m->SetVelocity(Vector3(0.0, 0, 2.0));
	m->m_particleDirection = ParticleDirection::MovementDirection;
	m->SetTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle2.png")));
	auto particle1 = SpriteParticle::Create3D(m);

	// �΂ߏォ�猩���낷
	Camera::GetMain3DCamera()->SetPosition(20, 20, -20.0f);

	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MovementDirection.png")));
	//TestEnv::SaveScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MovementDirection.png"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Scene_SpriteParticle, MaxParticles)
{
	auto m = SpriteParticleModel::create();
	m->SetSpawnRate(10);
	m->SetLifeTime(10.0f);
	m->SetPositionRange(Vector3(-10, 0, 0), Vector3(10, 0, 0));
	m->SetTexture(Texture2D::create(LN_LOCALFILE("TestData/Particle1.png")));
	m->SetRandomSeed(12345);
	m->SetMaxParticles(1);
	auto particle1 = SpriteParticle::Create3D(m);

	Camera::GetMain3DCamera()->SetPosition(0, 0, -20.0f);

	// �����\������1�B�܂��A�J��Ԃ��Ă��z��O�A�N�Z�X�Ƃ����Ȃ����ƁB
	for (int i = 0; i < 60; ++i) { Engine::UpdateFrame(); }

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_Scene_SpriteParticle.MaxParticles.png")));
}
#endif

#endif
