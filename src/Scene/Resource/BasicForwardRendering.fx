
static const int MMM_LightCount = 3;

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// �ʒu
	float2	Tex		: TEXCOORD0;	// �e�N�X�`�����W
	float3	Normal	: NORMAL0;//TEXCOORD1;	// �@��
	float4	Color	: COLOR0;		// ���_�F
};

//------------------------------------------------------------------------------

// ���W�ϊ��s��
float4x4	WorldViewProjMatrix	: WORLDVIEWPROJECTION;
float4x4	WorldMatrix			: WORLD;
float4x4	ViewMatrix			: VIEW;
float4x4	ProjMatrix			: PROJECTION;

//���C�g�֘A
bool		LightEnables[MMM_LightCount]		: LIGHTENABLES;		// �L���t���O
float4x4	LightWVPMatrices[MMM_LightCount]	: LIGHTWVPMATRICES;	// ���W�ϊ��s��
float3		LightDirection[MMM_LightCount]		: LIGHTDIRECTIONS;	// ����
float3		LightPositions[MMM_LightCount]		: LIGHTPOSITIONS;	// ���C�g�ʒu
float		LightZFars[MMM_LightCount]			: LIGHTZFARS;		// ���C�gzFar�l

// �}�e���A���F
float4		MaterialDiffuse		: DIFFUSE  <string Object = "Geometry";>;
float3		MaterialAmbient		: AMBIENT  <string Object = "Geometry";>;
float3		MaterialEmmisive	: EMISSIVE <string Object = "Geometry";>;
float3		MaterialSpecular	: SPECULAR <string Object = "Geometry";>;
float		SpecularPower		: SPECULARPOWER < string Object = "Geometry"; >;


// ���C�g�F
float3   LightDiffuses[MMM_LightCount]      : LIGHTDIFFUSECOLORS;
float3   LightAmbients[MMM_LightCount]      : LIGHTAMBIENTCOLORS;
float3   LightSpeculars[MMM_LightCount]     : LIGHTSPECULARCOLORS;

// ���C�g�F
static float4 DiffuseColor[3] =
{
	MaterialDiffuse * float4(LightDiffuses[0], 1.0f),
	MaterialDiffuse * float4(LightDiffuses[1], 1.0f),
	MaterialDiffuse * float4(LightDiffuses[2], 1.0f),
};
static float3 AmbientColor[3] =
{
	saturate(MaterialAmbient * LightAmbients[0]) + MaterialEmmisive,
	saturate(MaterialAmbient * LightAmbients[1]) + MaterialEmmisive,
	saturate(MaterialAmbient * LightAmbients[2]) + MaterialEmmisive,
};
static float3 SpecularColor[3] =
{
	MaterialSpecular * LightSpeculars[0],
	MaterialSpecular * LightSpeculars[1],
	MaterialSpecular * LightSpeculars[2],
};

// �I�u�W�F�N�g�̃e�N�X�`��
texture		ObjectTexture		: MATERIALTEXTURE;
sampler		ObjTexSampler		= sampler_state
{
	texture = <ObjectTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
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
	output.Pos = mul(float4(input.Pos, 1.0), WorldViewProjMatrix);
	
	// ���_�@�������[���h��Ԃɕϊ�
	output.Normal = normalize(mul(input.Normal, (float3x3)WorldMatrix));
	
	// �f�B�t���[�Y�F�{�A���r�G���g�F �v�Z
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i] * dot(output.Normal, -LightDirection[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = MaterialDiffuse.a;
	
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
	color = input.Color;// * texColor;//MaterialDiffuse;//input.Color * texColor;
	
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

