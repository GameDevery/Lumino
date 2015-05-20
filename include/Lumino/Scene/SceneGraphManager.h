
#pragma once

namespace Lumino
{
namespace Scene
{
	
/**
	@brief	�V�[���O���t�̊Ǘ��N���X�ł��B
*/
class SceneGraphManager
	: public RefObject
{
public:

	/// �������f�[�^
	struct ConfigData
	{
		Lumino::FileManager*		FileManager;
		Graphics::GraphicsManager*  GraphicsManager;
	};

public:

	/**
		@brief	SceneGraphManager �̃C���X�^���X���쐬���܂��B
	*/
	SceneGraphManager* Create(const ConfigData& configData);

protected:
	SceneGraphManager();
	virtual ~SceneGraphManager();
};

} // namespace Scene
} // namespace Lumino
