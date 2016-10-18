#include <TestConfig.h>
#include <Lumino/UI/UIPanel.h>

class Test_UI_Button : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_Button, Basic)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
	auto button1 = UIButton::Create();
	button1->SetText(_T("Button"));
	uiRoot->SetContent(button1);

	while (Engine::UpdateFrame());


}

class Test_UI_GridLayout : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, Basic)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid = UIGridLayout::Create();
	auto button = UIButton::Create();
	button->SetSize(SizeF(32, 32));
	grid->AddChild(button);
	uiRoot->SetContent(grid);

	tr::WeakRefPtr<UIButton> ref = button;

	while (Engine::UpdateFrame());

	// button �̎Q�Ƃ�؂��Ă��܂������Ă���
	button.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// grid �̎Q�Ƃ�؂��Ă��܂������Ă���
	grid.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// root ����̎Q�Ƃ�؂�Ƃ悤�₭�폜�����
	uiRoot->SetContent(nullptr);
	ASSERT_EQ(false, ref.IsAlive());

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_UI_TextBlock.Basic.png")));
}
