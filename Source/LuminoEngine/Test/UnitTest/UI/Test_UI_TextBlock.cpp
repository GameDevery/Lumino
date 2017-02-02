﻿#include <TestConfig.h>

class Test_UI_TextBlock : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_TextBlock, DefaultLayout)
{
	/*
		レイアウトルートに直接 SetContent() しただけの場合は、左上に表示される、で正しい。(WPF)
		WPF の TextBlock を同様にデフォルト状態で配置すると、
		- Widht,Height は NaN
		- ActualWidth,ActualHeight はウィンドウいっぱい
		となる。
	*/

	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
	auto textBlock1 = UITextBlock::Create();
	textBlock1->SetText(_T("TextBlock"));
	uiRoot->SetContent(textBlock1);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_TextBlock.Basic.png")));
}
