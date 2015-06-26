#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_IndexBuffer : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_IndexBuffer, BasicTriangle)
{
	ByteBuffer code(FileUtils::ReadAllBytes(LOCALFILE("TestData/PosColor.lnsl")));
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


	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	//while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(1, 1, 1, 1), 1.0f);
		r->SetVertexBuffer(vb);
		r->SetIndexBuffer(ib);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 2);
		swap->Present();

		//::Sleep(10);
	}

	//TestEnvironment::SaveScreenShot(LOCALFILE("test.png"));
	ASSERT_TRUE(TestEnvironment::EqualsScreenShot(LOCALFILE("TestData/Test_Graphics_IndexBuffer.BasicTriangle.png")));
}
