#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(Input, VirtualController, Basic)
{
	EngineSettings::addAssetsDirectory("D:/Proj/LN/HC1/External/Lumino/Samples");
	Engine::initialize();

	auto icon = GlyphIcon2D::create(_T("fa-star"), 64);

	Vector3 pos(100, 100, 0);

	while (Engine::update())
	{
		// �� �{�^����������Ă�����
		if (Input::isPressed(InputButtons::Left))
		{
			pos.x -= 1.0;
		}

		// �� �{�^����������Ă�����
		if (Input::isPressed(InputButtons::Right))
		{
			pos.x += 1.0;
		}

		// �� �{�^����������Ă�����
		if (Input::isPressed(InputButtons::Up))
		{
			pos.y -= 1.0;
		}

		// �� �{�^����������Ă�����
		if (Input::isPressed(InputButtons::Down))
		{
			pos.y += 1.0;
		}

		// �A�C�R���̈ʒu��ݒ肷��
		icon->setPosition(pos);
	}
}
