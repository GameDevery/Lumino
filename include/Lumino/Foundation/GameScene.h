
#pragma once
#include "../Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		�Q�[���V�[���̃x�[�X�N���X�ł��B
	@details	�Q�[���V�[���̓^�C�g����ʂ�Q�[���I�[�o�[��ʂȂǂ̉�ʑJ�ڂ̒P�ʂł��B
*/
class GameScene
{
public:

	/// �J�n����
	virtual void OnStart();

	/// �t���[���X�V
	virtual void OnUpdate();

	/// �I������
	virtual void OnTerminate();

protected:
	GameScene();
	virtual ~GameScene();
};

LN_NAMESPACE_END
