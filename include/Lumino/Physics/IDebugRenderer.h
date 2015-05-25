
#pragma once
#include <Lumino/Math/Vector3.h>

namespace Lumino
{
namespace Physics
{

/**
	@brief		�f�o�b�O�p�̌`��`��C���^�[�t�F�C�X�ł��B
*/
class IDebugRenderer
{
public:

	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;

};

} // namespace Physics
} // namespace Lumino
