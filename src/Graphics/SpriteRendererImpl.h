
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "Device/DeviceInterface.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

class SpriteRendererImpl
    : public RefObject
{
public:
	SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount, bool is3D);
	virtual ~SpriteRendererImpl();

public:

	/// ���W�ϊ��s��̐ݒ�
    void SetTransform( const Matrix& matrix );

	/// �r���[�A�v���W�F�N�V�����s��̐ݒ� (view �̓r���{�[�h�̌v�Z�ŕK�v�ɂȂ�̂ŁAview proj �ŕ�����)
	void SetViewProjMatrix(const Matrix& view, const Matrix& proj);

	void SetViewPixelSize(const Size& size);
	//void setViewProjMatrix( const LMatrix& view, const LMatrix& proj, const LVector2& viewPixelSize );
	
	/// �r���[�T�C�Y�̐ݒ�( 2D �p�̃v���W�F�N�V�����s�������āAsetViewProjMatrix() �ɐݒ肷��)
	//void setViewSize(const LVector2& viewSize);

	void SetRenderState( const RenderState& state );

	/// �\�[�g���@�̐ݒ�(SpriteSortMode  enable_view_pos_depth_:�J��������̋������\�[�g�pZ�l�ɂ���ꍇ true)
    void SetSortMode( uint32_t flags, bool enable_view_pos_depth_ );

	/// �`�惊�N�G�X�g
    void DrawRequest2D(
        const Vector3& position,
        const Vector3& center,
        Texture* texture,
        const RectF& srcRect,
        ColorF color );

	/// �`�惊�N�G�X�g
	void DrawRequest3D(
        const Vector3& position,
        const Vector3& center,
        const Vector2& size,
        Texture* texture,
        const RectF& src_rect,
		const ColorF* colorTable,    // 4 ���_���BNULL �̏ꍇ�͔�
		AxisDirection front);

	/// �o�b�`��������Ă���X�v���C�g�̕`��
	void Flash();

	/// �`�悹���Ƀo�b�`�����ׂăN���A����
	void Clear();

private:

	/// �o�b�`�����p�X�v���C�g���_
	struct BatchSpriteVertex
	{
		Vector3		Position;   ///< ���W
		ColorF		Color;      ///< ���_�J���[
		Vector2		TexUV;      ///< �e�N�X�`�����W

		/// ���_���C�A�E�g
		static Graphics::VertexElement* Elements()
		{
			static Graphics::VertexElement elements[] =
			{
				{ 0, Graphics::VertexElementType_Float3, Graphics::VertexElementUsage_Position, 0 },
				{ 0, Graphics::VertexElementType_Float4, Graphics::VertexElementUsage_Color, 0 },
				{ 0, Graphics::VertexElementType_Float2, Graphics::VertexElementUsage_TexCoord, 1 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	struct BatchSpriteData
	{
		BatchSpriteVertex	Vertices[4];  ///< ���W�ϊ��ςݒ��_ ([0].Position.z ���\�[�g�̊�ɂȂ�)
		Device::ITexture*	Texture;
		int					RenderStateIndex;
		int					Priority;       ///< �D��x (�傫�������ォ��`�悳��� =��O)
		float				Depth;          ///< �\�[�g�Ɏg���� Z �l (�傫���قǉ���)
		bool				Visible;
		bool				Assigned;       ///< SpriteRenderer::getBatchSpriteData() �Ŏ擾���ꂽ�ꍇ�� true �ɂȂ�

	};

    struct Attribute
    {
        int StartIndex;
        int PrimitiveNum;
		Device::ITexture*   Texture;
		int RenderStateIndex;
    };

	class SpriteCmpDepthBackToFront;		///< Z �l�̑傫�������珬�������փ\�[�g�����r
	class SpriteCmpDepthFrontToBack;		///< Z �l�̏�����������傫�����փ\�[�g�����r
	class SpriteCmpTexDepthBackToFront;		///< Z �l�̑傫�������珬�������փ\�[�g�����r (�e�N�X�`���D��)
	class SpriteCmpTexDepthFrontToBack;		///< Z �l�̏�����������傫�����փ\�[�g�����r (�e�N�X�`���D��)

private:
	typedef ArrayList<BatchSpriteData>	SpriteRequestList;
	typedef ArrayList<RenderState>		RenderStateList;
	typedef ArrayList<Attribute>		AttributeList;

	RefPtr<GraphicsManager>			m_manager;
	RefPtr<Device::IVertexBuffer>	m_vertexBuffer;
	RefPtr<Device::IIndexBuffer>	m_indexBuffer;
    int								m_maxSprites;
	SpriteRequestList				m_spriteRequestList;
	int								m_spriteRequestListUsedCount;
	ArrayList<int>					m_spriteIndexList;		///< Flash() ���Ŏg�p����Bm_spriteRequestList ���\�[�g����͍̂\���̃R�s�[�𔺂����ߑ��x�ʂŐS�z�B�Ȃ̂ŃC���f�b�N�X���\�[�g����B
	RenderStateList					m_renderStateList;
	int								m_currentRenderStateIndex;	///< ���� Draw �łɓK�p����� RenderState ������ m_renderStateList ���̃C���f�b�N�X

	AttributeList					m_attributeList;

	Matrix							m_transformMatrix;

	Vector3							m_viewDirection;		///< �r���{�[�h�̌v�Z�Ɏg�p����
	Matrix							m_viewInverseMatrix;	///< �r���{�[�h�̌v�Z�Ɏg�p����
	Vector3							m_viewPosition;			///< �J�����ʒu����Ƃ����\�[�g�Ŏg�p����
	Matrix							m_viewProjMatrix;
	Vector2							m_viewPixelSize;

	uint32_t						m_spriteSortMode;		///< �\�[�g���@ (LNSpriteSortMode)
	bool							m_enableViewPosDepth;	///< TODO:����� enum �̕����킩��₷���C������E�E�E

	bool							m_3DSystem;				///< true �̏ꍇ�A�X�v���C�g�̒��S�͎l�p�`�̒��S�ŁAY+ ��������ɂȂ�




    //BatchSpriteData*                    mSpriteRequests;
    //int                                 mNextIndex;         ///< mSpriteRequests �̋󂢂Ă���C���f�b�N�X������
    //int                                 mLastSpriteNum;
    //Matrix                             mTransformMatrix;
    //Matrix                             mViewInverseMatrix;
    //Matrix                             mViewProjMatrix;
    //Vector3                            mViewPosition;
    //Vector3                            mViewDirection;

    uint16_t*                                mSpriteIndexArray;
	uint16_t*                                mSpriteIndexArraySource;
	ArrayList<Attribute>		mAttributeList;

	bool(*mComFunc)(const uint16_t& l_, const uint16_t& r_);
    bool                                mIs3DSystem;        ///< true �̏ꍇ�A�X�v���C�g�̒��S�͎l�p�`�̒��S�ŁAY+ ��������ɂȂ�


	typedef ArrayList<RenderState>	RenderStateArray;
	RenderStateArray					mRenderStateArray;
	int									mCurrentRenderStateIndex;

    
	Vector2 mViewPixelSize;
   
	/// �V�F�[�_�֌W�̕ϐ����܂Ƃ߂��\����
    struct 
	{
		RefPtr<Device::IShader>		Shader;
		Device::IShaderVariable*    varViewProjMatrix;
		Device::IShaderVariable*    varViewPixelSize;
		Device::IShaderVariable*    varTexture;
		Device::IShaderTechnique*   techMainDraw;

    } m_shader;


    static BatchSpriteData* sSpriteRequests;
};

} // namespace Graphics
} // namespace Lumino
