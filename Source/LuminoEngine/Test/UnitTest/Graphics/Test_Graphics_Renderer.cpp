#include <TestConfig.h>

class Test_Graphics_Renderer : public ::testing::Test
{
protected:
	RefPtr<Shader>			m_shader;
	RefPtr<VertexBuffer>	m_vbUpTriangle;		// ��
	RefPtr<VertexBuffer>	m_vbDownTriangle;	// ��

	virtual void SetUp()
	{
		m_shader = TestEnv::CreateShader(LOCALFILE("TestData/PosColor.lnsl"));

		// ��
		PosColorVertex vertices1[] =
		{
			{ Vector3(-1.0f, -1.0f, 0.0f), ColorF::Blue },	// ���� ��
			{ Vector3(1.0f, -1.0f, 0.0f), ColorF::Green },	// �E�� ��
			{ Vector3(0.0f, 1.0f, 0.0f), ColorF::Red },		// ���_ ��
		};
		m_vbUpTriangle.Attach(VertexBuffer::Create(PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices1), vertices1));

		// ��
		PosColorVertex vertices2[] =
		{
			{ Vector3(-1.0f, 1.0f, 0.0f), ColorF::Blue },	// ���� ��
			{ Vector3(1.0f, 1.0f, 0.0f), ColorF::Green },	// �E�� ��
			{ Vector3(0.0f, -1.0f, 0.0f), ColorF::Red },	// ���_ ��
		};
		m_vbDownTriangle.Attach(VertexBuffer::Create(PosColorVertex::GetLayout(), PosColorVertex::LayoutCount, LN_ARRAY_SIZE_OF(vertices2), vertices2));


		Renderer* r = TestEnv::Renderer;
		SwapChain* swap = TestEnv::MainSwapChain;
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
	}

	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Renderer, SetDepthStencilState)
{
	Renderer* r = TestEnv::BeginRendering();

	// �X�e���V���o�b�t�@�����
	DepthStencilState state1;
	state1.DepthEnable = true;
	state1.DepthWriteEnable = false;			// �[�x�o�b�t�@���X�V����K�v�͂Ȃ�
	state1.DepthFunc = CompareFunc::Never;		// �[�x�e�X�g��S�Ď��s�ɂ��A�J���[�o�b�t�@�֏������܂Ȃ�
	state1.StencilEnable = true;
	state1.StencilFunc = CompareFunc::Always;	// �X�e���V���e�X�g�͏�ɍ��i����
	state1.StencilReferenceValue = 0xFF;		// �X�e���V���o�b�t�@�ɏ������ޒl
	state1.StencilFailOp = StencilOp::Keep;		// (��Ύ��s���Ȃ��̂ŏȗ����Ă悢��)
	state1.StencilDepthFailOp = StencilOp::Replace;
	state1.StencilPassOp = StencilOp::Replace;
	r->SetDepthStencilState(state1);
	r->SetVertexBuffer(m_vbDownTriangle);
	m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);

	// �X�e���V���o�b�t�@��K�p���ĕ`�悷��
	DepthStencilState state2;
	state2.DepthEnable = true;
	state2.DepthWriteEnable = true;
	state2.DepthFunc = CompareFunc::Less;		// �f�t�H���g�l�ɖ߂�
	state2.StencilEnable = true;
	state2.StencilFunc = CompareFunc::Equal;	// ��قǕ`���� 0xFF �Ɠ������̂����i�ɂ���
	state2.StencilReferenceValue = 0xFF;		// ��قǕ`���� 0xFF �Ɠ������̂����i�ɂ���
	state2.StencilFailOp = StencilOp::Keep;		// �X�e���V���o�b�t�@�ւ͏������܂Ȃ�
	state2.StencilDepthFailOp = StencilOp::Keep;// �X�e���V���o�b�t�@�ւ͏������܂Ȃ�
	state2.StencilPassOp = StencilOp::Keep;		// �X�e���V���o�b�t�@�ւ͏������܂Ȃ�
	r->SetDepthStencilState(state2);
	r->SetVertexBuffer(m_vbUpTriangle);
	m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
	r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);

	TestEnv::EndRendering();

	SS_CHECK(/*SS_SAVE*/SS_LOAD, "Test_Graphics_Renderer.SetDepthStencilState.png");
}
