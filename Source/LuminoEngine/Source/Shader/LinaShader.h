
#pragma once

LN_NAMESPACE_BEGIN

// rawcode �� IRCode + technique data
class LinaShaderIRGenerater
{
public:
	LinaShader();

	void loadRawHLSL(const std::string& code);

	std::string generateIRCode();

private:
};



class LinaShader
{
public:
	LinaShader();

	void loadRawHLSL(const std::string& code);

	// (RawIR:Intermediate Representation)
	// HLSL, GLSL �Ȃǂ̃R�[�h�͈͂������^�O���������ߍ��܂ꂽ�R�[�h�B
	void loadRawIR(const std::string& code);

	// DirectX API �ŃR���p�C���\�� HLSL �R�[�h���o�͂���
	std::string generateHLSLCode();

private:
};

LN_NAMESPACE_END
