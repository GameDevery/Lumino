﻿
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "Internal.hpp"
#include <sstream>
#include <glad/glad.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Include/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>
#include <spirv_cross/spirv_glsl.hpp>
#include "../Grammar/CppLexer.hpp"
#include <LuminoEngine/Shader/ShaderHelper.hpp>
#include "ShaderManager.hpp"
#include "ShaderTranspiler.hpp"

namespace ln {
namespace detail {

// from glslang: StanAalone/ResourceLimits.cpp
const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

// from glslang/StandAlone/StandAlone.cpp
// Add things like "#define ..." to a preamble to use in the beginning of the shader.
class TPreamble
{
public:
    TPreamble() {}

    bool isSet() const { return text.size() > 0; }
    const char* get() const { return text.c_str(); }
    const std::vector<std::string>& prepro() const { return processes; }

    // #define...
    void addDef(std::string def)
    {
        text.append("#define ");
        fixLine(def);

        processes.push_back("D");
        processes.back().append(def);

        // The first "=" needs to turn into a space
        const size_t equal = def.find_first_of("=");
        if (equal != def.npos)
            def[equal] = ' ';

        text.append(def);
        text.append("\n");
    }

    // #undef...
    void addUndef(std::string undef)
    {
        text.append("#undef ");
        fixLine(undef);

        processes.push_back("U");
        processes.back().append(undef);

        text.append(undef);
        text.append("\n");
    }

protected:
    void fixLine(std::string& line)
    {
        // Can't go past a newline in the line
        const size_t end = line.find_first_of("\n");
        if (end != line.npos)
            line = line.substr(0, end);
    }

    std::string text;                   // contents of preamble
    std::vector<std::string> processes; // what should be recorded by OpModuleProcessed, or equivalent
};

//=============================================================================
// LocalIncluder

class LocalIncluder
    : public glslang::TShader::Includer
{
public:
    ShaderManager* m_manager;
    const List<Path>* includeDirs;

    virtual IncludeResult* includeSystem(const char* headerName, const char* sourceName, size_t inclusionDepth)
    {
        for (auto& pair : m_manager->builtinShaderList()) {
            if (pair.first == headerName) {
                return new IncludeResult(headerName, pair.second.c_str(), pair.second.size(), nullptr);
            }
        }

        return nullptr;
    }

    virtual IncludeResult* includeLocal(const char* headerName, const char* sourceName, size_t inclusionDepth)
    {
        for (auto& dir : (*includeDirs)) {
            auto path = Path(dir, String::fromCString(headerName));
            if (FileSystem::existsFile(path)) {
                ByteBuffer* buf = new ByteBuffer(FileSystem::readAllBytes(path));
                return new IncludeResult(path.str().toStdString(), (const char*)buf->data(), buf->size(), buf);
            }
        }

        return nullptr;
    }

    virtual void releaseInclude(IncludeResult* result)
    {
        if (result) {
            if (result->userData) {
                delete reinterpret_cast<ByteBuffer*>(result->userData);
            }
            delete result;
        }
    }
};

//=============================================================================
// ShaderCodeTranspiler

struct OpenGLTypeConvertionItem
{
	GLenum glType;
	ShaderUniformType lnType;
	//uint32_t vectorElements;
	//uint32_t matrixRows;
	//uint32_t matrixColumns;
};
static const OpenGLTypeConvertionItem s_openGLTypeConvertionTable[] =
{
	{ GL_FLOAT, ShaderUniformType_Float },
	{ GL_FLOAT_VEC2, ShaderUniformType_Vector },
	{ GL_FLOAT_VEC3, ShaderUniformType_Vector },
	{ GL_FLOAT_VEC4, ShaderUniformType_Vector },
	{ GL_INT, ShaderUniformType_Int },
	{ GL_BOOL, ShaderUniformType_Bool },
	{ GL_FLOAT_MAT2, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT3, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT4, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT2x3, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT2x4, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT3x2, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT3x4, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT4x2, ShaderUniformType_Matrix },
	{ GL_FLOAT_MAT4x3, ShaderUniformType_Matrix },
	{ GL_SAMPLER_2D, ShaderUniformType_Texture },		// HLSL で texture2D 型の uniform を定義すると、glslang からは GL_SAMPLER_XXXX として得られる
	{ GL_SAMPLER_CUBE, ShaderUniformType_Texture },
	{ GL_SAMPLER_3D, ShaderUniformType_Texture },
};
static bool GLTypeToLNUniformType(GLenum value, const glslang::TType* type, uint16_t* outType)
{
	if (value == 0) {
		// see glslang\glslang\Include\Types.h TSampler::getString()
		if (type->getBasicType() == glslang::EbtSampler) {
			const glslang::TSampler& sampler = type->getSampler();
			if (sampler.sampler) {
				*outType = ShaderUniformType_SamplerState;
				return true;
			}
		}
	}
	else {
		for (int i = 0; i < LN_ARRAY_SIZE_OF(s_openGLTypeConvertionTable); i++) {
			if (s_openGLTypeConvertionTable[i].glType == value) {
				*outType = s_openGLTypeConvertionTable[i].lnType;
				return true;
			}
		}
	}
	return false;
}

void ShaderCodeTranspiler::initializeGlobals()
{
    glslang::InitializeProcess();
}

void ShaderCodeTranspiler::finalizeGlobals()
{
    glslang::FinalizeProcess();
}

ShaderCodeTranspiler::ShaderCodeTranspiler(ShaderManager* manager)
    : m_manager(manager)
    , m_stage(ShaderCodeStage::Vertex)
{
}

bool ShaderCodeTranspiler::parseAndGenerateSpirv(
    ShaderCodeStage stage,
    const char* code,
    size_t length,
    const std::string& entryPoint,
    const List<Path>& includeDir,
    const List<String>* definitions,
    DiagnosticsManager* diag)
{
    m_stage = stage;
    m_refrection = makeRef<UnifiedShaderRefrectionInfo>();

    LocalIncluder includer;
    includer.m_manager = m_manager;
    includer.includeDirs = &includeDir;

    TPreamble preamble;
    if (definitions) {
        for (auto& def : *definitions) {
            preamble.addDef(def.toStdString());
        }
    }

    // -d オプション
    //const int defaultVersion = Options & EOptionDefaultDesktop ? 110 : 100;
    const int defaultVersion = 110;

    glslang::EShSource sourceType = glslang::EShSourceHlsl;
    const int ClientInputSemanticsVersion = 410; //320;
    glslang::EshTargetClientVersion OpenGLClientVersion = glslang::EShTargetOpenGL_450;
    bool forwardCompatible = true;
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    EShLanguage lang;
    switch (stage) {
        case ShaderCodeStage::Vertex:
            lang = EShLanguage::EShLangVertex;
            break;
        case ShaderCodeStage::Fragment:
            lang = EShLanguage::EShLangFragment;
            break;
        default:
            LN_NOTIMPLEMENTED();
            break;
    }

    // この２つは開放順が重要。TProgram の ヘッダや Standalone.cpp CompileAndLinkShaderUnits の一番下に書いてある。
    glslang::TShader shader(lang);
    glslang::TProgram program;

    // parse
    {
        const char* shaderCode[1] = {code};
        const int shaderLenght[1] = {static_cast<int>(length)};
        const char* shaderName[1] = {"shadercode"};
        shader.setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);
        shader.setEntryPoint(entryPoint.c_str());

        shader.setEnvInput(sourceType, lang, glslang::EShClientOpenGL, ClientInputSemanticsVersion);
        shader.setEnvClient(glslang::EShClientOpenGL, OpenGLClientVersion);
        /* Vulkan Rule
		shader->setEnvInput((Options & EOptionReadHlsl) ? glslang::EShSourceHlsl : glslang::EShSourceGlsl,
		compUnit.stage, glslang::EShClientVulkan, ClientInputSemanticsVersion);
		shader->setEnvClient(glslang::EShClientVulkan, VulkanClientVersion);
		*/
        //shader.setShiftBinding(glslang::EResUbo, 0);
        shader.setShiftBinding(glslang::EResSampler, 1);	// sampler state
		shader.setShiftBinding(glslang::EResTexture, 1);	// texture
			//shader.setShiftBinding(glslang::EResImage, 1);
			//	shader.setShiftBinding(glslang::EResUbo, 1);
			//		shader.setShiftBinding(glslang::EResSsbo, 1);
			//			shader.setShiftBinding(glslang::EResUav, 1);
        //shader.setShiftBindingForSet(glslang::EResUbo, 0, 0);
        //shader.setShiftBindingForSet(glslang::EResSampler, 1, 1);

		// 実際に使われている UBO, Txture, SamplerState などに、自動的に binding 番号を振る。
		// false の場合は、すべて未指定 (-1) となる。
		// 値は program.getUniformBinding() で確認できる。
		shader.setAutoMapBindings(true);
		//shader.setAutoMapLocations(true);
		//shader.setHlslIoMapping(true);

        if (preamble.isSet()) {
            shader.setPreamble(preamble.get());
        }
        shader.addProcesses(preamble.prepro());

        /* TODO: parse でメモリリークしてるぽい。EShLangFragment の時に発生する。
			Dumping objects ->
			{12053} normal block at 0x06EB1410, 8 bytes long.
			 Data: <k       > 6B 0F 00 00 FF FF FF FF 
		*/
        if (!shader.parse(&DefaultTBuiltInResource, defaultVersion, forwardCompatible, messages, includer)) {
            if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportError(shader.getInfoLog());
            if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportError(shader.getInfoDebugLog());
            return false;
        } else if (shader.getInfoLog()) {
            if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportWarning(shader.getInfoLog());
            if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportWarning(shader.getInfoDebugLog());
        }
    }

    // link
    {
        program.addShader(&shader);

        if (!program.link(messages)) {
            if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportError(shader.getInfoLog());
            if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportError(shader.getInfoDebugLog());
            return false;
        } else if (shader.getInfoLog()) {
            if (!StringHelper::isNullOrEmpty(shader.getInfoLog())) diag->reportWarning(shader.getInfoLog());
            if (!StringHelper::isNullOrEmpty(shader.getInfoDebugLog())) diag->reportWarning(shader.getInfoDebugLog());
        }

        if (!program.mapIO()) {
            LN_NOTIMPLEMENTED();
        }
    }

    program.buildReflection();
    program.dumpReflection();

	// get input attributes
	{
		m_attributes.clear();

		for (int iAttr = 0; iAttr < program.getNumLiveAttributes(); iAttr++)
		{
			auto name = program.getAttributeName(iAttr);
			const glslang::TType* tt = program.getAttributeTType(iAttr);
			const glslang::TQualifier& qual = tt->getQualifier();

			VertexInputAttribute attr;
			int keywordLen = 0;
			if (strnicmp(qual.semanticName, "POSITION", 8) == 0) {
				attr.usage = AttributeUsage_Position;
				keywordLen = 8;
			}
			else if (strnicmp(qual.semanticName, "BLENDWEIGHT", 11) == 0) {
				attr.usage = AttributeUsage_BlendWeight;
				keywordLen = 11;
			}
			else if (strnicmp(qual.semanticName, "BLENDINDICES", 12) == 0) {
				attr.usage = AttributeUsage_BlendIndices;
				keywordLen = 12;
			}
			else if (strnicmp(qual.semanticName, "NORMAL", 6) == 0) {
				attr.usage = AttributeUsage_Normal;
				keywordLen = 6;
			}
			else if (strnicmp(qual.semanticName, "TEXCOORD", 8) == 0) {
				attr.usage = AttributeUsage_TexCoord;
				keywordLen = 8;
			}
			else if (strnicmp(qual.semanticName, "TANGENT", 7) == 0) {
				attr.usage = AttributeUsage_Tangent;
				keywordLen = 7;
			}
			else if (strnicmp(qual.semanticName, "BINORMAL", 8) == 0) {
				attr.usage = AttributeUsage_Binormal;
				keywordLen = 8;
			}
			else if (strnicmp(qual.semanticName, "COLOR", 5) == 0) {
				attr.usage = AttributeUsage_Color;
				keywordLen = 5;
			}

			attr.index = atoi(qual.semanticName + keywordLen);
			attr.layoutLocation = qual.layoutLocation;

			m_attributes.push_back(attr);
		}



		// $Global
		for (int i = 0; i < program.getNumLiveUniformBlocks(); i++)
		{
			ShaderUniformBufferInfo info;
			info.name = program.getUniformBlockName(i);
			info.size = program.getUniformBlockSize(i);

			LN_LOG_VERBOSE << "UniformBuffer[" << i << "] : ";
			LN_LOG_VERBOSE << "  name : " << info.name;
			LN_LOG_VERBOSE << "  size : " << info.size;
            LN_LOG_VERBOSE << "  bindingIndex : " << program.getUniformBlockBinding(i);

            m_refrection->buffers.push_back(std::move(info));
		}



		

		// ```
		// float4 g_color1;
		// cbuffer ConstBuff : register(b0) { float4 g_color2; };
		// ```
		// というコードがある場合、g_color1 は "g_color1"、"g_color2" は "g_color2" というように、そのままの名前で取得。
		//
		//
		// ```
		// cbuffer ConstBuff1 {
		//     float4 g_pos1;
		//     float4 g_pos2;
		// };
		// cbuffer ConstBuff2 {
		//     float4 g_pos3;
		//     float4 g_pos4;
		// };
		// vsMain() {
		//     return g_pos2;
	    // }
		// ```
		// というように g_pos2 だけ使われている場合、
		// - ConstBuff1 は Live となる
		// - ConstBuff2 は Live ではないので getNumLiveUniformBlocks() には含まれない
		// - g_pos1 も Live ではないので getNumLiveUniformVariables には含まれない
		// - ConstBuff1 は size = 32(vec4 2つ分) で Live
		// - g_pos2 はバッファ先頭からのオフセット 16
		// 必要な分の最小限だけ Live になる。ただし、UniformBuffer の領域は維持される。
		//
		// また、これらはシェーダ単位で行われる。OpenGL でいうところの、VertexShader と PixelShader をリンクした後の program ではないので注意。
		//
        for (int i = 0; i < program.getNumLiveUniformVariables(); i++)
        {

			ShaderUniformInfo info;
			info.name = program.getUniformName(i);

			const glslang::TType* type = program.getUniformTType(i);
			GLenum gltype = program.getUniformType(i);
			if (!GLTypeToLNUniformType(gltype, type, &info.type)) {
				diag->reportError("Invalid type.");
				return false;
			}

			//info.size = 0;
			info.offset = program.getUniformBufferOffset(i);

			info.vectorElements = type->getVectorSize();
			info.arrayElements = program.getUniformArraySize(i);
			info.matrixRows = type->getMatrixRows();
			info.matrixColumns = type->getMatrixCols();

			//auto aa = type->getTypeName();
			//int a = GL_FLOAT_VEC4;
			

			LN_LOG_VERBOSE << "Uniform[" << i << "] : ";
			LN_LOG_VERBOSE << "  name : " << info.name;
			LN_LOG_VERBOSE << "  type : " << info.type;
			LN_LOG_VERBOSE << "  basicType : " << type->getBasicTypeString();
			LN_LOG_VERBOSE << "  basicString : " << type->getBasicString();
			LN_LOG_VERBOSE << "  offset : " << info.offset;
			LN_LOG_VERBOSE << "  bindingIndex : " << program.getUniformBinding(i);
			LN_LOG_VERBOSE << "  vectorElements : " << info.vectorElements;
			LN_LOG_VERBOSE << "  arrayElements : " << info.arrayElements;
			LN_LOG_VERBOSE << "  matrixRows : " << info.matrixRows;
			LN_LOG_VERBOSE << "  matrixColumns : " << info.matrixColumns;

			


			int ownerUiformBufferIndex = program.getUniformBlockIndex(i);
			if (ownerUiformBufferIndex >= 0)
			{
				LN_LOG_VERBOSE << "  ownerUiformBufferIndex : " << ownerUiformBufferIndex << "(" << program.getUniformBlockName(ownerUiformBufferIndex) << ")";

                m_refrection->buffers[ownerUiformBufferIndex].members.push_back(std::move(info));
			}
			else {
				// TODO: texture など
			}
        }
	}

    glslang::GlslangToSpv(*program.getIntermediate(lang), m_spirvCode);


    spirv_cross::CompilerGLSL glsl(m_spirvCode);
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

#if 0   // binding のデフォルト値調査。・・・といっても、全部 0 だった。
    for (size_t i = 0; i < resources.uniform_buffers.size(); i++) {
        std::cout << "  name : " << resources.uniform_buffers[i].name << std::endl;
        std::cout << "  DecorationBinding : " << glsl.get_decoration(resources.uniform_buffers[i].id, spv::DecorationBinding) << std::endl;
        ;

        //glsl.unset_decoration(resources.uniform_buffers[i].id, spv::DecorationBinding);
    }
#endif

    return true;
}

std::vector<byte_t> ShaderCodeTranspiler::spirvCode() const
{
	auto begin = reinterpret_cast<const byte_t*>(m_spirvCode.data());
	auto end = begin + (m_spirvCode.size() * sizeof(uint32_t));
	return std::vector<byte_t>(begin, end);
}

std::vector<byte_t> ShaderCodeTranspiler::generateGlsl(uint32_t version, bool es)
{
    spirv_cross::CompilerGLSL glsl(m_spirvCode);
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // Set some options.
    spirv_cross::CompilerGLSL::Options options;
    options.version = version;
    options.es = es;
    glsl.set_common_options(options);

    // DescriptorSet は Vulkan 固有のものであるため、他のAPIがバインディングを理解できるように再マップする。
    // https://github.com/KhronosGroup/SPIRV-Cross#descriptor-sets-vulkan-glsl-for-backends-which-do-not-support-them-hlslglslmetal
    for (auto& resource : resources.sampled_images) {
        unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
        unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
        LN_LOG_VERBOSE << "Image " << resource.name.c_str() << " as set = " << set << ", binding = " << binding;

        glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
    }

    // HLSL で register(b0) のような指定をすると binding の値が生成され、 GLSL に出力すると
    //   layout(binding = 0, std140) uniform ConstBuff
    // というようなコードが出力される。
    // しかし、macOS (Mojave) では binding には対応しておらず、
    //   unknown identifer 'binding' in layout
    // というエラーになってしまう。
    // そこで、binding の decoration は削除してしまう。
    // こうしておくと、glsl.compile() で binding 指定を含まない GLSL を生成することができる。
    for (size_t i = 0; i < resources.uniform_buffers.size(); i++) {
        glsl.unset_decoration(resources.uniform_buffers[i].id, spv::DecorationBinding);
    }

    // HLSL では Texture と SamplerState は独立しているが、GLSL では統合されている。
    // ここでは "キーワード + Texture名 + SamplerState名" というような名前を付けておく。
    // 実行時に GLSLShader の中で uniform を列挙してこの規則の uniform を見つけ、実際の Texture と SamplerState の対応付けを行う。
    std::vector<std::string> combinedImageSamplerNames;
    {
        // From main.cpp
        // Builds a mapping for all combinations of images and samplers.
        glsl.build_combined_image_samplers();

        // Give the remapped combined samplers new names.
        // Here you can also set up decorations if you want (binding = #N).
        for (auto& remap : glsl.get_combined_image_samplers()) {
            // ここで結合するキーワードにに _ を含めないこと。
            // 識別子内に連続する _ があると、SPIRV-Cross が内部でひとつの _ に変換するため、不整合が起こることがある。
            std::string name = (LN_CIS_PREFIX LN_TO_PREFIX) + glsl.get_name(remap.image_id) + LN_SO_PREFIX + glsl.get_name(remap.sampler_id);
            glsl.set_name(remap.combined_id, name);
            combinedImageSamplerNames.push_back(std::move(name));
        }
    }

    // VertexShader から FragmentShader に渡す頂点データ (いわゆる昔の varying 変数) に同じ名前を付ける。
    {
        /*  TODO: 名前一致対応したい
			今のままだと、頂点シェーダの出力とピクセルシェーダの入力構造体のレイアウトが一致していなければ、
			glUseProgram が INVALID を返すみたいな原因のわかりにくい問題となる。
			glslang がセマンティクスまで理解していればいいが、そうでなければ構造体メンバの名前一致で指定できるようにしたい。
		*/

        if (m_stage == ShaderCodeStage::Vertex) {
            for (size_t i = 0; i < resources.stage_outputs.size(); i++) {
                std::stringstream s;
                s << "ln_varying_" << i;
                glsl.set_name(resources.stage_outputs[i].id, s.str());
                glsl.set_name(resources.stage_outputs[i].id, s.str());
            }
        } else if (m_stage == ShaderCodeStage::Fragment) {
            for (size_t i = 0; i < resources.stage_inputs.size(); i++) {
                std::stringstream s;
                s << "ln_varying_" << i;
                glsl.set_name(resources.stage_inputs[i].id, s.str());
            }
        }
    }

    // Generate GLSL code.
    std::string code = glsl.compile();

    // テクスチャサンプリング時にレンダリングターゲットであるかを判断し、上下反転するマクロコードを挿入する。
    {
        std::string declsIsRT;
        for (auto& name : combinedImageSamplerNames) {
            declsIsRT += "uniform int " + name + (LN_IS_RT_POSTFIX ";");
        }

        if (es) {
            if (m_stage == ShaderCodeStage::Vertex) {
                // VertexShader は精度指定子を記述する必要はないので、自動生成はされない。
                // https://qiita.com/konweb/items/ec8fa8cd3bc33df14933#%E7%B2%BE%E5%BA%A6%E4%BF%AE%E9%A3%BE%E5%AD%90
                code = code.insert(16, declsIsRT + "\n" + "vec4 LN_xxTexture(int isRT, sampler2D s, vec2 uv) { if (isRT != 0) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); } else { return texture(s, uv); } }\n"
                                                          "#define texture(s, uv) LN_xxTexture(s##lnIsRT, s, uv)\n"
                                                          "#line 1\n");
            } else {
                code = code.insert(16 + 45, declsIsRT + "\n" + "highp vec4 LN_xxTexture(int isRT, sampler2D s, vec2 uv) { if (isRT != 0) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); } else { return texture(s, uv); } }\n"
                                                               "#define texture(s, uv) LN_xxTexture(s##lnIsRT, s, uv)\n"
                                                               "#line 1\n");
            }
        } else {
            code = code.insert(13, declsIsRT + "\n" + "vec4 LN_xxTexture(int isRT, sampler2D s, vec2 uv) { if (isRT != 0) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); } else { return texture(s, uv); } }\n"
                                                      "vec4 LN_xxTexture(int isRT, sampler3D s, vec3 uv) { if (isRT != 0) { return texture(s, vec3(uv.x, (uv.y * -1.0) + 1.0, uv.z)); } else { return texture(s, uv); } }\n"
                                                      "#define texture(s, uv) LN_xxTexture(s##lnIsRT, s, uv)\n"
                                                      "#line 1\n");
        }

        /*
			DirectX に合わせたテクスチャ座標系(左上が原点)で OpenGL を使おうとすると、
			OpenGL のレンダリングターゲットをサンプリングするときに問題となる。

			普通のテクスチャは glTexImage などで画像を転送するときに上下反転するなど、自分で制御できる。
			しかし、レンダリングターゲットは OpenGL が書き込むため、上下の制御ができない。

			このためシェーダでは、レンダリングターゲットをサンプリングするときに限り上下反転する必要がある。

			bgfx の issue:
			https://github.com/bkaradzic/bgfx/issues/973

			TEXTURE COORDINATES – D3D VS. OPENGL:
			https://www.puredevsoftware.com/blog/2018/03/17/texture-coordinates-d3d-vs-opengl/
			通常テクスチャなら、glTexImage による反転と、サンプリング時の反転により正しく描画できる。

			Unity は内部的にうまいことやってくれているらしい:
			https://forum.unity.com/threads/fix-for-directx-flipping-vertical-screen-coordinates-in-image-effects-not-working.266455/
		*/
    }

    return std::vector<byte_t>(code.begin(), code.end());
}

//=============================================================================
// ShaderUniformBufferInfo

void ShaderUniformBufferInfo::mergeFrom(const ShaderUniformBufferInfo& other)
{
	LN_CHECK(name == other.name);
	LN_CHECK(size == other.size);

	for (auto& om : other.members)
	{
		auto itr = std::find_if(members.begin(), members.end(), [&](const ShaderUniformInfo& x) { return x.name == om.name; });
		if (itr != members.end()) {
			LN_CHECK(itr->name == om.name);
			LN_CHECK(itr->type == om.type);
			LN_CHECK(itr->offset == om.offset);
		}
		else {
			members.push_back(om);
		}
	}

	std::sort(members.begin(), members.end(), [](const ShaderUniformInfo& a, const ShaderUniformInfo& b) { return a.offset < b.offset; });
}

void ShaderUniformBufferInfo::mergeBuffers(
	const std::vector<ShaderUniformBufferInfo>& a,
	const std::vector<ShaderUniformBufferInfo>& b,
	std::vector<ShaderUniformBufferInfo>* out)
{
	*out = a;

	for (auto& buffer : b)
	{
		auto itr = std::find_if(out->begin(), out->end(), [&](const ShaderUniformBufferInfo& x) { return x.name == buffer.name; });
		if (itr != out->end()) {
			itr->mergeFrom(buffer);
		}
		else {
			out->push_back(buffer);
		}
	}
}

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
