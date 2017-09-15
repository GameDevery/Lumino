#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(Lumino, Basic, MinCode)
{
	// Lumino �̏���������
	Engine::initialize();

	// ���C�����[�v
	while (Engine::update())
	{
	}

	// Lumino �̏I������
	Engine::terminate();
}

LN_SAMPLE_MAIN(Lumino, Basic, HelloWorld)
{
	// Lumino �̏���������
	Engine::initialize();

	// �������\�����邽�߂� TextBlock2D �I�u�W�F�N�g���쐬����
	auto text = TextBlock2D::create(_LT("Hello, Lumino!"));

	// �\�����镶����̒��������_�Ƃ���
	text->setAnchorPoint(0.5, 0.5);

	// �E�B���h�E�����ɔz�u����
	text->setPosition(320, 240);

	// ���C�����[�v
	while (Engine::update())
	{
	}

	// Lumino �̏I������
	Engine::terminate();
}
