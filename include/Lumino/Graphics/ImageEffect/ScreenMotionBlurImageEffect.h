
#pragma once
#include "ImageEffect.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief
*/
class ScreenMotionBlurImageEffect
	: public ImageEffect
{
public:
	static ScreenMotionBlurImageEffectPtr Create();

	/// �u���\�K�p (center �̓X�N���[�����W��ԂŎw�肷��(-1.0�`1.0) )
	void StartBlur(double duration, float power, float scale, const Vector3& center);

protected:
	ScreenMotionBlurImageEffect();
	virtual ~ScreenMotionBlurImageEffect();
	void Initialize(GraphicsManager* manager);
	virtual void OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination) override;

	RenderTarget*		m_accumTexture;	// �O��̉�ʕ`����e


	struct BufferingBlarShader
	{
		Shader*          shader;
		ShaderVariable*  varBlurPower;
		ShaderVariable*  varBlurColor;
		ShaderVariable*  varBlurMatrix;
		ShaderVariable*  varSecondaryTexture;
		ShaderTechnique* techMainDraw;

	} m_shader;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
