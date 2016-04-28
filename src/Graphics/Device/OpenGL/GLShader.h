﻿
#pragma once 
#include "../ShaderVariableBase.h"
#include "GLRenderer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class GLShader;
class GLShaderVariable;
class GLShaderAnnotation;
class GLShaderTechnique;
class GLShaderPass;

struct ShaderDiag
{
	ShaderCompileResultLevel	level;
	StringA						message;
};

/// Pass が利用するシェーダ変数と、その GLSL Location
struct GLShaderPassVariableInfo
{
	GLShaderVariable*	Variable;
	int					Location;
	int					TextureStageIndex;
};

class GLSLUtils
{
public:
	static ShaderCompileResultLevel MakeShaderProgram(const char* vsCode, size_t vsCodeLen, const char* fsCode, size_t fsCodeLen, GLuint* outProgram, StringA* outMessage);
	static bool CompileShader(GLuint shader, int codeCount, const char** codes, const GLint* sizes, StringA* log);
};

/// OpenGL 用の IShader の実装
class GLShader
	: public IShader
{
public:
	GLShader();
	virtual ~GLShader();
	void Initialize(GLGraphicsDevice* device, const void* code, size_t codeByteCount);

	GLGraphicsDevice* GetGraphicsDevice() { return m_device; }
	GLShaderVariable* FindShaderVariable(const String& name);
	GLShaderVariable* CreateShaderVariable(ShaderVariableBase::ShaderVariableTypeDesc desc, const String& name, const String& semanticName);

public:
	virtual int GetVariableCount() const;
	virtual IShaderVariable* GetVariable(int index) const;
	virtual int GetTechniqueCount() const;
	virtual IShaderTechnique* GetTechnique(int index) const;
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	ShaderDiag* GetDiag() { return &m_diag; }
	GLuint GetVertexShader(const String& name);
	GLuint GetFlagmentShader(const String& name);

private:
	GLuint CompileShader(const char* code, size_t codeLen, const char* entryName, GLuint type);

	GLGraphicsDevice*				m_device;
	//GLuint							m_glProgram;
	StringA						m_lastMessage;

	ShaderDiag					m_diag;

	Array<GLShaderVariable*>	m_variables;
	Array<GLShaderTechnique*>	m_techniques;
	std::map<String, GLuint>	m_glVertexShaderEntryMap;
	std::map<String, GLuint>	m_glPixelShaderEntryMap;
};

/// OpenGL 用の IShaderVariable の実装
class GLShaderVariable
	: public ShaderVariableBase
{
public:
	static GLShaderVariable* Deserialize(GLShader* ownerShader, JsonReader2* json, bool* outOverwrited);

	GLShaderVariable();
	virtual ~GLShaderVariable();
	void Initialize(GLShader* owner, ShaderVariableTypeDesc desc, const String& name, const String& semanticName, GLint location);

public:
	/// 指定された GLSL Location に値を設定する
	void Apply(int location, int textureStageIndex);

public:
	virtual int GetAnnotationCount();
	virtual IShaderVariable* GetAnnotation(int index);

public:
	static void ConvertVariableTypeGLToLN(GLenum gl_type, GLsizei gl_var_size, ShaderVariableBase::ShaderVariableTypeDesc* desc);

private:
	GLShader*					m_ownerShader;
	GLint						m_glUniformLocation;
	//Array<SamplerStatePair>		m_samplerStatus;
	Array<GLShaderAnnotation*>	m_annotations;
};

// アノテーション
class GLShaderAnnotation
	: public ShaderVariableBase
{
public:
	static GLShaderAnnotation* Deserialize(JsonReader2* json);

	GLShaderAnnotation();
	virtual ~GLShaderAnnotation();
	void Initialize(const String& type, const String& name, const String& value);

	virtual void SetBool(bool value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetInt(int value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetFloat(float value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetVector(const Vector4& vec) { LN_THROW(0, InvalidOperationException); }
	virtual void SetVectorArray(const Vector4* vectors, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void SetMatrix(const Matrix& matrix) { LN_THROW(0, InvalidOperationException); }
	virtual void SetMatrixArray(const Matrix* matrices, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void SetTexture(ITexture* texture) { LN_THROW(0, InvalidOperationException); }
	virtual int GetAnnotationCount() { return 0; }
	virtual IShaderVariable* GetAnnotation(int index) { return NULL; }
};

// Technique
class GLShaderTechnique
	: public IShaderTechnique
{
public:
	static GLShaderTechnique* Deserialize(GLShader* ownerShader, JsonReader2* json);

	GLShaderTechnique();
	virtual ~GLShaderTechnique();
	void Initialize(GLShader* ownerShader, const String& name);
	void AddPass(GLShaderPass* pass) { m_passes.Add(pass); }

public:
	virtual const TCHAR* GetName() const { return m_name.c_str(); }
	virtual int GetPassCount() const { return m_passes.GetCount(); }
	virtual IShaderPass* GetPass(int index);
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }

private:
	GLShader*					m_ownerShader;
	String						m_name;
	Array<GLShaderPass*>		m_passes;
	Array<GLShaderAnnotation*>	m_annotations;
};

/// OpenGL 用の IShaderPass の実装
class GLShaderPass
	: public IShaderPass
{
public:
	static const int MaxUsageIndex = 16;		///< UsageIndex の最大 (DX9 にあわせて最大の 16)

public:
	static GLShaderPass* Deserialize(GLShader* ownerShader, JsonReader2* json);

	GLShaderPass();
	virtual ~GLShaderPass();
	void Initialize(GLShader* ownerShader, const String& name, GLuint vertShader, GLuint fragShader);
	void Initialize(GLShader* ownerShader, const String& name, const String& vertShaderName, const String& fragShaderName);

public:
	/// glVertexAttribPointer() に指定する attribute インデックスを取得する (-1 の場合は存在しないことを示す)
	int GetUsageAttributeIndex(VertexElementUsage usage, int index);

public:
	virtual const TCHAR* GetName() const { return m_name.c_str(); }
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }
	virtual void Apply();
	//virtual void End();

private:
	bool LinkShader(GLuint vertShader, GLuint fragShader, ShaderDiag* diag);
	void Build();
	
	GLShader*						m_ownerShader;
	GLuint							m_program;
	String							m_name;
	Array<GLShaderAnnotation*>		m_annotations;
	int8_t							m_usageAttrIndexTable[VertexElementUsage_Max][MaxUsageIndex];
	Array<GLShaderPassVariableInfo>	m_passVarList;		// この Pass が本当に使う変数のリスト。最適化されていれば消えるものもある。
	//int								m_textureVarCount;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
