
#include <Lumino.h>
using namespace ln;

// Lumino �A�v���P�[�V�����̃G���g���[�|�C���g
void Main()
{
	// Lumino �̏���������
	Engine::Initialize();

	// �摜��ǂݍ��݁A2D�X�v���C�g���쐬����
	auto sprite = Sprite2D::Create("Assets/icon256.png");

	// 2D�X�v���C�g�̈ʒu��ݒ肷��
	sprite->SetPosition(200, 100);

	// Lumino �̍X�V���� (�E�B���h�E�ւ̕`��Ȃǂ��s���A�E�B���h�E�������Ă���� false ��Ԃ�)
	while (Engine::UpdateFrame())
	{
	}

	// Lumino �̏I������
	Engine::Finalize();
}
