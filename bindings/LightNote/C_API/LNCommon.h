
#pragma once

//------------------------------------------------------------------------------
// Flags

// UNICODE �g�p�t���O
#if defined (UNICODE) || defined(_UNICODE)
	#define LN_UNICODE
#else
	#define LN_MBS
#endif

// DLL
#if defined(LN_BUILD_DLL)
	#define LN_API			__declspec(dllexport)
#else
	#define LN_API
#endif


//------------------------------------------------------------------------------
// typedef

#include <stdint.h>
typedef intptr_t		LNHandle;

#ifdef LN_UNICODE
	typedef wchar_t		TCHAR;
	#define __T(x)      L ## x
#else
	typedef char		TCHAR;
	#define __T(x)      x
#endif

#define _T(x)			__T(x)

//------------------------------------------------------------------------------
// C_API

// �f�t�H���g������`�̍׍H
#ifdef LN_DISABLE_AT_DOXY_BUILD
	// Doxygen ���s���͉�����`���Ȃ�
	// Doxygen �ݒ� [Preprcessor] > [PREDEFINED] >�uLN_DEFAULT_ARG(x)= =x�v�ɂ����
	// �h�L�������g��̓f�t�H���g���������݂��邱�ƂɂȂ�B
/*
	[Preprcessor] > [MACRO_EXPANSION] �c ON
	[Preprcessor] > [PREDEFINED] �c "LN_DISABLE_AT_DOXY_BUILD"
	[Preprcessor] > [PREDEFINED] �c "LN_DEFAULT_ARG(x)= =x"
*/
#else
	#ifdef __cplusplus
		// C++ �r���h���̓f�t�H���g������L���ɂ���
		#define LN_DEFAULT_ARG(x) =x
	#else
		// C �r���h���͖����ɂ���
		#define LN_DEFAULT_ARG(x)
	#endif
#endif

#define LN_MODULE(x)
#define LN_MODULE_END
#define LN_CLASS(...)			/*LNOTE_TYPE_INFO_DECL(LNSound);*/
#define LN_STATIC_CLASS(x)
#define LN_STRUCT_CLASS(x)
#define LN_EXTENSION_CLASS(x)
#define LN_CLASS_END

#define LN_INSTANCE_API		LN_API
#define LN_INTERNAL_API		LN_API
#define LN_STATIC_API		LN_API

#define LN_LIBRARY_INITIALIZER
#define LN_LIBRARY_TERMINATOR
#define LN_ATTR_CONSTRUCTOR
#define LN_ATTR_PROPERTY
#define LN_ATTR_EVENT
//#define LN_ATTR_STRUCT_CONSTRUCTOR

#define LN_HANDLE(x) LNHandle
#define LN_OUT
