
#ifndef	__EFFEKSEERRENDERER_GL_RENDERER_H__
#define	__EFFEKSEERRENDERER_GL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerRendererGL.Base.h"
#include "../../EffekseerRendererCommon/EffekseerRenderer.Renderer.h"

//----------------------------------------------------------------------------------
// Lib
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	�`��N���X
*/
class Renderer
	: public ::EffekseerRenderer::Renderer
{
protected:
	Renderer() {}
	virtual ~Renderer() {}

public:
	/**
		@brief	�C���X�^���X�𐶐�����B
		@param	squareMaxCount	[in]	�ő�`��X�v���C�g��
		@return	�C���X�^���X
	*/
	static Renderer* Create( int32_t squareMaxCount );

	/**
		@brief	�ő�`��X�v���C�g�����擾����B
	*/
	virtual int32_t GetSquareMaxCount() const = 0;

	/**
	@brief	�w�i���擾����B
	*/
	virtual GLuint GetBackground() = 0;

	/**
	@brief	�w�i��ݒ肷��B
	*/
	virtual void SetBackground(GLuint background) = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	���f��
*/
class Model
{
private:

public:
	GLuint		VertexBuffer;
	GLuint		IndexBuffer;
	int32_t		VertexCount;
	int32_t		IndexCount;
	int32_t		ModelCount;

	Model( ::Effekseer::Model::Vertex vertexData[], int32_t vertexCount, 
		::Effekseer::Model::Face faceData[], int32_t faceCount );
	~Model();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_RENDERER_H__