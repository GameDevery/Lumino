
#pragma once


#include "Include.h"


/**
	@brief	�����̍Đ��A������s���܂��B
*/
LN_CLASS(A, Prop)
class Sound
	: public Object
{
	LN_OBJECT;
public:


	/**
		@brief	Sound �N���X�̃C���X�^���X���쐬���܂��B
	*/
	static SoundPtr create(const Char* filePath);


	/**
		@brief	Sound �N���X�̃C���X�^���X���쐬���܂��B
		@param[in] a : test1
	*/
	static SoundPtr create(int a);

	/** @copydoc create(int) */
	static SoundPtr create(Stream* stream, SoundLoadingMode loadingMode);	

public:
	
	/**
		@brief		���̉����̉��ʂ�ݒ肵�܂��B
		@param[in]	volume	: ���� (0.0�`1.0�B�����l�� 1.0)
	*/
	LN_METHOD(Property)
	void setVolume(float volume);

	/**
		@brief		���̉����̉��ʂ��擾���܂��B
	*/
	LN_METHOD(Property)
	float getVolume() const;

	/**
		@brief		���̉����̃s�b�` (����) ��ݒ肵�܂��B
		@param[in]	volume	: �s�b�` (0.5�`2.0�B�����l�� 1.0)
	*/
	LN_METHOD(Property)
	void setPitch(float pitch);

	/**
		@brief		���̉����̃s�b�` (����) ���擾���܂��B
	*/
	LN_METHOD(Property)
	float getPitch() const;

	/**
		@brief		���[�v�Đ��̗L����ݒ肵�܂��B
		@param[in]	enabled		: ���[�v�Đ����邩
	*/
	LN_METHOD(Property)
	void setLoopEnabled(bool enabled);

	/**
		@brief		���[�v�Đ����L�������m�F���܂��B
	*/
	LN_METHOD(Property)
	bool isLoopEnabled() const;

	/**
		@brief		���[�v�͈͂�ݒ肵�܂��B
		@param[in]	begin		: ���[�v�͈͂̊J�n�T���v��
		@param[in]	length		: ���[�v�͈͂̃T���v����
		@details	MIDI �̏ꍇ�A���[�v�͈͂̓~���[�W�b�N�^�C���P�� (�l�������ЂƂ��� 768 �ŕ\��) �Ŏw�肵�܂��B 
	*/
	LN_METHOD()
	void SetLoopRange(uint32_t begin, uint32_t length);

	/**
		@brief		���̉����̍Đ����J�n���܂��B
	*/
	LN_METHOD()
	void play();

	/**
		@brief		���̉����̍Đ����~���܂��B
	*/
	LN_METHOD()
	void stop();

	/**
		@brief		���̉����̍Đ����ꎞ��~���܂��B
	*/
	LN_METHOD()
	void pause();

	/**
		@brief		�ꎞ��~���̍Đ����ĊJ���܂��B
	*/
	LN_METHOD()
	void Resume();

	/**
		@brief		�T�E���h�� 3D �����Ƃ��čĐ����邩��ݒ肵�܂��B(�K��l:false)
		@details	�ݒ�� play() �̑O�ɍs���K�v������܂��B
	*/
	void set3DEnabled(bool enabled);

	/**
		@brief		���̉����� 3D �����ł��邩���m�F���܂��B
	*/
	bool is3DEnabled() const;

	/**
		@brief		3D �����Ƃ��Ă̈ʒu��ݒ肵�܂��B
	*/
	void setEmitterPosition(const Vector3& position);

	/**
		@brief		3D �����Ƃ��Ă̈ʒu���擾���܂��B
	*/
	const Vector3& getEmitterPosition() const;
	
	/**
		@brief		3D �����Ƃ��Ă̑��x��ݒ肵�܂��B
	*/
	void setEmitterVelocity(const Vector3& velocity);

	/**
		@brief		3D �����Ƃ��Ă̈ʒu���擾���܂��B
	*/
	const Vector3& getEmitterVelocity() const;

	/**
		@brief		3D �����Ƃ��Ă̌���������ݒ肵�܂��B(default:100)
	*/
	void setEmitterMaxDistance(float distance);

	/**
		@brief		�����f�[�^�̑S�T���v�������擾���܂��B
		@details	MIDI �̏ꍇ�̓~���[�W�b�N�^�C���P�� (�l�������ЂƂ��� 768) �ŁA
					1�x�ł� play() �ōĐ����J�n���Ă��Ȃ��Ǝ擾�ł��܂���B
	*/
	int64_t getTotalSamples() const;

	/**
		@brief		���݂̍Đ��T���v�������擾���܂��B
	*/
	int64_t getPlayedSamples() const;

	/**
		@brief		�T���v�����O���[�g���擾���܂��B
		@details	MIDI �̏ꍇ�� 768 ��Ԃ��܂��B
	*/
	int getSamplingRate() const;

	/**
		@brief		���̉����̌��݂̍Đ���Ԃ��擾���܂��B
	*/
	SoundPlayingState getPlayingState() const;

	/**
		@brief		�����f�[�^�̍Đ����@��ݒ肵�܂��B(�K��l:Unknown)
		@details	�ݒ�� play() �̑O�ɍs���K�v������܂��B
	*/
	void SetPlayingMode(SoundPlayingMode mode);
	
	/**
		@brief		�����f�[�^�̍Đ����@���擾���܂��B
	*/
	SoundPlayingMode getPlayingMode() const;

	/**
		@brief		���ʂ̃t�F�[�h���J�n���܂��B
		@param[in]	targetVolume	: �t�F�[�h�批��
		@param[in]	time			: �ω��ɂ����鎞�� (�b)
		@param[in]	state			: ���ʃt�F�[�h�������̓���
		@details	���݂̉��ʂ��� targetVolume �։��ʂ̑J�ڂ��s���܂��B���݂̉��ʂ� getVolume() �Ŏ擾�ł���l�ł��B
					�t�F�[�h���͉��ʂ��ύX����AgetVolume() �Ŏ擾�ł���l���ς��܂��B
	*/
	void fadeVolume(float targetVolume, double time, SoundFadeBehavior behavior);

	/**
		@brief		���ʂ̃t�F�[�h���ł��邩���m�F���܂��B
	*/
	bool isVolumeFading() const;

LN_INTERNAL_ACCESS:
	Sound();
	virtual ~Sound();
	LN_METHOD()
	void initialize(const StringRef& filePath);
	void initialize(Stream* stream, SoundLoadingMode loadingMode);
	void initialize(detail::AudioStream* audioStream);
	void createAudioPlayerSync();
	void polling(float elapsedTime);

	void setGameAudioFlags(uint32_t flags) { m_gameAudioFlags = flags; }
	uint32_t getGameAudioFlags() const { return m_gameAudioFlags; }
	detail::AudioStream* getAudioStream() const { return m_audioStream; }

private:
	detail::AudioManager*		m_manager;
	Mutex						m_mutex;
	detail::AudioStream*		m_audioStream;
	detail::AudioPlayer*		m_audioPlayer;
	SoundPlayingMode			m_playingMode;

	Mutex						m_playerStateLock;	// TODO: ������A�X�s�����b�N�̕���������������������Ȃ�
	detail::AudioPlayerState	m_playerState;
	bool						m_is3DSound;
	Vector3						m_position;
	Vector3						m_velocity;
	float						m_maxDistance;

	uint32_t					m_gameAudioFlags;

	EasingValue<float, double>	m_fadeValue;
	SoundFadeBehavior			m_fadeBehavior;
	bool						m_fading;
};
