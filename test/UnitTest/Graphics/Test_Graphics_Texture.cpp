#include <TestConfig.h>
using namespace Lumino::Imaging;

class Test_Graphics_Texture : public ::testing::Test
{
protected:

	RefPtr<ByteBuffer> m_code;
	RefPtr<Shader> m_shader;
	RefPtr<VertexBuffer> m_vb1;
	RefPtr<VertexBuffer> m_vb2;

	virtual void SetUp() 
	{
		// �V�F�[�_
		m_code.Attach(FileUtils::ReadAllBytes(LOCALFILE("TestData/PosUV.glsl")));
		m_shader.Attach(Shader::Create((char*)m_code->GetData(), m_code->GetSize()));

		// ���_�o�b�t�@
		PosUVVertex vertices[] =
		{
			{ Vector3(-0.8f, 0.8f, 0.0f), Vector2(0, 0) },		// ����
			{ Vector3(-0.8f, -0.8f, 0.0f), Vector2(0, 1) },		// ����
			{ Vector3(0.8f, 0.8f, 0.0f), Vector2(1, 0) },		// �E��
			{ Vector3(0.8f, -0.8f, 0.0f), Vector2(1, 1) },		// �E��
		};
		m_vb1.Attach(VertexBuffer::Create(PosUVVertex::GetLayout(), 2, LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));
	
		// ���_�o�b�t�@2 (�E����`�`��)
		PosUVVertex vertices2[] =
		{
			{ Vector3(0.0f, 0.0f, 0.0f), Vector2(0, 0) },		// ����
			{ Vector3(0.0f, -1.0f, 0.0f), Vector2(0, 1) },		// ����
			{ Vector3(1.0f, 0.0f, 0.0f), Vector2(1, 0) },		// �E��
			{ Vector3(1.0f, -1.0f, 0.0f), Vector2(1, 1) },		// �E��
		};
		m_vb2.Attach(VertexBuffer::Create(PosUVVertex::GetLayout(), 2, LN_ARRAY_SIZE_OF(vertices2), vertices2, DeviceResourceUsage_Static));

	}
	virtual void TearDown() 
	{
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, BasicTriangle)
{
	// �e�N�X�`��
	RefPtr<Texture> tex1(Texture::Create(LOCALFILE("TestData/img1_BYTE_R8G8B8A8_20x20.png")));

	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	m_shader->GetVariables()[0]->SetTexture(tex1);

	//while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(1, 1, 1, 1), 1.0f);
		r->SetVertexBuffer(m_vb1);
		m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
		swap->Present();

		//::Sleep(10);
	}

	//ASSERT_TRUE(TestEnvironment::EqualsBitmapFile(swap->GetBackBuffer()->Lock(), LOCALFILE("TestData/Test_Graphics_VertexBuffer.BasicTriangle.png")));
	swap->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
	swap->GetBackBuffer()->Unlock();
}


//-----------------------------------------------------------------------------
TEST_F(Test_Graphics_Texture, Lock)
{
	// �e�N�X�`��
	RefPtr<Texture> tex1(Texture::Create(LOCALFILE("TestData/img1_BYTE_R8G8B8A8_20x20.png")));

	Renderer* r = TestEnvironment::Renderer;
	SwapChain* swap = TestEnvironment::MainSwapChain;

	m_shader->GetVariables()[0]->SetTexture(tex1);

	//while (TestEnvironment::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap->GetBackBuffer());
		r->SetDepthBuffer(swap->GetBackBufferDepth());
		r->Clear(true, true, ColorF(1, 1, 1, 1), 1.0f);

		// ���F���_��u��
		{
			Bitmap* bmp = tex1->Lock();
			byte_t* data = bmp->GetBitmapBuffer()->GetData();
			data[4] = 0xFF;		// R
			data[5] = 0xFF;		// G
			data[6] = 0x00;		// B
			data[7] = 0xFF;		// A
			tex1->Unlock();
		}
		r->SetVertexBuffer(m_vb1);
		m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);

		// ���F�̓_��u��
		{
			Bitmap* bmp = tex1->Lock();
			byte_t* data = bmp->GetBitmapBuffer()->GetData();
			data[8] = 0x00;		// R
			data[9] = 0xFF;		// G
			data[10] = 0xFF;	// B
			data[11] = 0xFF;	// A
			tex1->Unlock();
		}
		r->SetVertexBuffer(m_vb2);
		m_shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);


		swap->Present();

		//::Sleep(10);
	}

	::Sleep(1000);
	//swap->GetBackBuffer()->Lock();
	//swap->GetBackBuffer()->Unlock();
	ASSERT_TRUE(TestEnvironment::EqualsBitmapFile(swap->GetBackBuffer()->Lock(), LOCALFILE("TestData/Test_Graphics_Texture.Lock.png")));
	swap->GetBackBuffer()->Unlock();
	//swap->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
	//swap->GetBackBuffer()->Unlock();
}

