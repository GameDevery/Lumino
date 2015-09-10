
#pragma once

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class IMMESceneObject
{
public:
	IMMESceneObject() {}
	virtual ~IMMESceneObject() {}

public:

	// CONTROLOBJECT �A�m�e�[�V�����֌W
	virtual bool IsVisible() const = 0;
	virtual const Matrix& GetMatrix() const = 0;
	virtual float GetOpacity() const = 0;
	virtual const Matrix& FindFrameMarix(const String& name) const = 0;
	virtual float FindMorphBlend(const String& name) const = 0;


	// �I�u�W�F�N�g�̃��[���h�s��ƁA�J�����܂��̓��C�g�̃r���[�E�v���W�F�N�V�����s������߂�B
	// out �ɒl���i�[�����ꍇ�� true ��Ԃ��B
	//virtual bool GetObjectMatrix(Matrix* out, MMEVariableRequest req, int lightIndex) const = 0;
	// TODO: ���̂͂ǂ݂̂��K�v�B�Ȃ�A���̂� static �ɂł��Ȃ��H
	virtual LightNodeList* GetAffectLightList() = 0;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
