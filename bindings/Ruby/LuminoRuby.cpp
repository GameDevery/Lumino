#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

// �֐����� create_makefile �Ŏw�肵�����O�ƍ��킹�Ă����Ȃ���΂Ȃ�Ȃ�
extern "C" void Init_Lumino()
{
	// ���[�g���W���[��
	g_luminoModule = rb_define_module("Lumino");
	
    // ��O��`
    g_luminoError = rb_define_class_under(g_luminoModule, "LuminoError", rb_eRuntimeError);
	
	InitEnums();
	InitStructs();
	InitClasses();
}


