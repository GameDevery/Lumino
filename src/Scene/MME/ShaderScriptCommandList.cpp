
#include "../../Internal.h"
#include <Lumino/Graphics/Texture.h>
#include "../SceneNode.h"
#include "ShaderScriptCommandList.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// ShaderScriptCommandList
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderScriptCommandList::ShaderScriptCommandList()
	: m_ownerShader(NULL)
	, m_commandArray()
	//, m_drawingSubsetNumbers()
	, m_parentList(NULL)
	, m_scriptExternalColorPosition(-1)
	, m_clearColor()
	, m_clearDepth(0.0f)
	, m_oldDepthBuffer(NULL)
	, m_oldClearColor()
	, m_oldClearDepth(0.0f)
{
	memset(m_oldRenderTarget, 0, sizeof(m_oldRenderTarget));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderScriptCommandList::~ShaderScriptCommandList()
{
	//LN_FOREACH(ShaderScriptCommandList* list, m_childCommandListArray) {
	//	list->Release();
	//}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Initialize(MMEShader* ownerShader)
{
	m_ownerShader = ownerShader;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::AddChildPassCommandList(ShaderScriptCommandList* commandList)
{
	m_childCommandListArray.Add(commandList);
	commandList->m_parentList = this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void ShaderScriptCommandList::SetDrawingSubsetNumbers(const ArrayList<int>& numbers)
//{
//	m_drawingSubsetNumbers.CopyFrom(numbers);
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Execute(DrawParams& params)
{
	//params.SubsetNumbers = &m_drawingSubsetNumbers;
	PushCurrentState(params);
	InternalExecute(params, 0, 0);
	PopCurrentState(params);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::ExecutePreScriptExternal_Color(DrawParams& params)
{
	LN_THROW(m_scriptExternalColorPosition >= 0, InvalidOperationException);

	//params.SubsetNumbers = &m_drawingSubsetNumbers;
	PushCurrentState(params);
	InternalExecute(params, 0, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::ExecutePostScriptExternal_Color(DrawParams& params)
{
	LN_THROW(m_scriptExternalColorPosition >= 0, InvalidOperationException);

	//params.SubsetNumbers = &m_drawingSubsetNumbers;
	InternalExecute(params, m_scriptExternalColorPosition + 1, 0);
	PopCurrentState(params);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderScriptCommandList::ValidationError ShaderScriptCommandList::CheckValid(MMEScriptClass scriptClass, MMEScriptOrder scriptOrder)
{
	bool onTech = !m_childCommandListArray.IsEmpty();

	int loopNest = 0;
	bool foundScriptExternalColor = false;
	int size = m_commandArray.GetCount();
	for (int i = 0; i < size; ++i)
	{
		Command* cmd = &m_commandArray[i];
		switch (cmd->Type)
		{
		case COMMAND_RenderColorTarget:
		{
			if (cmd->RenderColorTarget.Index < 0 || cmd->RenderColorTarget.Index >= Graphics::Renderer::MaxMultiRenderTargets)
			{
				// �����_�����O�^�[�Q�b�g�̃C���f�b�N�X���s��
				return ValidationError_InvalidRenderTargetIndex;
			}
			break;
		}
		case COMMAND_ScriptExternal_Color:
		{
			if (scriptOrder != MME_SCRORDER_postprocess)
			{
				// �|�X�g�G�t�F�N�g�iSTANDARDSGLOBAL�p�����[�^��ScriptOrder�A�m�e�[�V������
				// "postprocess"���w�肳�ꂽ�G�t�F�N�g�t�@�C���j�ł̂ݎ��s�ł���B
				return ValidationError_ScriptExternal_Color_InvalidCall;
			}
			if (foundScriptExternalColor)
			{
				// ScriptExternal=Color ��1����
				return ValidationError_ScriptExternal_Color_MultipleCall;
			}
			if (loopNest > 0)
			{
				// ScriptExternal=Color �̓��[�v���ɋL�q���邱�Ƃ͂ł��Ȃ�
				return ValidationError_ScriptExternal_Color_CallInLoop;
			}
			foundScriptExternalColor = true;
			break;
		}
		case COMMAND_Pass:
		{
			if (onTech)
			{
				// �e�N�j�b�N�̃X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_Pass_CallByPass;
			}
			break;
		}
		case COMMAND_LoopByCount:
		{
			if (onTech)
			{
				// �e�N�j�b�N�̃X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_Loop_CallByPass;
			}
			++loopNest;
			break;
		}
		case COMMAND_LoopEnd:
		{
			if (onTech)
			{
				// ValidationError_Loop_CallByTechnique�X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_Loop_CallByPass;
			}
			--loopNest;
			if (loopNest < 0)
			{
				// LoopEnd ������
				return ValidationError_LoopBlockUnMath;
			}
			break;
		}
		case COMMAND_LoopGetIndex:
		{
			if (onTech)
			{
				// �e�N�j�b�N�̃X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_Loop_CallByPass;
			}
			break;
		}
		case COMMAND_DrawGeometry:
		{
			if (onTech)
			{
				// �p�X�̃X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_DrawGeometry_CallByTechnique;
			}
			if (scriptClass == MME_SCRCLS_scene)
			{
				// STANDARDSGLOBAL��ScriptClass��"scene"���w�肵�Ă���ꍇ�A
				// ���̃R�}���h�����s���Ă͂Ȃ�Ȃ��B
				return ValidationError_DrawGeometry_InvalidCall;
			}
			break;
		}
		case COMMAND_DrawBuffer:
		{
			if (onTech)
			{
				// �p�X�̃X�N���v�g��ł����g�p�ł��Ȃ��B
				return ValidationError_DrawBuffer_CallByTechnique;
			}
			if (scriptClass == MME_SCRCLS_object)
			{
				// STANDARDSGLOBAL��ScriptClass��"object"���w�肵�Ă���ꍇ�A
				// ���̃R�}���h�����s���Ă͂Ȃ�Ȃ��B
				return ValidationError_DrawBuffer_InvalidCall;
			}
			break;
		}
		}
	}

	return ValidationError_Success;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_RenderColorTarget(int index, Graphics::ShaderVariable* textureVariable)
{
	Command c;
	c.Type = COMMAND_RenderColorTarget;
	c.RenderColorTarget.Index = index;
	c.RenderColorTarget.TextureVariable = textureVariable;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_RenderDepthStencilTarget(Graphics::ShaderVariable* textureVariable)
{
	Command c;
	c.Type = COMMAND_RenderDepthStencilTarget;
	c.RenderDepthStencilTarget.TextureVariable = textureVariable;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_ClearSetColor(const Vector4& color)
{
	Command c;
	c.Type = COMMAND_ClearSetColor;
	c.ClearSetColor.Color[0] = color.X;
	c.ClearSetColor.Color[1] = color.Y;
	c.ClearSetColor.Color[2] = color.Z;
	c.ClearSetColor.Color[3] = color.W;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_ClearSetDepth(float depth)
{
	Command c;
	c.Type = COMMAND_ClearSetDepth;
	c.ClearSetDepth.Depth = depth;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_ClearColor()
{
	Command c;
	c.Type = COMMAND_ClearColor;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_ClearDepth()
{
	Command c;
	c.Type = COMMAND_ClearDepth;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_ScriptExternal_Color()
{
	Command c;
	c.Type = COMMAND_ScriptExternal_Color;
	m_commandArray.Add(c);

	// ���̈ʒu���o���Ă���
	m_scriptExternalColorPosition = m_commandArray.GetCount() - 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_Pass(Graphics::ShaderPass* pass, ShaderScriptCommandList* commandList)
{
	Command c;
	c.Type = COMMAND_Pass;
	c.Pass.Pass = pass;
	c.Pass.CommandList = commandList;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_LoopByCount(int count)
{
	Command c;
	c.Type = COMMAND_LoopByCount;
	c.LoopByCount.Count = count;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_LoopEnd()
{
	Command c;
	c.Type = COMMAND_LoopEnd;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_LoopGetIndex(Graphics::ShaderVariable* variable)
{
	Command c;
	c.Type = COMMAND_LoopGetIndex;
	c.LoopGetIndex.Variable = variable;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_DrawGeometry(Graphics::ShaderPass* pass)
{
	Command c;
	c.Type = COMMAND_DrawGeometry;
	c.DrawGeometry.Pass = pass;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::Add_DrawBuffer(Graphics::ShaderPass* pass)
{
	Command c;
	c.Type = COMMAND_DrawBuffer;
	c.DrawBuffer.Pass = pass;
	m_commandArray.Add(c);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int ShaderScriptCommandList::InternalExecute(DrawParams& params, int pc, int currentLoopCount)
{
	int size = m_commandArray.GetCount();
	for (; pc < size; ++pc)
	{
		Command* cmd = &m_commandArray[pc];
		switch (cmd->Type)
		{
		case COMMAND_RenderColorTarget:
		{
			if (cmd->RenderColorTarget.TextureVariable)
			{
				params.Params->Renderer->SetRenderTarget(cmd->RenderColorTarget.Index, cmd->RenderColorTarget.TextureVariable->GetTexture());
			}
			else
			{
				params.Params->Renderer->SetRenderTarget(cmd->RenderColorTarget.Index, m_oldRenderTarget[cmd->RenderColorTarget.Index]);
			}
			break;
		}
		case COMMAND_RenderDepthStencilTarget:
		{
			if (cmd->RenderDepthStencilTarget.TextureVariable)
			{
				params.Params->Renderer->SetDepthBuffer(cmd->RenderDepthStencilTarget.TextureVariable->GetTexture());
			}
			else
			{
				params.Params->Renderer->SetDepthBuffer(m_oldDepthBuffer);
			}
			break;
		}
		case COMMAND_ClearSetColor:
		{
			m_clearColor.Set(
				cmd->ClearSetColor.Color[0],
				cmd->ClearSetColor.Color[1],
				cmd->ClearSetColor.Color[2],
				cmd->ClearSetColor.Color[3]);
			break;
		}
		case COMMAND_ClearSetDepth:
		{
			m_clearDepth = cmd->ClearSetDepth.Depth;
			break;
		}
		case COMMAND_ClearColor:
		{
			params.Params->Renderer->Clear(true, false, m_clearColor, 0);
			break;
		}
		case COMMAND_ClearDepth:
		{
			params.Params->Renderer->Clear(false, true, Graphics::ColorF::Transparency, m_clearDepth);
			break;
		}
		case COMMAND_ScriptExternal_Color:
		{
			// �I��
			return pc;
		}
		case COMMAND_Pass:
		{
			if (cmd->Pass.CommandList)
			{
				cmd->Pass.CommandList->Execute(params);
			}
			// �R�}���h���X�g���ȗ�����Ă���ꍇ�͒��� Draw=Geometry
			/* Tech ���ō���Ă�̂ł����ł͍s��Ȃ�
			else
			{
			_drawGeometry( cmd->Pass.Pass );
			}
			*/
			break;
		}
		case COMMAND_LoopByCount:
		{
			int lastPC = pc;
			for (int i = 0; i < cmd->LoopByCount.Count; ++i)
			{
				lastPC = InternalExecute(params, pc + 1, i);
			}
			pc = lastPC;
			break;
		}
		case COMMAND_LoopEnd:
		{
			// �Ăяo�����֖߂�
			return pc;
		}
		case COMMAND_LoopGetIndex:
		{
			cmd->LoopGetIndex.Variable->SetInt(currentLoopCount);
			break;
		}
		case COMMAND_DrawGeometry:
		{
			DrawGeometry(params, cmd->DrawGeometry.Pass);
			break;
		}
		case COMMAND_DrawBuffer:
		{
			//Graphics::RenderState state = params.Renderer->GetRenderState();
			//state.DepthTest = false;
			//state.DepthWrite = false;
			//params.Renderer->setRenderState(state);

			if (cmd->DrawBuffer.Pass) {
				cmd->DrawBuffer.Pass->Apply();
			}

			params.Params->GeometryRenderer->DrawSquare(
				-1.0f,	1.0f,	0.0f,	0.0f, 0.0f, Graphics::ColorF::White,	// ����
				1.0f,	1.0f,	0.0f,	1.0f, 0.0f, Graphics::ColorF::White,	// �E��
				-1.0f,	-1.0f,	0.0f,	0.0f, 1.0f, Graphics::ColorF::White,	// ����
				1.0f,	-1.0f,	0.0f,	1.0f, 1.0f, Graphics::ColorF::White);	// �E��
			break;
		}
		}
	}
	return pc - 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::DrawGeometry(DrawParams& params, Graphics::ShaderPass* pass)
{
	params.RenderingNode->DrawSubsetInternal(*params.Params, params.SubsetIndex, m_ownerShader, pass);
#if 0
	// �T�u�Z�b�g�̎w�肪�Ȃ���΂��ׂĕ`�悷��
	if (params.SubsetNumbers->IsEmpty())
	{
		for (int i = 0; i < params.SubsetCount; ++i) {
			params.RenderingNode->DrawSubsetInternal(*params.Params, i, m_ownerShader, pass);
		}
	}
	// �T�u�Z�b�g�͈͂̎w�肪����
	else
	{
		int lastIndex = -1;
		int count = params.SubsetNumbers->GetCount();
		for (int i = 0; i < count; ++i)
		{
			if (params.SubsetNumbers->GetAt(i) == -1) {
				lastIndex = (i > 0) ? params.SubsetNumbers->GetAt(i) : 0;
				break;
			}
			if (params.SubsetNumbers->GetAt(i) < params.SubsetCount) {
				params.RenderingNode->DrawSubsetInternal(*params.Params, i, m_ownerShader, pass);
			}
		}

		// "0-4,6,8-" �̂悤�ɏI�[�� - �̏ꍇ�͈ȍ~�S�ẴT�u�Z�b�g��`�悷��
		if (lastIndex >= 0)
		{
			for (int i = lastIndex; i < params.SubsetCount; ++i) {
				params.RenderingNode->DrawSubsetInternal(*params.Params, i, m_ownerShader, pass);
			}
		}
	}
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::PushCurrentState(DrawParams& params)
{
	// ���݂̃����_�����O�^�[�Q�b�g&�[�x�o�b�t�@���L��
	for (int i = 0; i < Graphics::Renderer::MaxMultiRenderTargets; ++i)
	{
		m_oldRenderTarget[i] = params.Params->Renderer->GetRenderTarget(i);
		LN_SAFE_ADDREF(m_oldRenderTarget[i]);
	}
	m_oldDepthBuffer = params.Params->Renderer->GetDepthBuffer();
	LN_SAFE_ADDREF(m_oldDepthBuffer);

	// �e�̏�Ԃ��L��
	if (m_parentList != NULL)
	{
		m_oldClearColor = m_parentList->m_clearColor;
		m_oldClearDepth = m_parentList->m_clearDepth;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ShaderScriptCommandList::PopCurrentState(DrawParams& params)
{
	// �����_�����O�^�[�Q�b�g&�[�x�o�b�t�@�����ɖ߂�
	for (int i = 0; i < Graphics::Renderer::MaxMultiRenderTargets; ++i)
	{
		params.Params->Renderer->SetRenderTarget(i, m_oldRenderTarget[i]);
		LN_SAFE_RELEASE(m_oldRenderTarget[i]);
	}
	params.Params->Renderer->SetDepthBuffer(m_oldDepthBuffer);
	LN_SAFE_RELEASE(m_oldDepthBuffer);

	// �e�̏�Ԃ����ɖ߂�
	if (m_parentList != NULL)
	{
		m_parentList->m_clearColor = m_oldClearColor;
		m_parentList->m_clearDepth = m_oldClearDepth;
	}
}

} // namespace Scene
} // namespace Lumino


