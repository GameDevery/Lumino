
#include "../../Internal.h"
#include "MMEShader.h"
#include "MMEShaderErrorInfo.h"
#include "MMEShaderTechnique.h"

namespace Lumino
{
namespace Scene
{

#define LN_SET_SHADER_ERR_MSG( fmt_, ... ) \
{ \
    LRefTString err; \
    err.format( fmt_, __VA_ARGS__ ); \
    mSceneShader->setErrorMessage( err ); \
}

//=============================================================================
// MMEShaderTechnique
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique::MMEShaderTechnique()
	: m_errorInfo(NULL)
	, m_ownerShader(NULL)
	, m_coreShader(NULL)
	, m_technique(NULL)
	, m_mmdPass(MMD_PASS_object)
	, m_isUseTexture()
	, m_isUseSphereMap()
	, m_isUseToon()
	, m_isUseSelfShadow()
	, m_scriptCommandList()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique::~MMEShaderTechnique()
{
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses) {
		pass->Release();
	}

	//ln_foreach( DrawingCommandList* list, mPassDrawingCommandListArray )
	//{
	//	SAFE_DELETE( list );
	//}
	//mPassDrawingCommandListArray.clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::Initialize(MMEShader* shader, Graphics::ShaderTechnique* tech, MMEShaderErrorInfo* errorInfo)
{
	m_errorInfo = errorInfo;
	m_ownerShader = shader;
	m_coreShader = m_ownerShader->GetCoreShader();
	m_technique = tech;

	m_scriptCommandList.Initialize(m_ownerShader);

    //-------------------------------------------
    // MMD �p�X�ƃA�m�e�[�V�����𒲂ׂ�

    bool use_texture = false;
    bool use_sphere_map = false;
    bool use_toon = false;
    Graphics::ShaderVariable* anno = NULL;

	anno = m_technique->FindAnnotation(_T("MMDPass"));
    if (anno != NULL)
    {
		const TCHAR* mmd_pass = anno->GetString();
		if (mmd_pass != NULL)
		{
			if (_tcsicmp(mmd_pass, _T("object")) == 0) {
				m_mmdPass = MMD_PASS_object;
			}
			else if (_tcsicmp(mmd_pass, _T("zplot")) == 0) {
				m_mmdPass = MMD_PASS_zplot;
			}
			else if (_tcsicmp(mmd_pass, _T("object_ss")) == 0) {
				m_mmdPass = MMD_PASS_object_ss;
			}
			else if (_tcsicmp(mmd_pass, _T("shadow")) == 0) {
				m_mmdPass = MMD_PASS_shadow;
			}
			else if (_tcsicmp(mmd_pass, _T("edge")) == 0) {
				m_mmdPass = MMD_PASS_edge;
			}
		}
    }

	anno = m_technique->FindAnnotation(_T("UseTexture"));
	if (anno != NULL) {
		m_isUseTexture = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseSphereMap"));
	if (anno != NULL) {
		m_isUseSphereMap = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseToon"));
	if (anno != NULL) {
		m_isUseToon = anno->GetBool();
	}

	anno = m_technique->FindAnnotation(_T("UseSelfShadow"));
	if (anno != NULL) {
		m_isUseSelfShadow = anno->GetBool();
	}


    //-------------------------------------------
    // �`�悷��Subset�ԍ��̎w��𒲂ׂ�

    const TCHAR* subsetText = NULL;

	anno = m_technique->FindAnnotation(_T("Subset"));
	if (anno != NULL) {
		subsetText = anno->GetString();
	}
	if (subsetText != NULL) {
		ParseSubsetIndexArray(subsetText, &m_subsetIndexArray, &m_subsetIndexEndToRange);
    }

	//m_scriptCommandList.SetDrawingSubsetNumbers(subsetIndexArray);
        
    //-------------------------------------------
    // Script

	// ��ɑS�Ă� Pass �̃R�}���h���X�g (���ꕨ) ������Ă����B
	// (�X�N���v�g��͂� "Pass" ���������Ƃ��Ɍ����ł���悤�ɂ��邽��)
	LN_FOREACH(Graphics::ShaderPass* pass, m_technique->GetPasses())
	{
		MMEShaderPass* mmePass = LN_NEW MMEShaderPass();
		mmePass->m_pass = pass;
		mmePass->m_scriptCommandList.Initialize(m_ownerShader);
		m_mmeShaderPasses.Add(mmePass);

		m_scriptCommandList.AddChildPassCommandList(&mmePass->m_scriptCommandList);
	}

    // �e�N�j�b�N�� "Script" �A�m�e�[�V���������邩�`�F�b�N 
    Graphics::ShaderVariable* scriptAnno;
	scriptAnno = m_technique->FindAnnotation(_T("Script"));
	if (scriptAnno != NULL && scriptAnno->GetType() == Graphics::ShaderVariableType_String)
    {
		ParseScriptCommandString(scriptAnno, NULL, &m_scriptCommandList);
    }
    // �ȗ�����Ă���ꍇ�͑S�Ẵp�X�����s����R�}���h���X�g�����
    else
    {
		LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses) {
			m_scriptCommandList.Add_Pass(pass->m_pass, &pass->m_scriptCommandList);
		}
    }

	// �Ō�ɑS�Ẵp�X�̃X�N���v�g����͂���
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses)
	{
		// "Script" �A�m�e�[�V����������Ή�͂��A�ȗ�����Ă���ꍇ�� "DrawGeometry" �����ǉ�����
		scriptAnno = pass->m_pass->FindAnnotation(_T("Script"));
		if (scriptAnno != NULL && scriptAnno->GetType() == Graphics::ShaderVariableType_String) {
			ParseScriptCommandString(scriptAnno, pass->m_pass, &pass->m_scriptCommandList);
		}
		else {
			pass->m_scriptCommandList.Add_DrawGeometry(pass->m_pass);
		}
	}


    //-------------------------------------------
    // ���g�`�F�b�N
#if 0
    printf( "�� Script\n" );
    MMEScriptCommandList::iterator it = mScriptCommandList.begin();
    for ( ; it != mScriptCommandList.end(); ++it )
    {
        MMEScriptCommand* c = &(*it);
        switch ( c->Type )
        {
            case MME_SCRIPTCOM_RenderColorTarget:           
                printf( "RenderColorTarget index:%d var:%p\n", c->Index, c->Variable ); break;
            case MME_SCRIPTCOM_RenderDepthStencilTarget:    
                printf( "RenderDepthStencilTarget\n" ); break;
            case MME_SCRIPTCOM_ClearSetColor:               
                printf( "ClearSetColor\n" ); break;
            case MME_SCRIPTCOM_ClearSetDepth:               
                printf( "ClearSetDepth\n" ); break;
            case MME_SCRIPTCOM_Clear_Color:                 
                printf( "Clear_Color        %X\n", (it)->Color ); break;
            case MME_SCRIPTCOM_Clear_Depth:                 
                printf( "Clear_Depth        %f\n", (it)->Depth ); break;
            case MME_SCRIPTCOM_ScriptExternal_Color:        
                printf( "ScriptExternal_Color\n" ); break;
            case MME_SCRIPTCOM_Pass:                        
                printf( "Pass\n" ); break;
            case MME_SCRIPTCOM_BeginPass:                   
                printf( "BeginPass\n" ); break;
            case MME_SCRIPTCOM_EndPass:                     
                printf( "EndPass\n" ); break;
            case MME_SCRIPTCOM_LoopByCount:                 
                printf( "LoopByCount\n" ); break;
            case MME_SCRIPTCOM_LoopEnd:                     
                printf( "LoopEnd\n" ); break;
            case MME_SCRIPTCOM_LoopGetIndex:                
                printf( "LoopGetIndex\n" ); break;
            case MME_SCRIPTCOM_Draw_Geometry:               
                printf( "Draw_Geometry\n" ); break;
            case MME_SCRIPTCOM_Draw_Buffer:                 
                printf( "Draw_Buffer\n" ); break;
            case MME_SCRIPTCOM_RenderPort:                  
                printf( "RenderPort\n" ); break;
        }  
    }
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MMEShaderTechnique::ContainsSubsetIndex(int subsetIndex) const
{	
	// "Subset" ���ȗ�����Ă���B�S���`��ł���B
	if (m_subsetIndexArray.IsEmpty()) {
		return true;
	}
	// �͈͂̏I�[�� '-' �ɂȂ��Ă���B�C���f�b�N�X�͈͊O�͑S�ĕ`��\�B
	if (subsetIndex >= m_subsetIndexArray.GetCount()) {
		return m_subsetIndexEndToRange;
	}
	return m_subsetIndexArray.GetAt(subsetIndex);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderPass* MMEShaderTechnique::FindMMEShaderPass(const TCHAR* passName)
{
	LN_FOREACH(MMEShaderPass* pass, m_mmeShaderPasses)
	{
		if (pass->m_pass->GetName().Compare(passName)) {
			return pass;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseScriptCommandString(Graphics::ShaderVariable* scriptAnno, Graphics::ShaderPass* thisPass, ShaderScriptCommandList* commandList)
{
    TCHAR* str = NULL;
    //char* s;
    //char* tp;
    //Graphics::ShaderVariable* tv;

	const TCHAR* script = scriptAnno->GetString();
    if ( script )
	{
		// ��؂蕶��(;)���I�[�����ɒu�����ď��������邽�߁A��x���������\�o�b�t�@��s�S���R�s�[����
		ByteBuffer tmpBuf(script);
		TCHAR* cmdLine = (TCHAR*)tmpBuf.GetData();
		while (true)
		{
			// ; ��T���� '\0' �ɒu���B������ΏI�[�Ƃ������ƂŏI��
			int nextSemiColonIdx = StringUtils::IndexOf(cmdLine, _T(";"));
			if (nextSemiColonIdx == -1) { return; }

			cmdLine[nextSemiColonIdx] = _T('\0');

			// �u������ '\0' �܂ŁA(������̕�����v�ł�����) �L�[���[�h��T���Ă���
			switch (cmdLine[0])
            {
                /////////////////////////////
                // RenderColorTarget_0�`3
                // RenderDepthStencilTarget
                // RenderPort
                case _T('R'):
                {
                    ///////////////////////////// RenderColorTarget_0�`3
					if (cmdLine[6] == _T('C'))
                    {
                        // �ݒ�C���f�b�N�X�𒲂ׂ�
                        uint32_t index;
						TCHAR lc = cmdLine[17];
                        if ( '0' <= lc && lc <= '3' ) {
                            index = lc - '0';
                        }
                        else {
                            index = 0;
                        }

                        // = �̌�낪�e�N�X�`���^�ϐ����ɂȂ��Ă���͂�
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx == -1) {									// �e�N�X�`���ϐ������ȗ�����Ă���
							commandList->Add_RenderColorTarget(index, NULL);// �f�t�H���g�ɖ߂��R�}���h
						}
						else
						{
							// �ϐ����������A�R�}���h��
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Texture) {
								commandList->Add_RenderColorTarget(index, v);
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidRenderTargetVariable, &cmdLine[idx + 1]);
								return;
							}
						}
                    }
                    ///////////////////////////// RenderDepthStencilTarget
					else if (cmdLine[6] == _T('D'))
                    {
						// = �̌�낪�e�N�X�`���^�ϐ����ɂȂ��Ă���͂�
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx == -1) {									// �e�N�X�`���ϐ������ȗ�����Ă���
							commandList->Add_RenderDepthStencilTarget(NULL);// �f�t�H���g�ɖ߂��R�}���h
						}
						else
						{
							// �ϐ����������A�R�}���h��
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Texture) {
								commandList->Add_RenderDepthStencilTarget(v);
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidRenderDepthStencilTargetVariable, &cmdLine[idx + 1]);
								return;
							}
						}
                    }
                    ///////////////////////////// RenderPort
					else if (cmdLine[6] == _T('P'))
                    {
                        // MME ���̂��������̃R�}���h�B�����B
                    }
                    break;
                }

                /////////////////////////////
                // ClearSetColor
                // ClearSetDepth
                // Clear_Color
                // Clear_Depth
                case _T('C'):
                {
                    ///////////////////////////// ClearSetColor
					if (cmdLine[8] == _T('C'))
					{
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx != -1)
						{
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Vector)
							{
								commandList->Add_ClearSetColor(v->GetVector());
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetColorVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: �ϐ�����������Ȃ�����
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetColorVariable);
							return;
						}
                    }
                    ///////////////////////////// ClearSetDepth
					else if (cmdLine[8] == _T('D'))
					{
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx != -1)
						{
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Float)
							{
								commandList->Add_ClearSetDepth(v->GetFloat());
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: �ϐ�����������Ȃ�����
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable);
							return;
						}
                    }
                    ///////////////////////////// Clear_Color
					else if (cmdLine[6] == _T('C'))
                    {
						commandList->Add_ClearColor();
                    }
                    ///////////////////////////// Clear_Depth
					else if (cmdLine[6] == _T('D'))
                    {
						commandList->Add_ClearDepth();
                    }
                    break;
                }

                /////////////////////////////
                // ScriptExternal_Color
                case _T('S'):
                {
					commandList->Add_ScriptExternal_Color();
                    break;
                }

                /////////////////////////////
                // Pass
                case _T('P'):
				{
					int idx = StringUtils::IndexOf(cmdLine, _T("="));
					if (idx != -1)
					{
						MMEShaderPass* mmeShaderPass = FindMMEShaderPass(&cmdLine[idx + 1]);
						if (mmeShaderPass != NULL) {
							commandList->Add_Pass(mmeShaderPass->m_pass, &mmeShaderPass->m_scriptCommandList);
						}
						else {
							// Error: �����ȃp�X��
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidPassName, &cmdLine[idx + 1]);
							return;
						}
					}
					else {
						// Error: �����ȃp�X��
						m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidPassName);
						return;
					}
                    break;
                }

                /////////////////////////////
                // LoopByCount
                // LoopEnd
                // LoopGetIndex
                case _T('L'):
                {
                    ///////////////////////////// LoopByCount
					if (cmdLine[4] == _T('B'))
                    {
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx != -1)
						{
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Int)
							{
								commandList->Add_LoopByCount(v->GetInt());
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopByCountVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: �ϐ�����������Ȃ�����
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopByCountVariable);
							return;
						}
                    }
                    ///////////////////////////// LoopEnd
					else if (cmdLine[4] == _T('E'))
                    {
						commandList->Add_LoopEnd();
                    }
                    ///////////////////////////// LoopGetIndex
					else if (cmdLine[4] == _T('G'))
                    {
						int idx = StringUtils::IndexOf(cmdLine, _T("="));
						if (idx != -1)
						{
							Graphics::ShaderVariable* v = m_coreShader->FindVariable(&cmdLine[idx + 1]);
							if (v != NULL && v->GetType() == Graphics::ShaderVariableType_Int)
							{
								commandList->Add_LoopGetIndex(v);
							}
							else {
								// Error: �ϐ�����������Ȃ�����
								m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable, &cmdLine[idx + 1]);
								return;
							}
						}
						else {
							// Error: �ϐ�����������Ȃ�����
							m_errorInfo->AddError(MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable);
							return;
						}
                    }
                    break;
                }

                /////////////////////////////
                // Draw_Geometry
                // Draw_Buffer
                case _T('D'):
				{
					///////////////////////////// Draw_Geometry
					if (cmdLine[5] == _T('G'))
					{
						commandList->Add_DrawGeometry(thisPass);
					}
                    ///////////////////////////// Draw_Buffer
					else if (cmdLine[5] == _T('B'))
                    {
						commandList->Add_DrawBuffer(thisPass);
                    }
                    break;
                }
            }

			cmdLine += nextSemiColonIdx + 1;	// ; �̎����w��
		}
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseSubsetIndexArray(const String& subsetText, Array<bool>* subsetIndexArray, bool* endToRange)
{
	*endToRange = false;
	Array<String> tokens = StringUtils::Split(String(subsetText), _T(","), StringSplitOptions_RemoveEmptyEntries);

	LN_FOREACH(String& token, tokens)
	{
		if (*endToRange) {
			// TODO: ���@�G���[�B�Ōオ '-' �ł���g�[�N���͍Ō�̃g�[�N���łȂ���΂Ȃ�Ȃ�
			return;
		}

		int rangeMarkPos = token.IndexOf(_T("-"));

		// '-' �̖������ʂ̐��l������
		if (rangeMarkPos == -1)
		{
			int n = _ttoi(token);
			while (subsetIndexArray->GetCount() < n - 1) {	// �T�u�Z�b�g�ԍ� n �܂łɑ���Ȃ��v�f���l�߂Ă���
				subsetIndexArray->Add(false);
			}
			subsetIndexArray->Add(true);					// n �Ԃ̃T�u�Z�b�g�͕`��ł���
		}
		// �g�[�N���̍Ōオ '-' �������B�ȍ~�S�ẴT�u�Z�b�g��`�悷�邱�Ƃ�����
		else if (rangeMarkPos == token.GetLength() - 1)
		{
			int n = _ttoi(token.Left(rangeMarkPos));
			while (subsetIndexArray->GetCount() < n - 1) {	// �T�u�Z�b�g�ԍ� n �܂łɑ���Ȃ��v�f���l�߂Ă���
				subsetIndexArray->Add(false);
			}
			subsetIndexArray->Add(true);					// n �Ԃ̃T�u�Z�b�g�͕`��ł���
			*endToRange = true;								// �ȍ~�A�S�ẴT�u�Z�b�g��`�悷�邱�Ƃ�����
		}
		// "2-5" �̂悤�ȃg�[�N��
		else
		{
			int start = _ttoi(token.Left(rangeMarkPos));
			int end = _ttoi(token.Right((token.GetLength() - rangeMarkPos) - 1));
			while (subsetIndexArray->GetCount() < start - 1) {	// �T�u�Z�b�g�ԍ� start �܂łɑ���Ȃ��v�f���l�߂Ă���
				subsetIndexArray->Add(false);
			}
			while (subsetIndexArray->GetCount() < end) {		// �T�u�Z�b�g end �Ԃ܂ŕ`��ł���
				subsetIndexArray->Add(true);
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderTechnique::ParseSubsetIndexArray(const TCHAR* subsetText, Array<int>* subsetIndexArray)
{
	struct SubsetIndexOrder
	{
		int	Index;
		int	RangeState;    ///< �͈͂̎w�� (0=�Ȃ� 1=����SubsetIndex�̒l�܂� 2=�T�u�Z�b�g���̍ő�܂�)
	};
	typedef std::list< SubsetIndexOrder > SubsetIndexOrderList;


	const TCHAR* c;
	const TCHAR* lc; // �Ō�Ɍ������� ',' �܂͂� '-' �̎��̕���
	int   num;
	int   last_num = 0;
	bool  is_range = false;     // - �ɂ���Ĕ͈͎w�肪����ꍇ true
	TCHAR	temp[64];
	uint32_t   size = 0;             // ���ۂɓW�J�����z��̗v�f��
	SubsetIndexOrderList    subse_index_order_list;

	SubsetIndexOrder si;
	SubsetIndexOrder last_si;
	last_si.Index = 0;
	last_si.RangeState = 0;

	c = subsetText;
	lc = c;

	// �R���}���I�[�܂ŒT�����[�v
	while (true)
	{
		if (*c == _T(',') || *c == _T('\0'))
		{
			StringUtils::StrNCpy(temp, 64, lc, c - lc);

			temp[c - lc] = _T('\0');
			num = _ttoi(temp);
			lc = c + 1;

			si.Index = num;

			// �ЂƂO�̕����� - �̏ꍇ�̓T�u�Z�b�g�I�[�܂ł͈͎̔w��
			if (*(c - 1) == _T('-'))
			{
				si.RangeState = 2;
				size += 2;          // ���l�ƁA�I�[�܂ł�\���l 0xffffffff ���i�[���镪
			}
			else
			{
				si.RangeState = 0;

				// �ЂƂO����͈͎̔w��̏ꍇ
				if (last_si.RangeState == 1)
				{
					size += (si.Index - last_si.Index) + 1;   // ���̐������܂ނ̂� + 1  ( i < num �ł͂Ȃ� i <= num )
				}
				// �ЂƂ̐��l�̏ꍇ
				else
				{
					++size;
				}
			}

			subse_index_order_list.push_back(si);
			last_si = si;   // ����Ȃ�����

			// �I�[���A�T�u�Z�b�g�̏I�[�܂œǂނ��̂����������ꍇ�͂����ŏI��
			if (*c == _T('\0') || si.RangeState == 2)
			{
				break;
			}
		}
		// - �����������ꍇ�͂ЂƂO�� , (�܂��͐擪) ����𐔒l�ɂ��Ă���
		else if (*c == _T('-'))
		{
			StringUtils::StrNCpy(temp, 64, lc, c - lc);
			temp[c - lc] = _T('\0');
			last_num = _ttoi(temp);
			is_range = true;
			lc = c + 1;

			SubsetIndexOrder si;
			si.Index = last_num;
			si.RangeState = true;
			subse_index_order_list.push_back(si);

			last_si = si;
		}
		++c;
	}



	// �쐬�Ɏ��s�����ꍇ
	if (subse_index_order_list.empty())
	{
		// ��ɂ���
		subsetIndexArray->Clear();
	}
	else
	{
		subsetIndexArray->Resize(size);

		uint32_t idx = 0;
		SubsetIndexOrderList::iterator  it1 = subse_index_order_list.begin();
		SubsetIndexOrderList::iterator end1 = subse_index_order_list.end();
		for (; it1 != end1;)
		{
			// ���̒l�܂ł͈͎̔w��
			if (it1->RangeState == 1)
			{
				int i = it1->Index;
				++it1;

				// �����T�u�Z�b�g���܂ł͈͎̔w��̏ꍇ
				if (it1->RangeState == 2)
				{
					(*subsetIndexArray)[idx] = i;
					++idx;

					(*subsetIndexArray)[idx] = 0xffffffff;
					// ��쓮�h�~�̂��߁A�����ŏI���Bit1->RangeState == 2 �̂Ƃ��A�I�[�͕K�� 0xffffffff �ɂȂ�B
					break;
				}
				else
				{
					for (; (i <= it1->Index); ++i)
					{
						(*subsetIndexArray)[idx] = i;
						++idx;
					}
					++it1;
				}
			}
			// ���ʂ̒l
			else
			{
				(*subsetIndexArray)[idx] = it1->Index;
				++idx;
				++it1;
			}
		}
	}
}

} // namespace Scene
} // namespace Lumino
