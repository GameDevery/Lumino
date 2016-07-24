
static const int MMM_LightCount = 3;

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// �ʒu
	float2	Tex		: TEXCOORD0;	// �e�N�X�`�����W
	float3	Normal	: TEXCOORD1;	// �@��
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
	
	output.Color = input.Color;
	
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
	color = float4(LightDirection[0], 1);//input.Color * texColor;//MaterialDiffuse;//input.Color * texColor;
	
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

