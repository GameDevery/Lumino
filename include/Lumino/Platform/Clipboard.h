/**
	@file	Clipboard.h
*/
#pragma once
#include <Lumino/Base/String.h>

namespace Lumino
{
namespace Platform
{

/**
	@brief	�V�X�e���N���b�v�{�[�h�Ƃ̃f�[�^�]�����s���܂��B
*/
class Clipboard
{
public:

	/**
		@brief	�N���b�v�{�[�h�ɕ�����f�[�^���i�[���܂��B
	*/
	static void SetText(Window* window, const String& text);

	/**
		@brief	�N���b�v�{�[�h�Ɋi�[����Ă��镶����f�[�^���擾���܂��B
		@return	�i�[����Ă���f�[�^��������ł͂Ȃ��ꍇ�A��̕������Ԃ��܂��B
	*/
	static String GetText(Window* window);
};

} // namespace Platform
} // namespace Lumino
