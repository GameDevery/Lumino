#include <Lumino.h>
using namespace ln;

// �摜��\������
LN_SAMPLE_MAIN(Graphics2D, Sprite2D)
{
	Engine::initialize();

	// �摜��ǂݍ��݁A�X�v���C�g�I�u�W�F�N�g���쐬����
	auto sprite = Sprite2D::create(_T("Scene1.jpg"));

	while (Engine::update())
	{
	}
}
