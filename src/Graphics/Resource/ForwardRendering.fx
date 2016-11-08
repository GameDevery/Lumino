
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
float4x4	ln_WorldViewProjection;
float4x4	ln_World;
float4x4	ln_View;
float4x4	ln_Projection;

//���C�g�֘A
bool		ln_LightEnables[MMM_LightCount];		// �L���t���O
float3		ln_LightDirections[MMM_LightCount];		// ����
float3		ln_LightPositions[MMM_LightCount];		// ���C�g�ʒu
float		ln_LightZFars[MMM_LightCount];			// ���C�gzFar�l

// �}�e���A���F
float4		ln_MaterialDiffuse;
float3		ln_MaterialAmbient;
float3		ln_MaterialEmmisive;
float3		ln_MaterialSpecular;
float		ln_MaterialSpecularPower;

// ���C�g�F
float3		ln_LightDiffuses[MMM_LightCount];
float3		ln_LightAmbients[MMM_LightCount];
float3		ln_LightSpeculars[MMM_LightCount];

// ���C�g�F
static float4 DiffuseColor[3] =
{
	ln_MaterialDiffuse * float4(ln_LightDiffuses[0], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[1], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[2], 1.0f),
};
static float3 AmbientColor[3] =
{
	saturate(ln_MaterialAmbient * ln_LightAmbients[0]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[1]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[2]) + ln_MaterialEmmisive,
};
static float3 SpecularColor[3] =
{
	ln_MaterialSpecular * ln_LightSpeculars[0],
	ln_MaterialSpecular * ln_LightSpeculars[1],
	ln_MaterialSpecular * ln_LightSpeculars[2],
};

// �I�u�W�F�N�g�̃e�N�X�`��
texture		ln_MaterialTexture;
sampler		ObjTexSampler		= sampler_state
{
	texture = <ln_MaterialTexture>;
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
	output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	
	// ���_�@�������[���h��Ԃɕϊ�
	output.Normal = normalize(mul(input.Normal, (float3x3)ln_World));
	
	// �f�B�t���[�Y�F�{�A���r�G���g�F �v�Z
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (ln_LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i] * dot(output.Normal, -ln_LightDirections[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);zzzzzz
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = ln_MaterialDiffuse.a;
	
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

