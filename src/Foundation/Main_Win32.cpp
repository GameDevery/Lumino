
#include <windows.h>
#include "EntryPoint.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPWSTR    lpCmdLine,
//	_In_ int       nCmdShow)
int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
	)
{
#if 0
	char* c = GetCommandLineA();

	// �����𒲂ׂ�
	int argc = 0;
	for (; *c; ++argc)
	{
		// �擪��r����Ō�Ɏg���Ă���󔒂��X�L�b�v����
		while (*c == ' ') { ++c; }

		// �R�}���h���C���Ō�̋󔒂������ꍇ�C���[�v���甲����
		if (!(*c)) { break; }

		// "" �ň͂܂ꂽ�����������
		if (*c == '\"')
		{
			// " ���X�L�b�v
			++c;

			// �I���� " ��T��
			while (*c && *c != '\"') { ++c; }

			// ���̎��_�� c �� " ���w���Ă���̂ŁA���̕����Ɉړ�
			++c;
		}
		// �X�y�[�X�ň͂܂ꂽ�����������
		else
		{
			// ���̋󔒂܂ňړ�
			while (*c && *c != ' ') { ++c; }
		}
	}

	c = GetCommandLineA();
	char** argv = static_cast< char** >(malloc(sizeof(char*) * argc));
	int i = 0;

	while (*c)
	{
		// �擪��r����Ō�Ɏg���Ă���󔒂��X�L�b�v����
		while (*c == ' ') { ++c; }

		// �R�}���h���C���Ō�̋󔒂������ꍇ�C���[�v���甲����
		if (!(*c)) { break; }

		// "" �ň͂܂ꂽ�����������
		if (*c == '\"')
		{
			// " ���X�L�b�v
			++c;

			argv[i] = c;
			++i;

			// �I���� " ��T��
			while (*c && *c != '\"') { ++c; }

			*c = '\0';	// " �� \0 �ɕς��ċ�؂�

			++c;
		}
		// �X�y�[�X�ň͂܂ꂽ�����������
		else
		{
			// ���̋󔒂܂ňړ�
			while (*c && *c != ' ') { ++c; }
		}
	}
#endif

	int r = ln::EntryPoint();

	//SAFE_FREE(argv);

	return r;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int main(int argc_, char* argv_[])
{
	return ln::EntryPoint();
}
