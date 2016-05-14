
#include "Internal.h"
#include "Resource.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// InternalResource
//==============================================================================
const String InternalResource::DirectXNotInstalledError(_T("DirectXNotInstalledError"));

void InternalResource::InitializeEngineResource()
{
	SetString(DirectXNotInstalledError, _T("%s ��������܂���ł����B\nDirectX �G���h���[�U�[�����^�C�� ���C���X�g�[�����Ă��������B\nhttps://www.microsoft.com/ja-jp/download/details.aspx?id=35&"));
}

LN_NAMESPACE_END
