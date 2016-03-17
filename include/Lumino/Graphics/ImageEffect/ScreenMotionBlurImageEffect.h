
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
	//void StartBlur(double duration, float power, float scale, const Vector3& center);

	/** �摜���Ɏc���O�̃t���[���̗� (0.0�`1.0)�B�l�������Ȃ�قǁA���[�V�����g���C���������Ȃ�܂��B*/
	void SetAmount(float amount);

	/** �u���[�G�t�F�N�g����ˏ�ɓK�p���邽�߂̏���ݒ肵�܂��B*/
	void SetRadialCenter(const Vector2& center);

	void SetRadialScale(float scale);

protected:
	ScreenMotionBlurImageEffect();
	virtual ~ScreenMotionBlurImageEffect();
	void Initialize(GraphicsManager* manager);
	virtual void OnRender(RenderingContext2* renderingContext, RenderTarget* source, RenderTarget* destination) override;

	RenderTarget*	m_accumTexture;	// �O��̉�ʕ`����e
	float			m_amount;
	Vector2			m_center;
	float			m_scale;


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
