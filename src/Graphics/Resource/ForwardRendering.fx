
//==============================================================================
#ifdef LN_HLSL_DX9

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// �ʒu
	float2	Tex		: TEXCOORD0;	// �e�N�X�`�����W
	float3	Normal	: NORMAL0;//TEXCOORD1;	// �@��
	float4	Color	: COLOR0;		// ���_�F
};


// �I�u�W�F�N�g�̃e�N�X�`��
sampler		ObjTexSampler		= sampler_state
{
	texture = <ln_MaterialTexture>;
};

struct VS_OUTPUT
{
	float4	Pos		: POSITION;		// �ʒu
	float2	Tex		: TEXCOORD0;	// �e�N�X�`�����W
	float3	Normal	: TEXCOORD1;	// �@��
	float4	Color	: COLOR0;		// �F
};

//------------------------------------------------------------------------------
// ���_�V�F�[�_
VS_OUTPUT Basic_VS(LN_VS_INPUT input)
{
	VS_OUTPUT output;
	
	// ���_�ʒu���X�N���[����Ԃɕϊ�
	output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	
	// ���_�@�������[���h��Ԃɕϊ�
	output.Normal = normalize(mul(input.Normal, (float3x3)ln_World));
	
	// ���_�J���[
	output.Color = LN_GetLambertVertexColor(output.Normal) * input.Color;
	
	// �e�N�X�`�����W
	output.Tex = input.Tex;
	
	return output;
}

//------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_
float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	float4 color;
	
	float4 texColor = tex2D(ObjTexSampler, input.Tex);
	color = input.Color * texColor;//MaterialDiffuse;//input.Color * texColor;
	
	return color;
}

//------------------------------------------------------------------------------
// �`��e�N�j�b�N
technique MainTec0
{
	pass P0
	{
		VertexShader = compile vs_3_0 Basic_VS();
		PixelShader	 = compile ps_3_0 Basic_PS();
	}
}


#endif /* LN_HLSL_DX9 */

//=============================================================================
#ifdef LN_GLSL_VERTEX_Main
attribute vec3	ln_Vertex;			// Pos
attribute vec2	ln_MultiTexCoord0;	// UV
attribute vec4	ln_Color0;			// Color

varying vec2	v_TexUV;
varying vec4	v_Color;

void main()
{
	gl_Position		= vec4(ln_Vertex, 1.0) * ln_WorldViewProjection;
	v_TexUV			= LN_FlipTexCoord(ln_MultiTexCoord0);
	v_Color			= ln_Color0;
}
#endif /* LN_GLSL_VERTEX_Main */

//=============================================================================
#ifdef LN_GLSL_FRAGMENT_Main
varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
    gl_FragColor = texture2D(ln_MaterialTexture, v_TexUV) * v_Color;
}
#endif /* LN_GLSL_FRAGMENT_Main */


