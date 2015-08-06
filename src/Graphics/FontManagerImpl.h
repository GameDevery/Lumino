
#pragma once

#include <Lumino/Imaging/FontManager.h>

namespace Lumino
{
namespace Imaging
{

/// �t�H���g�̊Ǘ��N���X�̎���
///		�����N���X�𕪂��Ă���̂́AFreeType �̃w�b�_�ނ����J���Ȃ��悤�ɂ��邽�߁B
class FontManagerImpl
	: public FontManager
{
public:
	FontManagerImpl(FileManager* fileManager);
	~FontManagerImpl();

private:
	RefPtr<FileManager>	m_fileManager;
};

} // namespace Imaging
} // namespace Lumino
