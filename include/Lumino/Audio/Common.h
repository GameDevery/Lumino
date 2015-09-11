/*
	@file	Common.h
*/
#pragma once

#include <Lumino/Math/Vector3.h>
#include <Lumino/Base/Common.h>

#define LN_NAMESPACE_AUDIO_BEGIN	//namespace Audio {
#define LN_NAMESPACE_AUDIO_END		//}

#define LN_MUSIC_TIME_BASE	768	// TODO

namespace Lumino
{
namespace Audio
{

/// �����t�@�C���t�H�[�}�b�g
enum StreamFormat
{
	StreamFormat_Unknown = 0,		///< �s���ȃt�@�C��
	StreamFormat_Wave,				///< WAVE
	StreamFormat_Mp3,				///< MP3
	StreamFormat_Ogg,				///< OGG
	StreamFormat_Midi,				///< MIDI

	StreamFormat_Max,				///< (terminator)
};

/// PCM �t�H�[�}�b�g
struct WaveFormat
{
	uint16_t	FormatTag;
	uint16_t	Channels;
	uint32_t	SamplesPerSec;
	uint32_t	AvgBytesPerSec;
	uint16_t	BlockAlign;
	uint16_t	BitsPerSample;
	uint16_t	EXSize;
};

/// DirectMusic �̏��������@
enum DirectMusicInitMode
{
	DirectMusicInitMode_NotUse = 0,			///< DirectMusic ���g�p���Ȃ�
	DirectMusicInitMode_Normal,				///< �ʏ�
	DirectMusicInitMode_ThreadWait,			///< �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͑҂�
	DirectMusicInitMode_ThreadRequest,		///< �ʃX���b�h�ŏ��������āA�Đ����ɖ������̏ꍇ�͍Đ���\�񂷂�

	DirectMusicInitMode_MAX,
};

/** �����f�[�^�̓ǂݍ��ݕ��@��\���܂��B*/
enum class SoundLoadingMode
{
	Sync = 0,		/**< �����I�ɓǂݍ��� */
	ASync,			/**< �񓯊��I�ɓǂݍ��� */

	TERMINATOR,
};

/// �Đ����@
enum class SoundPlayingMode
{
	Unknown = 0,		///< �s���ȍĐ����@ (�����I��)	TODO: ���O�� Auto �ɂ���
	//SoundPlayType_Auto,				///< �����I�� ( �f�t�H���g�ł̓f�R�[�h��̃T�C�Y�� 10000 �o�C�g�ȏ�ɂȂ�ꍇ�̓X�g���[�~���O�A�����łȂ��ꍇ�̓I���������Đ��ɂȂ�܂� )
	OnMemory,			///< �I��������
	Streaming,	    ///< �X�g���[�~���O
	Midi,  			///< SMF

	TERMINATOR,
};

/** �����̍Đ���Ԃ�\���܂��B*/
enum class SoundPlayingState
{
	Stopped = 0,		/**< ��~�� */
	Playing,			/**< �Đ��� */
	Pausing,			/**< �ꎞ��~�� */

	TERMINATOR,
};

/** ���ʃt�F�[�h�������̓����\���܂��B*/
enum class SoundFadeBehavior
{
	Continue = 0,		/**< �Đ����p������ */
	Stop,				/**< ��~���� */
	StopReset,			/**< ��~���āA���̍Đ��ɔ����ăT�E���h�̉��ʂ����̒l�ɖ߂� */
	Pause,				/**< �ꎞ��~���� */
	PauseReset,			/**< �ꎞ��~���āA���̍Đ��ɔ����ăT�E���h�̉��ʂ����̒l�ɖ߂� */

	TERMINATOR,
};

/// 3D �T�E���h���X�i�[
struct SoundListener
{
	Vector3		Position;
	Vector3		Direction;
	Vector3		UpDirection;
	Vector3		Velocity;

	SoundListener()
		: Position(0, 0, 0)
		, Direction(0, 0, 1.0f)
		, UpDirection(0, 1.0f, 0)
		, Velocity(0, 0, 0)
	{}
};

} // namespace Audio
} // namespace Lumino
