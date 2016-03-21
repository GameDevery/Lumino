
#pragma once
#include <list>

LN_NAMESPACE_BEGIN
class Vector3;

LN_NAMESPACE_AUDIO_BEGIN
class AudioManagerImpl;

/*
	@brief	
*/
class GameAudioImpl
{
public:

	/// BGM �����t����
	void PlayBGM(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// �w�肳�ꂽ Sound �� BGM �Ƃ��ĉ��t����
	void PlayBGMFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGM �̉��t���~����
	void StopBGM(double fadeTime = 0.0);

	/// BGS ( ���� ) �����t����
	void PlayBGS(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// �w�肳�ꂽ Sound �� BGS �Ƃ��ĉ��t����
	void PlayBGSFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f, double fadeTime = 0.0);

	/// BGS �̉��t���~����
	void StopBGS(double fadeTime = 0.0);

	// ME ( ���ʉ��y ) �����t����
	void PlayME(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// �w�肳�ꂽ Sound �� ME �Ƃ��ĉ��t����
	void PlayMEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// ME �̉��t���~����
	void StopME();

	/// SE �����t����
	void PlaySE(const TCHAR* filePath, float volume = 1.0f, float pitch = 1.0f);

	/// SE �� 3D ��ԏ�ŉ��t����
	void PlaySE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume = 1.0f, float pitch = 1.0f);

	/// �w�肳�ꂽ Sound �� SE �Ƃ��ĉ��t����
	void PlaySEFromSound(Sound* sound, float volume = 1.0f, float pitch = 1.0f);

	/// �S�Ă� SE �̉��t���~����
	void StopSE();

	/// ME ���t���� BGM �̃t�F�[�h���Ԃ�ݒ肷��
	void SetMEFadeState(double begin, double end);

	/// �Đ�����BGM�̉��ʂ�ݒ肷�� (�t�F�[�h�A�E�g���͖���)
	void SetBGMVolume(float volume, double fadeTime = 0.0);

	/// �Đ�����BGS�̉��ʂ�ݒ肷�� (�t�F�[�h�A�E�g���͖���)
	void SetBGSVolume(float volume, double fadeTime = 0.0);

	/// ���� BGM �̉��t�ĊJ�t���O�̐ݒ�
	void SetEnableBGMRestart(bool flag) { mBGMRestart = flag; }

	/// ���� BGS �̉��t�ĊJ�t���O�̐ݒ�
	void SetEnableBGSRestart(bool flag) { mBGSRestart = flag; }

private:
	friend class AudioManagerImpl;
	GameAudioImpl(AudioManagerImpl* mamager);
	~GameAudioImpl();

private:
	void Polling();
	void PushReleaseAtPlayEndList(Sound* sound);

private:
	typedef std::list<Sound*>	ReleaseAtPlayEndList;
	typedef std::list<Sound*>	SoundList;

	AudioManagerImpl*			mManager;
	Threading::Mutex			mLock;
	ReleaseAtPlayEndList        mReleaseAtPlayEndList;  ///< �Đ��I�����ɉ�����鉹�����X�g
	Sound*		                mBGM;
	Sound*		                mBGS;
	Sound*						mME;
	String			            mBGMName;
	String				        mBGSName;
	float						mBGMVolume;
	float						mBGMPitch;
	float						mBGSVolume;
	float						mBGSPitch;
	double						mBGMFadeOutTime;
	double						mBGMFadeInTime;
	bool                        mMEPlaying;
	bool                        mBGMRestart;
	bool                        mBGSRestart;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
