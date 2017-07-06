#include <Lumino.h>
using namespace ln;

LN_SAMPLE_MAIN(HelloWorld)
{
	// Lumino �̏���������
	LN_SAMPLE_INITIALIZE;

	// �������\�����邽�߂� TextBlock2D �I�u�W�F�N�g���쐬����
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));

	// �\�����镶����̒��������_�Ƃ���
	text->setAnchorPoint(0.5, 0.5);

	// �E�B���h�E�����ɔz�u����
	text->setPosition(320, 240);

	// ���C�����[�v
	while (LN_SAMPLE_UPDATE)
	{
	}

	// Lumino �̏I������
	LN_SAMPLE_TERMINATE;
}
