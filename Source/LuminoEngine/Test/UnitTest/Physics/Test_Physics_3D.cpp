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
		//auto col1 = BoxCollider::Create(1, 2, 3);
		//auto body1 = RigidBody::Create(col1);
		//for (int i = 0; i < 10; i++) Engine::Update();
		//ASSERT_EQ(true, body1->GetWorldTransform().GetPosition().y < 1);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Physics_3D, TriggerCollider)
{
	// <Test> 
	{
		//auto col1 = BoxCollider::Create(1, 1, 1);
		//col1->SetTrigger(true);
		//auto col2 = BoxCollider::Create(1, 1, 1);
		//col2->SetTrigger(true);
		//auto body1 = RigidBody::Create(col1);
		//auto body2 = RigidBody::Create(col2);

		//Engine::Update();

		//for (int i = 0; i < 10; i++) 
		//ASSERT_EQ(true, body1->GetWorldTransform().GetPosition().y < 1);
	}
}
