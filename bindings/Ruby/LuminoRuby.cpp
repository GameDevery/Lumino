#include "LuminoRuby.h"

VALUE g_luminoModule;

// �֐����� create_makefile �Ŏw�肵�����O�ƍ��킹�Ă����Ȃ���΂Ȃ�Ȃ�
extern "C" void Init_Lumino()
{
	g_luminoModule = rb_define_module("Lumino");
	InitEnums();
	InitStructs();
}


