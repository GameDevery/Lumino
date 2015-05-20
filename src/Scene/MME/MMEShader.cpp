
#include "../../Internal.h"
#include "../SceneGraphManager.h"
#include "../SceneNode.h"
#include "../Camera.h"
#include "../Light.h"
#include "../VisualNodeParams.h"
#include "MMEShaderTechnique.h"
#include "MMEShader.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// MMEShader
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader::MMEShader(SceneGraphManager* manager)
	: m_manager(NULL)
	, m_coreShader(NULL)
	, m_mmeScriptOutput(MME_SCROUT_color)
	, m_mmeScriptClass(MME_SCRCLS_object)
	, m_mmeScriptOrder(MME_SCRORDER_standard)
	, m_mmeShaderVariableList()
	, m_controlObjectVariables()
	, m_worldMatrixCalcMask(0)
	, m_requiredLightCount(0)
	, m_mmeShaderTechniqueList()
{
	LN_REFOBJ_SET(m_manager, manager);
	m_manager->AddShader(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader::~MMEShader()
{
	LN_FOREACH(MMEShaderTechnique* tech, m_mmeShaderTechniqueList) {
		tech->Release();
	}
	LN_FOREACH(MMEShaderVariable* var, m_mmeShaderVariableList) {
		delete var;
	}

	if (m_manager != NULL) {
		m_manager->RemoveShader(this);
		LN_SAFE_RELEASE(m_manager);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateSceneParams(const MMESceneParams& params, SceneGraphManager* scene)
{
	Graphics::ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		// �ϐ����z��̃��C�g���͂��̊֐����ł͐ݒ肵�Ȃ��̂Ŏ���
		if (sv->LightNum >= 1 && sv->LightParamIsArray) {
			continue;
		}

		var = sv->Variable;

		// CONTROLOBJECT
		if (MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END) {
			if (sv->ObjectName == _T("(self)")) {
				// �m�[�h�P�ʂ̃f�[�^�Ƃ��ĕʓr�ݒ肷�邽�߁A�����ł͓��ɉ������Ȃ�
			}
			else if (sv->ObjectName == _T("(OffscreenOwner)")) {
				LN_THROW(0, NotImplementedException);
			}
			else {
				IMMESceneObject* obj = scene->FindNodeFirst(sv->ObjectName);
				SetControllObjectParam(sv, obj);
			}
			continue;
		}

		switch (sv->Request)
		{
		case MME_VARREQ_TIME:
			var->SetFloat(params.Time);
			break;
		case MME_VARREQ_ELAPSEDTIME:
			var->SetFloat(params.ElapsedTime);
			break;
		case MME_VARREQ_MOUSEPOSITION:
			var->SetVector(params.MousePosition);
			break;
		case MME_VARREQ_LEFTMOUSEDOWN:
			var->SetVector(params.LeftMouseDown);
			break;
		case MME_VARREQ_MIDDLEMOUSEDOWN:
			var->SetVector(params.MiddleMouseDown);
			break;
		case MME_VARREQ_RIGHTMOUSEDOWN:
			var->SetVector(params.RightMouseDown);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateCameraParams(Camera* camera, const SizeF& viewPixelSize)
{
	Graphics::ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		var = sv->Variable;

		switch (sv->Request)
		{
			// �s��
		case MME_VARREQ_MATRIX_CAMERA_View:
			var->SetMatrix(camera->GetViewMatrix());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj:
			var->SetMatrix(camera->GetProjectionMatrix());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj:
			var->SetMatrix(camera->GetViewProjectionMatrix());
			break;

			// �t�s��
		case MME_VARREQ_MATRIX_CAMERA_View_I:
			var->SetMatrix(camera->GetViewMatrixI());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj_I:
			var->SetMatrix(camera->GetProjectionMatrixI());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj_I:
			var->SetMatrix(camera->GetViewProjectionMatrixI());
			break;

			// �]�u�s��
		case MME_VARREQ_MATRIX_CAMERA_View_T:
			var->SetMatrix(camera->GetViewMatrixT());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj_T:
			var->SetMatrix(camera->GetProjectionMatrixT());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj_T:
			var->SetMatrix(camera->GetViewProjectionMatrixT());
			break;

			// �t�s��~�]�u�s��
		case MME_VARREQ_MATRIX_CAMERA_View_IT:
			var->SetMatrix(camera->GetViewMatrixIT());
			break;

		case MME_VARREQ_MATRIX_CAMERA_Proj_IT:
			var->SetMatrix(camera->GetProjectionMatrixIT());
			break;

		case MME_VARREQ_MATRIX_CAMERA_ViewProj_IT:
			var->SetMatrix(camera->GetViewProjectionMatrixIT());
			break;
			// �J�����̈ʒu�E����
		case MME_VARREQ_CAMERA_POSITION:
			var->SetVector(Vector4(camera->GetPosition(), 0.0f));
			break;
		case MME_VARREQ_CAMERA_DIRECTION:
			var->SetVector(camera->GetDirectionInternal());
			break;

			// ���ˉe�s��
			//case LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC:
			//    var->SetMatrix( params.Mat_CameraOrtho );
			//    break;

			// �`���RT�T�C�Y
		case MME_VARREQ_VIEWPORTPIXELSIZE:
			var->SetVector(Vector4(viewPixelSize.Width, viewPixelSize.Height, 0, 0));
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateNodeParams(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList)
{
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		// CONTROLOBJECT
		if (MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END) {
			if (sv->ObjectName == _T("(self)")) {
				SetControllObjectParam(sv, node);
			}
			else if (sv->ObjectName == _T("(OffscreenOwner)")) {
				//LN_THROW_NotImpl( 0 );
			}
			else {
				// �V�[���P�ʂ̃p�����[�^�ݒ�Őݒ�ς�
			}
			continue;
		}

		Graphics::ShaderVariable* var = sv->Variable;

		// ���m�Ȕ͈͂���Ȃ����ǁA�����ł���������̉񐔂����炷���߂�
		if (MME_VARREQ_GEOMETRY_MATRIX_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_GEOMETRY_MATRIX_END)
		{
			// �ufloat4x4 WorldLightViewMatrix : WORLDVIEWPROJECTION < string Object = "Light"; > ;�v ���A
			// ���C�g�̍s��ƃm�[�h�̍s�����Z�������̂��K�v�ł���ꍇ
			if (sv->LightNum > 0 && node->GetAffectLightList() != NULL)
			{
				bool isSet = false;
				if (node->GetAffectLightList()->GetCount() > 0)
				{
					// �e�����C�g�� 0 ��
					Light* light = node->GetAffectLightList()->GetAt(0);
					switch (sv->Request)	// ���C�g�P�̂̍��W�ϊ��s��
					{
						// �s��
					case MME_VARREQ_MATRIX_LIGHT_View:
						var->SetMatrix(light->GetViewMatrix()/* * params.Mat_LightView*/);
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj:
						var->SetMatrix(light->GetProjectionMatrix());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj:
						var->SetMatrix(light->GetViewProjectionMatrix());
						break;

						// �t�s��
					case MME_VARREQ_MATRIX_LIGHT_View_I:
						var->SetMatrix(light->GetViewMatrixI());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_I:
						var->SetMatrix(light->GetProjectionMatrixI());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
						var->SetMatrix(light->GetViewProjectionMatrixI());
						break;

						// �]�u�s��
					case MME_VARREQ_MATRIX_LIGHT_View_T:
						var->SetMatrix(light->GetViewMatrixT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_T:
						var->SetMatrix(light->GetProjectionMatrixT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
						var->SetMatrix(light->GetViewProjectionMatrixT());
						break;

						// �t�s��~�]�u�s��
					case MME_VARREQ_MATRIX_LIGHT_View_IT:
						var->SetMatrix(light->GetViewMatrixIT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
						var->SetMatrix(light->GetProjectionMatrixIT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
						var->SetMatrix(light->GetViewProjectionMatrixIT());
						break;

						//	// ���C�g�̐F
						//case MME_VARREQ_LIGHT_DIFFUSE:
						//	var->SetVector(*params.LightDiffuse);
						//	break;
						//case MME_VARREQ_LIGHT_AMBIENT:
						//	var->SetVector(*params.LightAmbient);
						//	break;
						//case MME_VARREQ_LIGHT_SPECULAR:
						//	var->SetVector(*params.LightSpecular);
						//	break;

						//	// ���C�g�̈ʒu�E����
						//case MME_VARREQ_LIGHT_POSITION:
						//	var->SetVector(params.LightPosition);
						//	break;
						//case MME_VARREQ_LIGHT_DIRECTION:
						//	var->SetVector(params.LightDirection);
						//	break;
					}
				}

				if (!isSet)
				{
					// SetMatrixArray() �Őݒ肷��K�v�����鍀��
					if (sv->LightParamIsMatrix && sv->LightParamIsArray)
					{
						Matrix matrices[MaxAffectLights];
						for (int i = 0; i < sv->LightNum; ++i) {
							GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, i, &matrices[i]);
						}
						sv->Variable->SetMatrixArray(matrices, sv->LightNum);
					}
					// SetMatrix() �Őݒ肷��K�v�����鍀��
					else if (sv->LightParamIsMatrix && !sv->LightParamIsArray)
					{
						Matrix mat;
						GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, 0, &mat);
						sv->Variable->SetMatrix(mat);
					}
					// SetVectorArray() �Őݒ肷��K�v�����鍀��
					else if (!sv->LightParamIsMatrix && sv->LightParamIsArray)
					{
						LightNodeList* lights = node->GetAffectLightList();
						Vector4 vectors[MaxAffectLights];
						switch (sv->Request)
						{
						case MME_VARREQ_LIGHT_DIFFUSE:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetDiffuseColor());
							}
							break;
						case MME_VARREQ_LIGHT_AMBIENT:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetAmbientColor());
							}
							break;
						case MME_VARREQ_LIGHT_SPECULAR:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetSpecularColor());
							}
							break;
						case MME_VARREQ_LIGHT_POSITION:
							for (int i = 0; i < sv->LightNum; ++i) {
								const Vector3& v = lights->GetAt(i)->GetPosition();
								vectors[i].Set(v.X, v.Y, v.Z, 0.0f);
							}
							break;
						case MME_VARREQ_LIGHT_DIRECTION:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = lights->GetAt(i)->GetDirectionInternal();
							}
							break;
						}

						sv->Variable->SetVectorArray(vectors, sv->LightNum);
					}
					// �s��^�A�z��^�ł͂Ȃ� (SetVector())
					else
					{
						// �e�����C�g�� 0 ��
						Light* light = node->GetAffectLightList()->GetAt(0);
						switch (sv->Request)
						{
						case MME_VARREQ_LIGHT_DIFFUSE:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetDiffuseColor()));
							break;
						case MME_VARREQ_LIGHT_AMBIENT:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetAmbientColor()));
							break;
						case MME_VARREQ_LIGHT_SPECULAR:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetSpecularColor()));
							break;
						case MME_VARREQ_LIGHT_POSITION:
						{
							const Vector3& v = light->GetPosition();
							Vector4 vec(v, 0.0f);
							sv->Variable->SetVector(vec);
							break;
						}
						case MME_VARREQ_LIGHT_DIRECTION:
						{
							sv->Variable->SetVector(light->GetDirectionInternal());
							break;
						}
						}
					}
				}
			}
			// ���C�g�ȊO
			else
			{
				Matrix mat;
				if (GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, 0, &mat)) {
					sv->Variable->SetMatrix(mat);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateSubsetParams(const VisualNodeSubsetParams& params)
{
    Graphics::ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
        var = sv->Variable;

		switch (sv->Request)
		{
			// �f�B�t���[�Y�F�i�g�U���j
		case MME_VARREQ_OBJECT_DIFFUSE:
			var->SetVector(params.Material.Diffuse);
			break;

			// �A���r�G���g�F�i�����j
		case MME_VARREQ_OBJECT_AMBIENT:
			var->SetVector(params.Material.Ambient);
			break;

			// �G�~�b�V�����F�i���ˌ��j
		case MME_VARREQ_OBJECT_EMISSIVE:
			var->SetVector(params.Material.Emissive);
			break;

			// �X�y�L�����F�i���ʌ��j
		case MME_VARREQ_OBJECT_SPECULAR:
			var->SetVector(params.Material.Specular);
			break;

			// �X�y�L�����̋��x
		case MME_VARREQ_OBJECT_SPECULARPOWER:
			var->SetFloat(params.Material.Power);
			break;

			// �g�D�[���F
		case MME_VARREQ_OBJECT_TOONCOLOR:
			var->SetVector(params.Material.ToonColor);
			break;

			// �֊s�F
		case MME_VARREQ_OBJECT_EDGECOLOR:
			var->SetVector(params.Material.EdgeColor);
			break;

			// �}�e���A���ɐݒ肳��Ă���e�N�X�`��
		case MME_VARREQ_OBJECT_MATERIALTEXTURE:
			if (!params.Material.Texture.IsNull()) {
				var->SetTexture(params.Material.Texture);
			}
			// �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
			else {
				var->SetTexture(m_manager->GetDummyTexture());
			}
			break;

			// �}�e���A���ɐݒ肳��Ă���A�X�t�B�A�}�b�v�e�N�X�`��
		case MME_VARREQ_OBJECT_MATERIALSPHEREMAP:
			if (!params.Material.SphereTexture.IsNull()) {
				var->SetTexture(params.Material.SphereTexture);
			}
			// �e�N�X�`�����Ȃ���΃_�~�[��ݒ�
			else {
				var->SetTexture(m_manager->GetDummyTexture());
			}
			break;

			// �s�����x
		case LN_VARREQ_OPACITY:
		{
			var->SetFloat(params.Opacity);
			break;
		}
			// ��Z����F
		case LN_VARREQ_COLOR_SCALE:
		{
			var->SetVector(params.ColorScale);
			break;
		}
			// �u�����h����F
		case LN_VARREQ_BLEND_COLOR:
		{
			var->SetVector(params.BlendColor);
			break;
		}
			// �F��
		case LN_VARREQ_TONE:
		{
			var->SetVector(params.Tone);
			break;
		}
			// �e�N�X�`�����W�ϊ��s��
		case LN_VARREQ_UVTRANSFORM:
		{
			var->SetMatrix(params.UVTransform);
			break;
		}
		}
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::SetControllObjectParam(MMEShaderVariable* sv, const IMMESceneObject* obj)
{
	Graphics::ShaderVariable* var = sv->Variable;

	if (sv->Request == MME_VARREQ_CONTROLOBJECT_Visible)
	{
		if (obj == NULL) {
			var->SetBool(false);
		}
		else {
			var->SetBool(obj->IsVisible());
		}
	}
	else if (obj != NULL)
	{
		switch (sv->Request)
		{
		case MME_VARREQ_CONTROLOBJECT_Scale:
		{
			Vector4 scale;
			obj->GetMatrix().Decompose((Vector3*)&scale, NULL, NULL);
			var->SetVector(scale);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Position:
			var->SetVector((const Vector4&)obj->GetMatrix().GetPosition());
			break;
		case MME_VARREQ_CONTROLOBJECT_World:
			var->SetMatrix(obj->GetMatrix());
			break;

		case MME_VARREQ_CONTROLOBJECT_BoneOffset:
			var->SetVector(
				(const Vector4&)obj->FindFrameMarix(sv->ItemName).GetPosition());
			break;
		case MME_VARREQ_CONTROLOBJECT_BoneMatrix:
			var->SetMatrix(
				obj->FindFrameMarix(sv->ItemName));
			break;
		case MME_VARREQ_CONTROLOBJECT_MorphBlend:
			var->SetFloat(obj->FindMorphBlend(sv->ItemName));
			break;

		case MME_VARREQ_CONTROLOBJECT_X:
			var->SetFloat(obj->GetMatrix().GetPosition().X);
			break;
		case MME_VARREQ_CONTROLOBJECT_Y:
			var->SetFloat(obj->GetMatrix().GetPosition().Y);
			break;
		case MME_VARREQ_CONTROLOBJECT_Z:
			var->SetFloat(obj->GetMatrix().GetPosition().Z);
			break;
		case MME_VARREQ_CONTROLOBJECT_XYZ:
			var->SetVector((const Vector4&)obj->GetMatrix().GetPosition());
			break;

		case MME_VARREQ_CONTROLOBJECT_Rx:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.X);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Ry:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.Y);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Rz:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.Z);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Rxyz:
		{
			Vector4 rad(obj->GetMatrix().ToEulerAngles(), 0);
			var->SetVector(rad);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Si:
		{
			Vector4 scale;
			obj->GetMatrix().Decompose((Vector3*)&scale, NULL, NULL);
			var->SetFloat(scale.X);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Tr:
			var->SetFloat(obj->GetOpacity());
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MMEShader::GetGeometryTransform(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList, MMEVariableRequest req, int lightIndex, Matrix* outMatrix)
{
	switch (req)
	{
	case MME_VARREQ_MATRIX_World:
	{
		*outMatrix = node->GetMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		return true;
	}

		// �t�s��
	case MME_VARREQ_MATRIX_World_I:
	{
		*outMatrix = Matrix::Inverse(node->GetMatrix());
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Inverse();
		return true;
	}

		// �]�u�s��
	case MME_VARREQ_MATRIX_World_T:
	{
		*outMatrix = Matrix::Transpose(node->GetMatrix());
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Transpose();
		return true;
	}

		// �t�s��~�]�u�s��
	case MME_VARREQ_MATRIX_World_IT:
	{
		*outMatrix = Matrix::Inverse(node->GetMatrix());
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	}

	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique* MMEShader::FindTechnique(MMDPass mmdPass, bool UseTexture, bool UseSphereMap, bool UseToon, bool UseSelfShadow, int subsetIndex)
{
	/* MMM �ł͕����̃e�N�j�b�N���}�b�`���Ă��A���ۂɎg����̂�
	 * �t�@�C����Ő�ɋL�q����Ă��� 1 �����B
	 */

	LN_FOREACH(MMEShaderTechnique* tech, m_mmeShaderTechniqueList)
	{
		if (tech->GetMMDPass() != mmdPass) {
			continue;
		}
		if (!tech->IsUseTexture().IsNull() && tech->IsUseTexture() != UseTexture) {	// �ȗ�����Ă���ꍇ�̓}�b�`���Ă���Ƃ���
			continue;
		}
		if (!tech->IsUseSphereMap().IsNull() && tech->IsUseSphereMap() != UseSphereMap) {	// �ȗ�����Ă���ꍇ�̓}�b�`���Ă���Ƃ���
			continue;
		}
		if (!tech->IsUseToon().IsNull() && tech->IsUseToon() != UseToon) {	// �ȗ�����Ă���ꍇ�̓}�b�`���Ă���Ƃ���
			continue;
		}
		if (!tech->IsUseSelfShadow().IsNull() && tech->IsUseSelfShadow() != UseSelfShadow) {	// �ȗ�����Ă���ꍇ�̓}�b�`���Ă���Ƃ���
			continue;
		}
		if (!tech->ContainsSubsetIndex(subsetIndex)) {
			continue;
		}
		return tech;
	}
	return NULL;
}

} // namespace Scene
} // namespace Lumino
