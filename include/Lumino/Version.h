
#ifndef LUMINO_VERSION_H
#define LUMINO_VERSION_H

#define LUMINO_VERSION_MAJOR		0			/**< ���W���[�o�[�W���� */
#define LUMINO_VERSION_MINOR		1			/**< �}�C�i�[�o�[�W���� */
#define LUMINO_MERSION_REVISION		0			/**< �p�b�`�ԍ� */
#define LUMINO_VERSION_STRING		_T("0.1.0")	/**< �o�[�W���������� */

LN_NAMESPACE_BEGIN

/**
	@brief		���C�u�����̃o�[�W�������ł��B
	@details	���̃N���X����擾�ł���l�̓��C�u�����̃o�C�i���t�@�C���̃o�[�W�����ԍ��ł��B
				�w�b�_�t�@�C���̃o�[�W������v���v���Z�X�Ŏg�p�������ꍇ�� 
				LUMINO_VERSION_MAJOR �� LUMINO_VERSION_MINOR ���g�p���Ă��������B
*/
namespace Version
{

/**
	@brief	���W���[�o�[�W�������擾���܂��B
*/
int GetMajor();

/**
	@brief	�}�C�i�[�o�[�W�������擾���܂��B
*/
int GetMinor();

/**
	@brief	���r�W�����o�[�W�������擾���܂��B
*/
int GetRevision();

/**
	@brief	�o�[�W����������̎擾���擾���܂��B
*/
const TCHAR* GetString();

/**
	@brief	�w�肵���o�[�W�����ԍ��ƁA���C�u�����t�@�C���̃R���p�C���o�[�W�����ԍ����r���܂��B
	@return	�w��o�[�W���� >= �R���p�C���o�[�W���� �ł���ꍇ�Atrue ��Ԃ��܂��B
*/
bool IsAtLeast(int major = LUMINO_VERSION_MAJOR, int minor = LUMINO_VERSION_MINOR, int revision = LUMINO_MERSION_REVISION);


} // namespace Version
LN_NAMESPACE_END

#endif // LUMINO_VERSION_H
