
#pragma once

#include "MMETypes.h"

namespace Lumino
{
namespace Scene
{

enum MMEShaderError
{
	MMEShaderError_Success = 0,

	MMEShaderError_ScriptCommand_InvalidRenderTargetVariable,				///< "RenderColorTarget" �R�}���h�Ńe�N�X�`���^�ϐ���������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidRenderDepthStencilTargetVariable,	///< "RenderDepthStencilTarget" �R�}���h�Ńe�N�X�`���^�ϐ���������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidClearSetColorVariable,				///< "ClearSetColor" �R�}���h�� Vector �^�ϐ���������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable,				///< "ClearSetDepth" �R�}���h�� Vector �^�ϐ���������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidPassName,							///< "Pass" �R�}���h�ŗL���ȃp�X����������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidLoopByCountVariable,				///< "LoopByCount" �R�}���h�� Int �^�ϐ���������Ȃ�����
	MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable,				///< "LoopGetIndex" �R�}���h�� Int �^�ϐ���������Ȃ�����
};

/// MMEShaderErrorObject
class MMEShaderErrorObject
{
public:
	MMEShaderError	ErrorCode;
	String			Message;
};
	
/// MMEShaderErrorInfo
class MMEShaderErrorInfo
{
public:
	void AddError(MMEShaderError errorCode);
	void AddError(MMEShaderError errorCode, const String& message);

private:
	ArrayList<MMEShaderErrorObject>	m_errorList;
};

} // namespace Scene
} // namespace Lumino
