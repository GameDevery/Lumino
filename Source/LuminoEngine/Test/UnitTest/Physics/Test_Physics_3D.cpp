#include <TestConfig.h>

class Test_Physics_3D : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_Physics_3D, Basic)
{
	// <Test> �n�ʂ������̂ŉ��̂ق��֗����Ă���
	{
		auto col1 = BoxCollider::Create(1, 2, 3);
		auto body1 = RigidBody::Create(col1);
		for (int i = 0; i < 10; i++) Engine::Update();
		ASSERT_EQ(true, body1->GetWorldTransform().GetPosition().y < 1);
	}
}

