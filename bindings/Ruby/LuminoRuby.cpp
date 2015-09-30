#include "LuminoRuby.h"

VALUE g_luminoModule;
VALUE g_luminoError;

const char* LNGetLastErrorMessage()
{
	const LNChar* str;
	LNError_GetLastErrorMessage(&str);
	return LNInternal_ConvertToUTF8String(str, -1);
}

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


