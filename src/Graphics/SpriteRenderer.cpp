/*
    �\�[�g����v�f�́A
        �E�v���C�I���e�B
        �EZ���W or �r���[�s��̈ʒu��������̋���
        �E�e�N�X�`��


    �s��v�Z�̃e�X�g�c

        ���_���Ƀ��[���h�s����o���Ă����� (Vector3 �� Right Up Front Position ��4x3)�A
        �V�F�[�_���Ōv�Z����̂ƁACPU ����4���_�ɑ΂��� transform() ����̂Ƃǂ����������̂��e�X�g�B

        ���@�c
            drawRequest() �� 6400 ����s�B
            ���̃��[�v�ƁA�V�[���S�̂̕`�揈���̎��Ԃ��v������B

            Intel(R) Core(TM)2 Duo CPU     E8200  @ 2.66GHz (2 CPUs), ~2.7GHz
            NVIDIA Quadro FX 1700

        ���ʁc

            �P�DCPU ���ō��W�ϊ�

                drawRequest : �� 6500ns
                Render      : �� 3500ns

            �Q�D�V�F�[�_���Ōv�Z

                drawRequest : �� 11000ns
                Render      : �� 5000ns

            �R�D�Q�̊��ŁA�s��̑����؂��Ă݂�

                drawRequest : �� 5000ns
                Render      : �� 5000ns

            ���܂�.
                �V�F�[�_���� mul �񐔂� 1 �񂾂��ɂ��Ă݂��B(���͍̂s��쐬��mul��ViewProj mul ��2��)

                drawRequest : �� 5000ns
                Render      : �� 5000ns

                ����܂�ς��Ȃ��B

        �l�@�c

            �s��̌v�Z�����A�l�R�s�[�~4 (4���_��) �̂ق����͂邩�ɏd���B
            �V�F�[�_�̕��͓n�����_�f�[�^�̃T�C�Y����ԉe���傫�������݂����B
                
*/

/*

�� �݌v�Ƃ�

�ESprite �N���X�� drawSubset() �́A�T�u�Z�b�g�ԍ� 0 �݂̂��n����Ă���B

�ESceneObject ���p�����邱�ƂŃ��[�U�[�C�ӂ̕`����ł���悤�ɂ���
	�ESpriteRenderer ���g������AdrawPrimitive() �g���킯����Ȃ��̂ł���܂Œʂ���Ă����킯�ɂ͂����Ȃ��B

�ESceneOjbect �ɂ� onRequestBatchDraw() ���������Ă����B
  ���̊֐��́ASpriteRenderer �� BillboardRenderer �̂悤�ɁA�f�[�^���o�b�t�@�ɗ��߂Ă���
  ��ň�x�ɕ`�悷����̂̕`�惊�N�G�X�g���M����������B

�EISprite �ɂ͂Q�̎�ނ�����B
  �ЂƂ́A�P��̔ėp���̂���X�v���C�g���Ă����C���[�W�B
  ����͈ȑO�܂łƓ��l�AonDrawSubset() �� setVertexBuffer()�AdrawPrimitive() �̏��Ɏ��s���ĕ`�悷��B
  �o�b�`�����̑Ώۂł͂Ȃ��B
  �����ЂƂ́A�o�b�`�����������́B�ЂƂ߂̂��������ɕ`��ł���
  (�`�撆�AsetVertexBuffer() �Œ��_�o�b�t�@�̐ݒ��ς��Ȃ���)���A
  �X�v���C�g�ȊO�̃I�u�W�F�N�g�Ƃ�Z�\�[�g�����܂������Ȃ��B
  �i2D�ł���X�v���C�g�Ȃ�撣��΂�����Ǝv�����ǁA3D��Ԃɔz�u�����r���{�[�h�͂��Ȃ�L�c�C�j
  ���̂��߁A�o�b�`�����͈ȉ��̂��̂�ΏۂƂ���B
    �E�������v�f�̖����I�u�W�F�N�g(�A���t�@�e�X�g�ŃJ�b�g�������̂�OK(�A���t�@�l�����S��0))
    �E�������v�f������A���A�������v�f�̕`��p�X�ŕ`�悷�����(�ʏ�̃I�u�W�F�N�g�̌�ɕ`�悷��ׂ�����)
  �ꉞ�A�o�b�`�����̑z�肵�Ă���g�����͎�ɃG�t�F�N�g�֌W�Ȃ̂ŁA���̕����Ŗ��͂Ȃ������B
  �쐬�Ɋւ��Ă� createSprite() createBatchSprite() �̊֐����g���A
  �Ԃ�C���^�[�t�F�C�X�͗����Ƃ� ISprite�B





Selene �ł̓X�v���C�g�ЂƂ��� drawPrimitive �ǂ�ł����ǁc�H

����
    ���_ 60000
    �g���C�A���O�����X�g
    ���_�̋߂��ŕ`��

    �P�D�l�p�`�ЂƂɂ� ��ADrawPrimitive() ���Ă�
        ��t���[���ڂ����� 1000ms
        �����t���[���ڂ����� 500ms

    �Q�D�S�̂�1�x�� DrawPrimitive() �ŕ`��
        �P�D�ƂقƂ�Ǔ����B

    DrawPrimitive() �̂Ƃ��낾����ׂĂ݂�ƁA
    �P�D�ł� 0�`2ms�A�Q�D�ł� 0ms�B


    ���x�I�ɂ͂قƂ�Ǖς��Ȃ������B���Ȃ݂ɑ��x�������Ă�̂� Present()�B
    �덷�͊֐��Ăяo���̕��̎��Ԃ��Ċ����ŁA�`��ɂ͂قƂ�Ǌ֌W�Ȃ������B




�� �����̎l�p�`���i�[�������_�o�b�t�@�ɑ΂��āA
   �l�p�`�ЂƂɂ���x drawPrimitive() ���Ă�ŕ`�悷��


���؂���R�[�h

	lnU32 n = (VERTEX_NUM / 3) / 2;
	for ( lnU32 i = 0; i < n; ++i )
	{
	    renderer_->setVertexBuffer( mVertexBuffer, true );
	    renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
	}


���_�� 60000
�g���C�A���O�����X�g
��x�� drawPrimitive() ��2�̃v���~�e�B�u (�l�p�`)��`�悷��


�� ���I�Ȓ��_�o�b�t�@�Ƃ��č쐬�����ꍇ
	80ms�`90ms
	
�� �ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬�����ꍇ
	40ms�`50ms
	�{���炢�����B

�� lock() unlock() �̏d��
	�ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬��A
	��L�R�[�h�̒��O��
		mVertexBuffer->lock();
		mVertexBuffer->unlock();
	�̂Q�s���ĂԁB
	���ʁA80ms�`90ms�B
	���I�Ȓ��_�o�b�t�@�Ƃقړ����B
	
�� lock() unlock() �̏d��  100��
	�ÓI�Ȓ��_�o�b�t�@�Ƃ��č쐬��A
	��L�R�[�h�̒��O��
	for ( lnU32 i = 0; i < 100; ++i )
    {
        mVertexBuffer->lock();
        mVertexBuffer->unlock();
    }
    �����s����B
    ���ʁA80ms�`90ms�B
    �قƂ�Ǖς��Ȃ������B
    
�� renderer_->setVertexBuffer( mVertexBuffer, true ); ��
	���[�v�̊O�ɏo���āA�P�x�������s����悤�ɂ���
	
	�E���I�̏ꍇ
		80ms�`90ms�B
		���܂�ς��Ȃ��B
		
	�E�ÓI�̏ꍇ
		40ms�`50ms
		���܂�ς��Ȃ��B
		
	�E�ÓI�łP�O�O��lock()
		80ms�`90ms�B
		���܂�ς��Ȃ��B
		
		
�� �����ЂƂ��_�o�b�t�@������āA�ȉ��̃R�[�h�����s

	lnU32 n = (VERTEX_NUM / 3) / 2;
	n /= 2;
    for ( lnU32 i = 0; i < n; ++i )
    {
        renderer_->setVertexBuffer( mVertexBuffer, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );

        renderer_->setVertexBuffer( mVertexBuffer2, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
    }
    
    �E���I�̏ꍇ
		200ms�`220ms�B
		
		
    �E�ÓI�̏ꍇ
    	90�`110ms
    	
    �E�ÓI�E100 lock() �̏ꍇ
    	90�`110ms
    	
	�E�ÓI�E100 lock() �����̏ꍇ
    	90�`110ms



*/

#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include "SpriteRendererImpl.h"

#define LN_CALL_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType_Deferred) { \
		m_manager->GetPrimaryRenderingCommandList()->AddCommand<command>(m_impl, __VA_ARGS__); \
	} \
	else { \
		m_impl->func(__VA_ARGS__); \
	}

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// SpriteRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer* SpriteRenderer::Create(int maxSpriteCount, GraphicsManager* manager)
{
	return LN_NEW SpriteRenderer(manager, maxSpriteCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer::SpriteRenderer(GraphicsManager* manager, int maxSpriteCount)
{
	m_manager = manager;
	m_impl = LN_NEW SpriteRendererImpl(manager, maxSpriteCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRenderer::~SpriteRenderer()
{
	LN_SAFE_RELEASE(m_impl);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::SetTransform(const Matrix& matrix)
{
	LN_CALL_COMMAND(SetTransform, SpriteRendererImpl::SetTransformCommand, matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	LN_CALL_COMMAND(SetViewProjMatrix, SpriteRendererImpl::SetViewProjMatrixCommand, view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::SetViewPixelSize(const Size& size)
{
	LN_CALL_COMMAND(SetViewPixelSize, SpriteRendererImpl::SetViewPixelSizeCommand, size);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::SetRenderState(const RenderState& state)
{
	LN_CALL_COMMAND(SetRenderState, SpriteRendererImpl::SetRenderStateCommand, state);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::SetSortMode(uint32_t flags, SortingDistanceBasis basis)
{
	LN_CALL_COMMAND(SetSortMode, SpriteRendererImpl::SetSortModeCommand, flags, basis);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::DrawRequest2D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const ColorF* colorTable)
{
	const ColorF defaultColor[] = { ColorF::White, ColorF::White, ColorF::White, ColorF::White };
	if (colorTable == NULL) {
		colorTable = defaultColor;
	}
	LN_CALL_COMMAND(
		DrawRequest2D, SpriteRendererImpl::DrawRequest2DCommand,
		position, center, size, texture->GetDeviceObject(), srcRect, colorTable);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::DrawRequest3D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const ColorF* colorTable,
	AxisDirection front)
{
	const ColorF defaultColor[] = { ColorF::White, ColorF::White, ColorF::White, ColorF::White };
	if (colorTable == NULL) {
		colorTable = defaultColor;
	}
	LN_CALL_COMMAND(
		DrawRequest3D, SpriteRendererImpl::DrawRequest3DCommand,
		position, center, size, texture->GetDeviceObject(), srcRect, colorTable, front);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRenderer::Flash()
{
	LN_CALL_COMMAND(Flash, SpriteRendererImpl::FlashCommand);
}


//=============================================================================
// SpriteRendererImpl
//=============================================================================
	
static const byte_t g_SpriteRenderer_fx_Data[] =
{
#include "Resource/SpriteRenderer.fx.h"
};
static const size_t g_SpriteRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_SpriteRenderer_fx_Data);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRendererImpl::SpriteRendererImpl(GraphicsManager* manager, int maxSpriteCount)
	: m_manager(manager, true)
	, m_vertexBuffer()
	, m_indexBuffer()
	, m_maxSprites(maxSpriteCount)
	, m_spriteRequestList()
	, m_spriteRequestListUsedCount(0)
	, m_spriteIndexList()
	, m_renderStateList()
	, m_currentRenderStateIndex(0)
	, m_attributeList()
	, m_transformMatrix()
	, m_viewDirection()
	, m_viewInverseMatrix()
	, m_viewPosition()
	, m_viewProjMatrix()
	, m_viewPixelSize()
	, m_spriteSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront)
	, m_sortingBasis(SortingDistanceBasis_RawZ)
{
	Device::IGraphicsDevice* device = m_manager->GetGraphicsDevice()->GetDeviceObject();

	//-----------------------------------------------------
	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@

	m_vertexBuffer.Attach(device->CreateVertexBuffer(
		BatchSpriteVertex::Elements(), BatchSpriteVertex::ElementCount, m_maxSprites * 4, NULL, DeviceResourceUsage_Dynamic));

	m_indexBuffer.Attach(device->CreateIndexBuffer(
		m_maxSprites * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	// �C���f�b�N�X�o�b�t�@�̒l�͍ŏ�����Œ��OK�Ȃ̂ł����ŏ�������ł��܂�
	uint16_t* ib;
	size_t lockedSize;	// dummy
	m_indexBuffer->Lock((void**)&ib, &lockedSize);
	int idx = 0;
	int i2 = 0;
	for (int i = 0; i < m_maxSprites; ++i)
	{
		i2 = i * 6;
		idx = i * 4;
		ib[i2 + 0] = idx;
		ib[i2 + 1] = idx + 1;
		ib[i2 + 2] = idx + 2;
		ib[i2 + 3] = idx + 2;
		ib[i2 + 4] = idx + 1;
		ib[i2 + 5] = idx + 3;
	}
	m_indexBuffer->Unlock();

	//-----------------------------------------------------
	// �V�F�[�_

	ShaderCompileResult r;
	m_shader.Shader.Attach(device->CreateShader(g_SpriteRenderer_fx_Data, g_SpriteRenderer_fx_Len, &r));
	m_shader.varTexture			= m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.varViewProjMatrix	= m_shader.Shader->GetVariableByName(_T("gViewProjMatrix"));
	m_shader.varViewPixelSize	= m_shader.Shader->GetVariableByName(_T("gViewportSize"));
	m_shader.varTexture			= m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.techMainDraw		= m_shader.Shader->GetTechnique(0);

	//-----------------------------------------------------
	// �������m�ۂƊe�평���l

	m_spriteRequestList.Resize(m_maxSprites);
	m_spriteIndexList.Resize(m_maxSprites);

	// �X�e�[�g�ɂ̓f�t�H���g���ЂƂl�߂Ă���
	m_renderStateList.Add(RenderState());

	//if (m_3DSystem) {
	//	// ���_����̋������傫�����̂��ɕ`�悷��
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_ViewPont);
	//}
	//else {
	//	// �X�v���C�g�� Z �l�����������̂��ɕ`�悷��
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_RawZ);
	//}

	m_spriteRequestListUsedCount = 0;
	m_currentRenderStateIndex = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteRendererImpl::~SpriteRendererImpl()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::SetTransform(const Matrix& matrix)
{
	m_transformMatrix = matrix;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	m_viewDirection.Set(view.M[0][2], view.M[1][2], view.M[2][2]);
	m_viewInverseMatrix = Matrix::Inverse(view);
	m_viewPosition = m_viewInverseMatrix.GetPosition();
	m_viewProjMatrix = (view * proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::SetViewPixelSize(const Size& size)
{
	m_viewPixelSize.Set((float)size.Width, (float)size.Height);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::SetRenderState(const RenderState& state)
{
	// �������̂�����΃J�����g��
	size_t count = m_renderStateList.GetCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (state == m_renderStateList[i]) {
			m_currentRenderStateIndex = i;
			return;
		}
	}

	// ������Ȃ�������o�^
	m_renderStateList.Add(state);
	m_currentRenderStateIndex = count;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::SetSortMode(uint32_t flags, SortingDistanceBasis basis)
{
	m_spriteSortMode = flags;
	m_sortingBasis = basis;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest2D(
    const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Device::ITexture* texture,
	const RectF& srcRect,
	const ColorF* colorTable)
{
	DrawRequest3DInternal(
		position,
		center,
		size,
		texture,
		srcRect,
		colorTable,
		AxisDirection_RZ,
		false);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest3D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Device::ITexture* texture,
	const RectF& srcRect,
	const ColorF* colorTable,
	AxisDirection front)
{
	DrawRequest3DInternal(position, center, size, texture, srcRect, colorTable, front, true);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest3DInternal(
    const Vector3& position,
    const Vector3& center,
    const Vector2& size,
	Device::ITexture* texture,
    const RectF& srcRect,
	const ColorF* colorTable,
	AxisDirection front,
	bool is3D)
{
	LN_THROW(m_spriteRequestListUsedCount < m_maxSprites, InvalidOperationException);

	BatchSpriteData& sprite = m_spriteRequestList[m_spriteRequestListUsedCount];

	// 3D �̏ꍇ�̒��_���W
	if (is3D)
	{
		Vector3 origin(-center);
		Vector2 harf_size(size * 0.5f);
		float l, t, r, b;
		r = harf_size.X;
		b = -harf_size.Y;
		l = -r;
		t = -b;

#define LN_WRITE_V3( x_, y_, z_ ) origin.X + x_, origin.Y + y_, origin.Z + z_

		switch (front)
		{
		case AxisDirection_X:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, l));     // ����
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, t, r));     // �E��
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, b, l));     // ����
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, r));     // �E��
			break;
		case AxisDirection_Y:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, 0, t));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, 0, t));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, 0, b));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, 0, b));
			break;
		case AxisDirection_Z:
			/*
			sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
			sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
			sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
			sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
			*/
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, t, 0));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, t, 0));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, b, 0));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, b, 0));
			break;
		case AxisDirection_RX:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, r));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, t, l));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, b, r));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, l));
			break;
		case AxisDirection_RY:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, 0, t));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, 0, t));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, 0, b));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, 0, b));
			break;
		case AxisDirection_RZ:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, t, 0));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, t, 0));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, b, 0));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, b, 0));
			/* �E��p
			sprite.Vertices[ 0 ].Position.set( LN_WRITE_V3( l, t, 0 ) );
			sprite.Vertices[ 1 ].Position.set( LN_WRITE_V3( l, b, 0 ) );
			sprite.Vertices[ 2 ].Position.set( LN_WRITE_V3( r, t, 0 ) );
			sprite.Vertices[ 3 ].Position.set( LN_WRITE_V3( r, b, 0 ) );
			*/
			break;
		}
#undef LN_WRITE_V3
	}
	// 2D �̏ꍇ�̒��_���W
	else
	{
		Vector3 origin(-center);
		sprite.Vertices[0].Position.Set(origin.X, origin.Y, origin.Z);
		sprite.Vertices[1].Position.Set(origin.X + size.X, origin.Y, origin.Z);
		sprite.Vertices[2].Position.Set(origin.X, origin.Y + size.Y, origin.Z);
		sprite.Vertices[3].Position.Set(origin.X + size.X, origin.Y + size.Y, origin.Z);
	}

	Matrix mat = m_transformMatrix.GetRotationMatrix();


	// �r���{�[�h (Scene ����g���ꍇ�� SceneNode ���ʓ|���Ă�̂ŁAScene �ȊO�ŕK�v�ɂȂ�܂ŕۗ��c)
	if (0)
	{
		// TODO:
		//mat.setMul3x3( mViewInverseMatrix );
	}
	// �r���{�[�h�EY ���݂̂ɓK�p
	else if (0)
	{
		if (m_viewDirection.X > 0.0f)
		{
			mat.RotationY(-atanf(m_viewDirection.Z / m_viewDirection.X) + Math::PI / 2);
		}
		else if (m_viewDirection.X == 0.0f)
		{
			//D3DXMatrixIdentity(&matWorld); // 0���Z��h������
		}
		else
		{
			mat.RotationY(-atanf(m_viewDirection.Z / m_viewDirection.X) - Math::PI / 2);
		}

	}
	// �r���{�[�h�ł͂Ȃ�
	else
	{
	}

	mat.Translation(position);
	mat.Translation(m_transformMatrix.GetPosition());

	// ���W�ϊ�
	sprite.Vertices[0].Position.TransformCoord(mat);
	sprite.Vertices[1].Position.TransformCoord(mat);
	sprite.Vertices[2].Position.TransformCoord(mat);
	sprite.Vertices[3].Position.TransformCoord(mat);

	// �F
	if (colorTable != NULL)
	{
		sprite.Vertices[0].Color = colorTable[0];
		sprite.Vertices[1].Color = colorTable[1];
		sprite.Vertices[2].Color = colorTable[2];
		sprite.Vertices[3].Color = colorTable[3];
	}
	else
	{
		sprite.Vertices[0].Color = ColorF::White;
		sprite.Vertices[1].Color = ColorF::White;
		sprite.Vertices[2].Color = ColorF::White;
		sprite.Vertices[3].Color = ColorF::White;
	}

	// �e�N�X�`��
	if (texture != NULL)
	{
		// �e�N�X�`�����W
		const Size& texSize = texture->GetRealSize();
		Vector2 texSizeInv(1.0f / texSize.Width, 1.0f / texSize.Height);
		RectF sr(srcRect);
		float l = sr.X * texSizeInv.X;
		float t = sr.Y * texSizeInv.Y;
		float r = (sr.X + sr.Width) * texSizeInv.X;
		float b = (sr.Y + sr.Height) * texSizeInv.Y;
		sprite.Vertices[0].TexUV.X = l;
		sprite.Vertices[0].TexUV.Y = t;
		sprite.Vertices[1].TexUV.X = r;
		sprite.Vertices[1].TexUV.Y = t;
		sprite.Vertices[2].TexUV.X = l;
		sprite.Vertices[2].TexUV.Y = b;
		sprite.Vertices[3].TexUV.X = r;
		sprite.Vertices[3].TexUV.Y = b;

		// �e�N�X�`��
		sprite.Texture = texture;	// TOOD: AddRef ����ׂ������H
	}
	else
	{
		sprite.Vertices[0].TexUV.X = 0;
		sprite.Vertices[0].TexUV.Y = 0;
		sprite.Vertices[1].TexUV.X = 1;
		sprite.Vertices[1].TexUV.Y = 0;
		sprite.Vertices[2].TexUV.X = 0;
		sprite.Vertices[2].TexUV.Y = 1;
		sprite.Vertices[3].TexUV.X = 1;
		sprite.Vertices[3].TexUV.Y = 1;
		sprite.Texture = m_manager->GetDummyTexture();
	}

	// �J��������̋������\�[�g�pZ�l�ɂ���ꍇ
	if (m_sortingBasis == SortingDistanceBasis_ViewPont) {
		sprite.Depth = (m_viewPosition - position).GetLengthSquared();
	}
	else {
		sprite.Depth = position.Z;
	}

	sprite.Visible = true;
	sprite.Assigned = false;
	sprite.Priority = 0;
	sprite.RenderStateIndex = m_currentRenderStateIndex;

	++m_spriteRequestListUsedCount;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
// Z �l�̑傫�������珬�������փ\�[�g�����r
class SpriteRendererImpl::SpriteCmpDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Depth > rsp.Depth)
				{
					return true;
				}
				if (lsp.Depth == rsp.Depth)
				{
					return lsp.Texture < rsp.Texture;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z �l�̏�����������傫�����փ\�[�g�����r
class SpriteRendererImpl::SpriteCmpDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Depth < rsp.Depth)
				{
					return true;
				}
				if (lsp.Depth == rsp.Depth)
				{
					return lsp.Texture < rsp.Texture;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z �l�̑傫�������珬�������փ\�[�g�����r (�e�N�X�`���D��)
class SpriteRendererImpl::SpriteCmpTexDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					// Depth �~��
					return lsp.Depth > rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z �l�̏�����������傫�����փ\�[�g�����r (�e�N�X�`���D��)
class SpriteRendererImpl::SpriteCmpTexDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					return lsp.Depth < rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::Flash()
{
	int spriteCount = m_spriteRequestListUsedCount;	// �`�悷��X�v���C�g�̐�
	if (spriteCount == 0) { return; }				// 0 �Ȃ�Ȃɂ����Ȃ�

	// �`�悷��X�v���C�g�̃C���f�b�N�X���X�g������������
	for (int i = 0; i < spriteCount; ++i) {
		m_spriteIndexList[i] = i;
	}
	//memcpy(m_spriteIndexList, mSpriteIndexArraySource, sizeof(*m_spriteIndexList) * mLastSpriteNum);

	// �C���f�b�N�X����ёւ���
	if (m_spriteSortMode & SpriteSortMode_Texture)
	{
		if (m_spriteSortMode & SpriteSortMode_DepthBackToFront)
		{
			SpriteCmpDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (m_spriteSortMode & SpriteSortMode_DepthFrontToBack)
		{
			SpriteCmpDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}
	else
	{
		if (m_spriteSortMode & SpriteSortMode_DepthBackToFront)
		{
			SpriteCmpTexDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (m_spriteSortMode & SpriteSortMode_DepthFrontToBack)
		{
			SpriteCmpTexDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}

	int si = 0;     // Sprite Index (m_spriteIndexList)
	int vi = 0;     // Vertex Index
	int ri = 0;     // Request Index (m_spriteRequestList)
	int start_idx = 0;
	int prim_num = 0;
	Device::ITexture* current_tex = m_spriteRequestList[m_spriteIndexList[0]].Texture;
	int currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[0]].RenderStateIndex;

	//-----------------------------------------------------
	// �������X�g�����

	m_attributeList.Clear();
	while (true)
	{
		while (true)
		{
			++si;
			++prim_num;
#if 1
			// ���̃X�v���C�g�̃e�N�X�`�����A�������̃e�N�X�`���ƈقȂ�ꍇ�͎��̑����쐬�Ɉڂ�
			if (si >= spriteCount ||
				m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex ||
				m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex != currnetRenderStateIndex)
			{
				break;
			}
#else
			// ���̃X�v���C�g�̃e�N�X�`�����A�������̃e�N�X�`���ƈقȂ�ꍇ�͎��̑����쐬�Ɉڂ�
			if (si >= spriteCount || m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex)
			{
				break;
			}
#endif
		}

		Attribute attr;
		attr.StartIndex = start_idx;
		attr.PrimitiveNum = prim_num * 2;
		attr.Texture = current_tex;
		attr.RenderStateIndex = currnetRenderStateIndex;
		m_attributeList.Add(attr);

		if (si >= spriteCount)
		{
			break;
		}

		start_idx = si * 6;
		prim_num = 0;
		current_tex = m_spriteRequestList[m_spriteIndexList[si]].Texture;
		currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex;
	}

	//-----------------------------------------------------
	// ���_�f�[�^���R�s�[

	BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >(m_vertexBuffer->Lock());
	si = 0;
	vi = 0;
	for (; si < spriteCount; ++si)
	{
		ri = m_spriteIndexList[si];
		memcpy(&vb[vi], m_spriteRequestList[ri].Vertices, sizeof(m_spriteRequestList[0].Vertices));
		vi += 4;
	}
	m_vertexBuffer->Unlock();

	//-----------------------------------------------------
	// �`��

	Device::IRenderer* r = m_manager->GetGraphicsDevice()->GetDeviceObject()->GetRenderer();
	r->SetVertexBuffer(m_vertexBuffer);
	r->SetIndexBuffer(m_indexBuffer);
	m_shader.varViewProjMatrix->SetMatrix(m_viewProjMatrix);
	m_shader.varViewPixelSize->SetVector(Vector4(m_viewPixelSize.X, m_viewPixelSize.Y, 0, 0));

	Device::IShaderPass* pass = m_shader.techMainDraw->GetPass(0);

	AttributeList::iterator itr = m_attributeList.begin();
	AttributeList::iterator end = m_attributeList.end();
	for (; itr != end; ++itr)
	{
		r->SetRenderState(m_renderStateList[itr->RenderStateIndex]);
		m_shader.varTexture->SetTexture(itr->Texture);
		pass->Apply();
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, itr->StartIndex, itr->PrimitiveNum);
	}

	//-----------------------------------------------------
	// �N���[���A�b�v

	Clear();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteRendererImpl::Clear()
{
	m_renderStateList.Clear();
	m_renderStateList.Add(RenderState());
	m_currentRenderStateIndex = 0;
	m_spriteRequestListUsedCount = 0;
}

} // namespace Graphics
} // namespace Lumino
