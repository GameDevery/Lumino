#include "Common.hpp"

class Test_Graphics_Bitmap : public ::testing::Test {};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_Bitmap, SimpleLoadSave)
{
	//* [ ] downflow �Ńs�N�Z���f�[�^���擾�ł��邱�ƁB
	{
		auto bmp1 = newObject<Bitmap2D>();
		bmp1->load(LN_ASSETFILE("Bitmap1.png"));
		ASSERT_EQ(Color32(255, 0, 0, 255), bmp1->getPixel32(0, 0));
		ASSERT_EQ(Color32(0, 255, 0, 255), bmp1->getPixel32(3, 0));
		ASSERT_EQ(Color32(0, 0, 255, 255), bmp1->getPixel32(0, 3));
		ASSERT_EQ(Color32(255, 255, 255, 0), bmp1->getPixel32(3, 3));
	}
	//* [ ] downflow �Ńs�N�Z���f�[�^��ۑ��ł��邱�ƁB
	{
		{
			auto bmp0 = newObject<Bitmap2D>(4, 4, PixelFormat::RGBA32);
			bmp0->setPixel32(0, 0, Color32(255, 0, 0, 255));
			bmp0->setPixel32(3, 0, Color32(0, 255, 0, 255));
			bmp0->setPixel32(0, 3, Color32(0, 0, 255, 255));
			bmp0->setPixel32(3, 3, Color32(255, 255, 255, 0));
			bmp0->save(LN_TEMPFILE("bmp0.png"));
		}
		{
			auto bmp1 = newObject<Bitmap2D>();
			bmp1->load(LN_TEMPFILE("bmp0.png"));
			ASSERT_EQ(Color32(255, 0, 0, 255), bmp1->getPixel32(0, 0));
			ASSERT_EQ(Color32(0, 255, 0, 255), bmp1->getPixel32(3, 0));
			ASSERT_EQ(Color32(0, 0, 255, 255), bmp1->getPixel32(0, 3));
			ASSERT_EQ(Color32(255, 255, 255, 0), bmp1->getPixel32(3, 3));
		}
	}
}
