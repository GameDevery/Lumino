
#pragma once

#include "../Modeling/Model.h"
#include "VisualNode.h"

namespace Lumino
{
namespace Scene
{

/// Model
class Model
	: public VisualNode
{
public:
	Model();
	virtual ~Model();

public:

	/// �쐬
	void Create(const TCHAR* filePath, SceneGraphManager* manager = NULL);

	/// ���̃��f���ɑ΂���A�j���[�V�������s���N���X�̎擾
	Animation::Animator* GetAnimator() { return m_model->GetAnimator(); }

protected:
	virtual void UpdateFrame(float elapsedTime);
	virtual void UpdateNodeRenderingParams(MMEShader* priorityShader);
	virtual void DrawSubset(RenderingParams& params, int subsetIndex);

private:
	RefPtr<Modeling::Model>	m_model;
};

} // namespace Scene
} // namespace Lumino
