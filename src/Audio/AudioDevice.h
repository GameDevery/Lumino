/*
	@file	AudioDevice.h
*/
#pragma once

#include <Lumino/Base/Cache.h>
#include <Lumino/IO/Stream.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

class AudioStream;
class AudioPlayer;

/// 3D �T�E���h���X�i�[
struct SoundListenerData
{
	Vector3		Position;
	Vector3		Direction;
	Vector3		UpDirection;
	Vector3		Velocity;

	SoundListenerData()
		: Position(0, 0, 0)
		, Direction(0, 0, 1.0f)
		, UpDirection(0, 1.0f, 0)
		, Velocity(0, 0, 0)
	{}
};

/// �f�o�C�X�Ǘ��ƁAPlayer �̐����E�Ǘ����s���N���X�̃x�[�X�N���X
class AudioDevice
	: public RefObject
{
public:
	AudioDevice() {}
	virtual ~AudioDevice() {}
public:

	/// 3D�T�E���h���X�i�[�̎擾
	SoundListenerData* getSoundListenerData() { return &m_soundListenerData; }

public:

	/// AudioPlayer ���쐬���� (type �� LN_SOUNDPLAYTYPE_AUTO �͎w��ł��Ȃ��̂Œ���)
	virtual AudioPlayer* CreateAudioPlayer(AudioStream* source, bool enable3d, SoundPlayingMode mode) = 0;
	/// �X�V (�X�V�X���b�h����Ă΂��)
	virtual void Update() = 0;
	//virtual void SetListenerState(const Vector3& position, const Vector3& front) = 0;
	/// 3D ��Ԃ�1���[�g�������̋����̐ݒ�
	virtual void SetMetreUnitDistance(float d) = 0;

protected:
	SoundListenerData		m_soundListenerData;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
