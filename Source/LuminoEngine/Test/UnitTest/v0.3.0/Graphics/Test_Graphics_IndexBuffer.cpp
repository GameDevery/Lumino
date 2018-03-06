#include <TestConfig.h>

class Test_Graphics_IndexBuffer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_IndexBuffer, BasicTriangle)
{
	ByteBuffer code(FileSystem::ReadAllBytes(LOCALFILE("TestData/PosColor.lnsl")));
	RefPtr<Shader> shader(Shader::Create((char*)code.GetData(), code.GetSize()));

	// ���_�o�b�t�@
	PosColorVertex vertices[] =
	{
		{ Vector3(-0.8f, 0.8f, 0.0f),	ColorF::Blue },		// ���� ��
		{ Vector3(0.8f, 0.8f, 0.0f),	ColorF::White },	// �E�� ��
		{ Vector3(-0.8f, -0.8f, 0.0f),	ColorF::Green },	// ���� ��
		{ Vector3(0.8f, -0.8f, 0.0f),	ColorF::Red },		// �E�� ��
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		PosColorVertex::GetLayout(), 2, LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));

	// �C���f�b�N�X�o�b�t�@
	uint16_t indices[] =
	{
		0, 2, 1,	// �����v��� (���ʁB�f�t�H���g�ł͕`�悳��Ȃ�)
		2, 1, 3,	// ���v��� (�f�t�H���g�̕\��)
	};
	RefPtr<IndexBuffer> ib(IndexBuffer::Create(LN_ARRAY_SIZE_OF(indices), indices, IndexBufferFormat_UInt16, DeviceResourceUsage_Static));


	Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;

	//while (TestEnv::Application->DoEvents())
	{
		Renderer* r = TestEnv::BeginRendering();
		r->SetVertexBuffer(vb);
		r->SetIndexBuffer(ib);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 2);
		TestEnv::EndRendering();

		//::Sleep(10);
	}

	//TestEnv::SaveScreenShot(LOCALFILE("test.png"));
	ASSERT_TRUE(TestEnv::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_IndexBuffer.BasicTriangle.png")));
}