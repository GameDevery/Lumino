
#pragma once
#include <Lumino/Base/Array.h>

namespace Lumino
{
namespace Scene
{
class SceneGraphManager;
class ViewPane;
class Layer;
class DrawingLayer;
class RenderingPass;
class RenderingParams;
class SceneNode;
class VisualNode;
class Camera;
class Light;
class MMEShader;
class MMEShaderTechnique;

struct VisualNodeSubsetParams;

typedef Array<RenderingPass*>	RenderingPassList;
typedef Array<Light*>			LightNodeList;
typedef Array<SceneNode*>		SceneNodeList;
typedef Array<MMEShader*>		SceneShaderList;

/// �m�[�h���
enum SceneNodeType
{
	SceneNodeType_BaseNode = 0,
	SceneNodeType_VisualNode,
};

/// �J�����̃v���W�F�N�V����
enum CameraProjection
{
	CameraProjection_2D = 0,	///< 2D ���[�h
	CameraProjection_3D,		///< 3D ���[�h

	CameraProjection_Max,		///< (Terminator)
};

/// ���C�g�̎��
enum LightType
{
	LightType_Directional = 0,	///< �f�B���N�V���i�����C�g
	LightType_Point,			///< �|�C���g���C�g
	LightType_Spot,				///< �X�|�b�g���C�g

	LightType_Max,				///< (Terminator)
};

/// �r���{�[�h�̌v�Z���@
enum BillboardType
{
	BillboardType_None = 0,		///< �r���{�[�h�̌v�Z���s��Ȃ�
	BillboardType_Front,		///< �J���� (�r���[�s��) �ɑ΂��Đ��ʂ�����
	BillboardType_RotY,			///< Y ����]�̂ݍs��

	BillboardType_Max,			///< (Terminator)
};

enum SpriteCoord
{
	SpriteCoord_X = 0,			///< X+ ���� (�E����)
	SpriteCoord_Y,				///< Y+ ���� (�����)
	SpriteCoord_Z,				///< Z+ ���� (������)
	SpriteCoord_RX,				///< X- ���� (������)
	SpriteCoord_RY,				///< Y- ���� (������)
	SpriteCoord_RZ,				///< Z- ���� (��O����)
	SpriteCoord_2D,				///< 2D ��ԗp�B���_������AY+ �������������Ƃ���B

	SpriteCoord_Max,			///< (Terminator)
};

/// ���]���@
enum FlipMode
{
	FlipMode_None = 0,		///< ���]����
	FlipMode_H,				///< ���E���]
	FlipMode_V,				///< �㉺���]
	FlipMode_HV,			///< ���E�㉺���]
};

/// �`��p�X
enum InternalRenderingPass
{
	InternalRenderingPass_object = 65536,	///< �I�u�W�F�N�g�{�́i�Z���t�V���h�EOFF�j
	InternalRenderingPass_zplot,			///< �Z���t�V���h�E�pZ�l�v���b�g
	InternalRenderingPass_object_ss,		///< �I�u�W�F�N�g�{�́i�Z���t�V���h�EON�j
	InternalRenderingPass_shadow,			///< �e�i�Z���t�V���h�E�ł͂Ȃ��P���ȉe�j
	InternalRenderingPass_edge,				///< �֊s�iPMD���f���̂݁j
	InternalRenderingPass_Infomation,		///< �f�o�b�O���
};

/// �J�����P�ʂ̐ݒ荀�� (1�t���[����1�x�����ݒ肷��΂����p�����[�^)
//struct CameraSceneParams
//{
//	//const Matrix*	Mat_CameraView;
//	//const Matrix*	Mat_CameraProj;
//	//const Matrix*	Mat_CameraViewProj;    ///< �J�����̃r���[�ƃv���W�F�N�V�����s��̐�
//
//	//Vector4			CameraPosition;	    ///< �J�����̈ʒu
//	//Vector4			CameraDirection;	    ///< �J�����̌���
//	//Vector4         ViewPixelSize;         ///< �`���RT�T�C�Y
//
//	//// �ȉ��� calcMatrices() �Őݒ肷��
//	//Matrix  Mat_CameraView_Inverse;
//	//Matrix  Mat_CameraProj_Inverse;
//	//Matrix  Mat_CameraViewProj_Inverse;
//	//Matrix  Mat_CameraView_Transpose;
//	//Matrix  Mat_CameraProj_Transpose;
//	//Matrix  Mat_CameraViewProj_Transpose;
//	//Matrix  Mat_CameraView_InverseTranspose;
//	//Matrix  Mat_CameraProj_InverseTranspose;
//	//Matrix  Mat_CameraViewProj_InverseTranspose;
//
//	//void CalcMatrices()
//	//{
//	//	Mat_CameraView_Inverse = Matrix::Inverse(*Mat_CameraView);
//	//	Mat_CameraProj_Inverse = Matrix::Inverse(*Mat_CameraProj);
//	//	Mat_CameraViewProj_Inverse = Matrix::Inverse(*Mat_CameraViewProj);
//	//	Mat_CameraView_Transpose = Matrix::Transpose(*Mat_CameraView);
//	//	Mat_CameraProj_Transpose = Matrix::Transpose(*Mat_CameraProj);
//	//	Mat_CameraViewProj_Transpose = Matrix::Transpose(*Mat_CameraViewProj);
//	//	Mat_CameraView_InverseTranspose = Mat_CameraView_Inverse * Mat_CameraView_Transpose;
//	//	Mat_CameraProj_InverseTranspose = Mat_CameraProj_Inverse * Mat_CameraProj_Transpose;
//	//	Mat_CameraViewProj_InverseTranspose = Mat_CameraViewProj_Inverse * Mat_CameraViewProj_Transpose;
//	//}
//};

} // namespace Scene
} // namespace Lumino

