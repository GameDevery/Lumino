
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

namespace detail
{
	
/*
	�ʃX���b�h�ł̃f�R�[�h�E�Đ��Ǘ����s�������̂ŁA�ʃX���b�h�ŏ������Ɋe��X�e�[�g��ύX���Ăق����Ȃ��B
	Polling �ŃX�e�[�g���ۂ��ƃR�s�[���Ă����B
	Sound �N���X�̎��X�e�[�g��ύX����֐��͑S�� mutex ����Ȃ���΂Ȃ�Ȃ��B�ǂݎ��͖����Ă������B
*/
class AudioPlayerState
{
public:
	enum ModifiedFlags
	{
		ModifiedFlags_None = 0x0000,
		ModifiedFlags_Volume = 0x0001,
		ModifiedFlags_Pitch = 0x0002,
		ModifiedFlags_LoopEnabled = 0x0004,
		ModifiedFlags_LoopBegin = 0x0008,
		ModifiedFlags_LoopLength = 0x0010,
		ModifiedFlags_PlayingState = 0x0020,
	};

public:
	AudioPlayerState();

	void				SetModifiedFlags(uint32_t v) { m_modified = v; }
	uint32_t			GetModifiedFlags() const { return m_modified; }
	void				SetVolume(float v) { m_volume = v; m_modified |= ModifiedFlags_Volume; }
	float				GetVolume() const { return m_volume; }
	void				SetPitch(float v) { m_pitch = v; m_modified |= ModifiedFlags_Pitch; }
	float				GetPitch() const { return m_pitch; }
	void				SetLoopEnabled(bool v) { m_loopEnabled = v; m_modified |= ModifiedFlags_LoopEnabled; }
	bool				IsLoopEnabled() const { return m_loopEnabled; }
	void				SetLoopBegin(uint32_t v) { m_loopBegin = v; m_modified |= ModifiedFlags_LoopBegin; }
	uint32_t			GetLoopBegin() const { return m_loopBegin; }
	void				SetLoopLength(uint32_t v) { m_loopLength = v; m_modified |= ModifiedFlags_LoopLength; }
	uint32_t			GetLoopLength() const { return m_loopLength; }
	void				SetPlayingState(SoundPlayingState v) { m_playingState = v; m_modified |= ModifiedFlags_PlayingState; }
	SoundPlayingState	GetPlayingState() const { return m_playingState; }

private:
	uint32_t			m_modified;			// ModifiedFlags
	float				m_volume;			// ����
	float				m_pitch;			// �s�b�`
	bool			    m_loopEnabled;		// ���[�v�L��
	uint32_t			m_loopBegin;		// ���[�v�����̈�̍ŏ��̃T���v�� (Midi �Ȃ� �~���[�W�b�N�^�C���P��)
	uint32_t			m_loopLength;		// ���[�v�̈�̒��� (�T���v�����P��)  (Midi �Ȃ� �~���[�W�b�N�^�C���P��)
	SoundPlayingState	m_playingState;		// �Đ����
};

} // namespace detail

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
