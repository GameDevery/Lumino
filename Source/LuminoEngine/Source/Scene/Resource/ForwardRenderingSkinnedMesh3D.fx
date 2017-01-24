


// ���W�ϊ��s��
float4x4	WorldViewProjMatrix		: WORLDVIEWPROJECTION;
float4x4	WorldMatrix				: WORLD;
float4x4	ViewMatrix				: VIEW;
float4x4	ProjMatrix				: PROJECTION;

//���C�g�֘A
bool		LightEnables[MMM_LightCount]		: LIGHTENABLES;		// �L���t���O
float4x4	LightWVPMatrices[MMM_LightCount]	: LIGHTWVPMATRICES;	// ���W�ϊ��s��
float3		LightDirection[MMM_LightCount]		: LIGHTDIRECTIONS;	// ����
float3		LightPositions[MMM_LightCount]		: LIGHTPOSITIONS;	// ���C�g�ʒu
float		LightZFars[MMM_LightCount]			: LIGHTZFARS;		// ���C�gzFar�l

//�ގ����[�t�֘A
float4	 AddingTexture = float4(0, 0, 0, 0)		  ;	// �ގ����[�t���ZTexture�l
float4	 AddingSphere = float4(0, 0, 0, 0)		  ;	// �ގ����[�t���ZSphereTexture�l
float4	 MultiplyTexture = float4(1, 1, 1, 1)	  ;	// �ގ����[�t��ZTexture�l
float4	 MultiplySphere	 = float4(1, 1, 1, 1)	  ;	// �ގ����[�t��ZSphereTexture�l

//�J�����ʒu
float3	 CameraPosition		: POSITION  < string Object = "Camera"; >;

// �}�e���A���F
float4   MaterialDiffuse	: DIFFUSE  < string Object = "Geometry"; >;
float3   MaterialAmbient	: AMBIENT  < string Object = "Geometry"; >;
float3   MaterialEmmisive	: EMISSIVE < string Object = "Geometry"; >;
float3   MaterialSpecular	: SPECULAR < string Object = "Geometry"; >;
float    SpecularPower		: SPECULARPOWER < string Object = "Geometry"; >;


// ���C�g�F
float3   LightDiffuses[MMM_LightCount]      : LIGHTDIFFUSECOLORS;
float3   LightAmbients[MMM_LightCount]      : LIGHTAMBIENTCOLORS;
float3   LightSpeculars[MMM_LightCount]     : LIGHTSPECULARCOLORS;

// ���C�g�F
static float4 DiffuseColor[3]  = { MaterialDiffuse * float4(LightDiffuses[0], 1.0f)
				 , MaterialDiffuse * float4(LightDiffuses[1], 1.0f)
				 , MaterialDiffuse * float4(LightDiffuses[2], 1.0f)};
static float3 AmbientColor[3]  = { saturate(MaterialAmbient * LightAmbients[0]) + MaterialEmmisive
				 , saturate(MaterialAmbient * LightAmbients[1]) + MaterialEmmisive
				 , saturate(MaterialAmbient * LightAmbients[2]) + MaterialEmmisive};
static float3 SpecularColor[3] = { MaterialSpecular * LightSpeculars[0]
				 , MaterialSpecular * LightSpeculars[1]
				 , MaterialSpecular * LightSpeculars[2]};



float           gOpacity            : OPACITY;
float4          gColorScale         : COLORSCALE;
float4          gBlendColor         : BLENDCOLOR;
float4          gTone               : TONE;
static float4   gColor              = float4( gColorScale.xyz, gColorScale.a * gOpacity );

// �I�u�W�F�N�g�̃e�N�X�`��
texture ObjectTexture: MATERIALTEXTURE;
sampler ObjTexSampler = sampler_state {
	texture = <ObjectTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

// �X�t�B�A�}�b�v�̃e�N�X�`��
texture ObjectSphereMap: MATERIALSPHEREMAP;
sampler ObjSphareSampler = sampler_state {
	texture = <ObjectSphereMap>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;		// �ˉe�ϊ����W
	float2 Tex		: TEXCOORD0;	// �e�N�X�`��
	float4 SubTex	: TEXCOORD1;	// �T�u�e�N�X�`��/�X�t�B�A�}�b�v�e�N�X�`�����W
	float3 Normal	: TEXCOORD2;	// �@��
	float3 Eye		: TEXCOORD3;	// �J�����Ƃ̑��Έʒu
#if 0
	float4 SS_UV1   : TEXCOORD4;	// �Z���t�V���h�E�e�N�X�`�����W
	float4 SS_UV2   : TEXCOORD5;	// �Z���t�V���h�E�e�N�X�`�����W
	float4 SS_UV3   : TEXCOORD6;	// �Z���t�V���h�E�e�N�X�`�����W
#endif
	float4 Color	: COLOR0;		// ���C�g0�ɂ��F
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(MMM_SKINNING_INPUT IN, uniform bool useTexture, uniform bool useSphereMap, uniform bool useToon, uniform bool useSelfShadow)
{
	VS_OUTPUT output;
	
	// ���W�Ɩ@�����擾����
	MMM_SKINNING_OUTPUT skinOut = MMM_SkinnedPositionNormal(IN.Pos, IN.Normal, IN.BlendWeight, IN.BlendIndices, IN.SdefC, IN.SdefR0, IN.SdefR1);
	
	// �J�����Ƃ̑��Έʒu
	output.Eye = CameraPosition - mul( skinOut.Position, WorldMatrix ).xyz;
	// ���_�@��
	output.Normal = normalize( mul( skinOut.Normal, (float3x3)WorldMatrix ) );
	
	// ���_���W
	//if (MMM_IsDinamicProjection)
	//{
	//	float4x4 wvpmat = mul(mul(WorldMatrix, ViewMatrix), MMM_DynamicFov(ProjMatrix, length(output.Eye)));
	//	output.Pos = mul( Skinoutput.Position, wvpmat );
	//}
	//else
	//{
		output.Pos = mul( skinOut.Position, WorldViewProjMatrix );
	//}
	
	// �f�B�t���[�Y�F�{�A���r�G���g�F �v�Z
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i].rgb * dot(output.Normal, -LightDirection[i])));
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = MaterialDiffuse.a;

	// �e�N�X�`�����W
	output.Tex = IN.Tex;
	output.SubTex = float4(IN.AddUV1.xy, 0, 0);
	
	if ( useSphereMap )
	{
		// �X�t�B�A�}�b�v�e�N�X�`�����W
		float2 NormalWV = mul( output.Normal, (float3x3)ViewMatrix );
		output.SubTex.z = NormalWV.x * 0.5f + 0.5f;
		output.SubTex.w = NormalWV.y * -0.5f + 0.5f;
	}
	
	//output.Color.r = IN.BlendWeight.x;
	//output.Color.g = IN.BlendWeight.y;
	//output.Color.b = IN.BlendWeight.z;
	//output.Color.a = 1;

#if 0
	if (useSelfShadow)
	{
		float4 dpos = mul(skinOut.Position, WorldMatrix);
		//�f�v�X�}�b�v�e�N�X�`�����W
		output.SS_UV1 = mul(dpos, LightWVPMatrices[0]);
		output.SS_UV2 = mul(dpos, LightWVPMatrices[1]);
		output.SS_UV3 = mul(dpos, LightWVPMatrices[2]);
		
		output.SS_UV1.y = -output.SS_UV1.y;
		output.SS_UV2.y = -output.SS_UV2.y;
		output.SS_UV3.y = -output.SS_UV3.y;

		output.SS_UV1.z = (length(LightPositions[0] - skinOut.Position) / LightZFars[0]);
		output.SS_UV2.z = (length(LightPositions[1] - skinOut.Position) / LightZFars[1]);
		output.SS_UV3.z = (length(LightPositions[2] - skinOut.Position) / LightZFars[2]);
	}
#endif

    return output;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(VS_OUTPUT IN, uniform bool useTexture, uniform bool useSphereMap, uniform bool useToon, uniform bool useSelfShadow) : COLOR0
{
	return tex2D(ObjTexSampler, IN.Tex);//IN.Color;//float4(1, 0, 0, 1);// 
#if 0
	float4 Color = IN.Color;
	float4 texColor = float4(1,1,1,1);
	float  texAlpha = MultiplyTexture.a + AddingTexture.a;

	//�X�y�L�����F�v�Z
	float3 HalfVector;
	float3 Specular = 0;
	for (int i = 0; i < 3; i++) {
		if (LightEnables[i]) {
			HalfVector = normalize( normalize(IN.Eye) + -LightDirection[i] );
			Specular += pow( max(0,dot( HalfVector, normalize(IN.Normal) )), SpecularPower ) * SpecularColor[i];
		}
	}

	// �e�N�X�`���K�p
	if (useTexture) {
		texColor = tex2D(ObjTexSampler, IN.Tex);
		texColor.rgb = (texColor.rgb * MultiplyTexture.rgb + AddingTexture.rgb) * texAlpha + (1.0 - texAlpha);
	}
	Color.rgb *= texColor.rgb;

	// �X�t�B�A�}�b�v�K�p
	//if ( useSphereMap ) {
	//	// �X�t�B�A�}�b�v�K�p
	//	if(spadd) Color.rgb = Color.rgb + (tex2D(ObjSphareSampler,IN.SubTex.zw).rgb * MultiplySphere.rgb + AddingSphere.rgb);
	//	else      Color.rgb = Color.rgb * (tex2D(ObjSphareSampler,IN.SubTex.zw).rgb * MultiplySphere.rgb + AddingSphere.rgb);
	//}
	// �A���t�@�K�p
	Color.a = IN.Color.a * texColor.a;

	// �Z���t�V���h�E�Ȃ��̃g�D�[���K�p
#if 0
	float3 color;
	if (!useSelfShadow && useToon && usetoontexturemap ) {
		//================================================================================
		// MikuMikuMoving�f�t�H���g�̃g�D�[���F���擾����(MMM_GetToonColor)
		//================================================================================
		color = MMM_GetToonColor(MaterialToon, IN.Normal, LightDirection[0], LightDirection[1], LightDirection[2]);
		Color.rgb *= color;
	}
	// �Z���t�V���h�E
	if (useSelfShadow) {

		if (useToon && usetoontexturemap) {
			//================================================================================
			// MikuMikuMoving�f�t�H���g�̃Z���t�V���h�E�F���擾����(MMM_GetSelfShadowToonColor)
			//================================================================================
			float3 shadow = MMM_GetToonColor(MaterialToon, IN.Normal, LightDirection[0], LightDirection[1], LightDirection[2]);
			color = MMM_GetSelfShadowToonColor(MaterialToon, IN.Normal, IN.SS_UV1, IN.SS_UV2, IN.SS_UV3, false, useToon);

			Color.rgb *= min(shadow, color);
		}
		else {
			Color.rgb *= MMM_GetSelfShadowToonColor(MaterialToon, IN.Normal, IN.SS_UV1, IN.SS_UV2, IN.SS_UV3, false, useToon);
		}
	}
#endif
	
	// �X�y�L�����K�p
	Color.rgb += Specular;

	return Color;
#endif
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic(true, false, false, false);
        PixelShader	 = compile ps_3_0 psBasic(true, false, false, false);
        CullMode = None;
    }
}

//=============================================================================
//
//=============================================================================
