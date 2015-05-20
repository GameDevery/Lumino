
#pragma once
#include <Lumino/Base/Array.h>

namespace Lumino
{
namespace Scene
{
class SceneGraphManager;
class Layer;
class DrawingLayer;
class RenderingPass;
class SceneNode;
class VisualNode;
class Camera;
class Light;

typedef ArrayList<Layer*>			LayerList;
typedef ArrayList<RenderingPass*>	RenderingPassList;
typedef ArrayList<Light*>			LightNodeList;
typedef ArrayList<SceneNode*>		SceneNodeList;

/// �m�[�h���
enum SceneNodeType
{
	SceneNodeType_BaseNode = 0,
	SceneNodeType_VisualNode,
};

/// �r���{�[�h�̌v�Z���@
enum BillboardType
{
	BillboardType_None = 0,		///< �r���{�[�h�̌v�Z���s��Ȃ�
	BillboardType_Front,		///< �J���� (�r���[�s��) �ɑ΂��Đ��ʂ�����
	BillboardType_RotY,			///< Y ����]�̂ݍs��

	BillboardType_Max,			///< (Terminator)
};

/// ���]���@
enum FlipMode
{
	FlipMode_None = 0,		///< ���]����
	FlipMode_H,				///< ���E���]
	FlipMode_V,				///< �㉺���]
	FlipMode_HV,			///< ���E�㉺���]
};

} // namespace Scene
} // namespace Lumino

