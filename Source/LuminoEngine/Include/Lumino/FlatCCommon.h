#pragma once
#include <stdint.h>

#if defined(LUMINO_BUILD_DLL) && defined(LN_MSVC)
#define LN_API	__declspec(dllexport)
#else
#define LN_API
#endif

typedef wchar_t		LNChar;
#define __T(x)      L ## x

#define LN_OK	LN_RESULTCODE_OK

typedef int		LNHandle;
typedef void* LNUserData;

/** �^�U�l */
typedef enum tagLNBool
{
	/** �U */
	LN_FALSE	= 0,

	/** �^ */
	LN_TRUE		= 1,

} LNBool;

typedef enum
{
	LN_OK = 0,

} LNResultCode;

