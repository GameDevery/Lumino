
#include "../Internal.h"
#include <Lumino/Audio/Sound.h>
#include "AudioManager.h"
#include "GameAudioImpl.h"
#include "AudioHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

enum GameAudioFlags
{
	GameAudioFlags_SE = 0x01,
};

//=============================================================================
// GameAudioImpl
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameAudioImpl::GameAudioImpl(AudioManagerImpl* mamager)
	: mManager(mamager)
	, mBGM(NULL)
	, mBGS(NULL)
	, mME(NULL)
	, mBGMVolume(0)
	, mBGMPitch(0)
	, mBGSVolume(0)
	, mBGSPitch(0)
	, mBGMFadeOutTime(0.5)
	, mBGMFadeInTime(1.0)
	, mMEPlaying(false)
	, mBGMRestart(false)
	, mBGSRestart(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GameAudioImpl::~GameAudioImpl()
{
	StopBGM(0);
	StopBGS(0);
	StopME();
	StopSE();

	// �Đ��I����̉�����X�g�ɓ����Ă���T�E���h�����
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		(*itr)->Release();
	}
	mReleaseAtPlayEndList.clear();

	LN_SAFE_RELEASE(mBGM);
	LN_SAFE_RELEASE(mBGS);
	LN_SAFE_RELEASE(mME);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayBGM(const TCHAR* filePath, float volume, float pitch, double fadeTime)
{
	// ���t�ĊJ�`�F�b�N
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( mBGMName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> sound(Sound::Create(filePath, mManager), false);
	//mManager->createSound( filePath, SOUNDPLAYTYPE_STREAMING, false ) );

	PlayBGMFromSound(sound, volume, pitch, fadeTime);

	// �t�@�C�����L��
	mBGMName = filePath;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayBGMFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	LN_CHECK_ARGS(sound != NULL);

	// ���t�ĊJ�`�F�b�N
	// TODO:�t�F�[�h�A�E�g���ɍĊJ����Ɩ����ɂȂ�
	//if ( !mBGMRestart && mBGM )
	//{
	//	if ( sound_ == mBGM && volume_ == mBGMVolume && pitch_ == mBGMPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> prev_bgm(mBGM, false);

	{
		Threading::MutexScopedLock lock(mLock);

		mBGM = sound;
		LN_SAFE_ADDREF(mBGM);

		mBGM->SetPitch(pitch);
		mBGM->SetLoopEnabled(true);

		mBGMVolume = volume;
		mBGMPitch = pitch;

		// ME ���Đ����ł͂Ȃ����𒲂ׂ�
		bool me_not_play = true;
		if (mME)
		{
			SoundPlayingState state = mME->GetPlayingState();
			if (state == SoundPlayingState::Playing)
			{
				me_not_play = false;
			}
		}

		// ME �Đ����ł͂Ȃ��ꍇ
		if (me_not_play)
		{
			if (fadeTime > 0)
			{
				mBGM->SetVolume(0);
				mBGM->Play();
				mBGM->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
			}
			else
			{
				mBGM->SetVolume(volume);
				mBGM->Play();
			}
		}
		// ME �Đ����̏ꍇ�́A��x�Đ����邯�ǂ����Ɉꎞ��~���� ( ME �I����ɍĊJ )
		else
		{
			mBGM->Play();
			mBGM->Pause();
		}
	}


	// �t�F�[�h���Ԃ�����ꍇ
	if (fadeTime > 0)
	{
		if (prev_bgm != NULL)
		{
			// �ЂƂO��BGM�́Afade_time_ ��ɒ�~�A�������悤�ɂ���
			prev_bgm->SetLoopEnabled(false);
			prev_bgm->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			PushReleaseAtPlayEndList(prev_bgm);
		}
	}
	// �t�F�[�h���Ԃ��Ȃ��ꍇ�͂����ɒ�~
	else
	{
		if (prev_bgm != NULL)
		{
			prev_bgm->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::StopBGM(double fadeTime)
{
	if (mBGM)
	{
		if (fadeTime > 0)
		{
			// �t�F�[�h�I����ɒ�~���āA���ʓ������ɖ߂�
			mBGM->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGM->Stop();
			LN_SAFE_RELEASE(mBGM);
		}

		mBGMName = _T("");
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayBGS(const TCHAR* filePath, float volume, float pitch, double fadeTime)
{
	// ���t�ĊJ�`�F�b�N
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( mBGSName == filename_ )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> sound(Sound::Create(filePath, mManager), false);

	PlayBGSFromSound(sound, volume, pitch, fadeTime);

	// �t�@�C�����L��
	mBGSName = filePath;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayBGSFromSound(Sound* sound, float volume, float pitch, double fadeTime)
{
	// ���t�ĊJ�`�F�b�N
	//if ( !mBGSRestart && mBGS )
	//{
	//	if ( sound_ == mBGS && volume_ == mBGSVolume && pitch_ == mBGSPitch )
	//	{
	//		return LN_OK;
	//	}
	//}

	RefPtr<Sound> prev_bgs(mBGS, false);

	{
		Threading::MutexScopedLock lock(mLock);

		mBGS = sound;
		LN_SAFE_ADDREF(mBGS);

		mBGS->SetPitch(pitch);
		mBGS->SetLoopEnabled(true);

		mBGSVolume = volume;
		mBGSPitch = pitch;

		if (fadeTime > 0)
		{
			mBGS->SetVolume(0);
			mBGS->Play();
			mBGS->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
		}
		else
		{
			mBGS->SetVolume(volume);
			mBGS->Play();
		}
	}

	// �t�F�[�h���Ԃ�����ꍇ
	if (fadeTime > 0)
	{
		if (prev_bgs != NULL)
		{
			// �ЂƂO��BGS�́Afade_time_ ��ɒ�~�A�������悤�ɂ���
			prev_bgs->SetLoopEnabled(false);
			prev_bgs->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
			PushReleaseAtPlayEndList(prev_bgs);
		}
	}
	// �t�F�[�h���Ԃ��Ȃ��ꍇ�͂����ɒ�~
	else
	{
		if (prev_bgs != NULL)
		{
			prev_bgs->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::StopBGS(double fadeTime)
{
	if (mBGS)
	{
		if (fadeTime > 0)
		{
			// �t�F�[�h�I����ɒ�~���āA���ʓ������ɖ߂�
			mBGS->FadeVolume(0, fadeTime, SoundFadeBehavior::StopReset);
		}
		else
		{
			mBGS->Stop();
		}

		mBGSName = _T("");
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayME(const TCHAR* filePath, float volume, float pitch)
{
	RefPtr<Sound> sound(Sound::Create(filePath, mManager), false);
	PlayMEFromSound(sound, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlayMEFromSound(Sound* sound, float volume, float pitch)
{
	StopME();

	mME = sound;
	LN_SAFE_ADDREF(mME);

	mME->SetVolume(volume);
	mME->SetPitch(pitch);
	mME->SetLoopEnabled(false);

	bool flag = false;  // BGM ������A�Đ�����Ă��邩�������t���O

						// BGM ������ꍇ
	if (mBGM)
	{
		SoundPlayingState state = mBGM->GetPlayingState();

		// �Đ�����Ă���ꍇ
		if (state == SoundPlayingState::Playing)
		{
			flag = true;
		}
	}

	// BGM ���Đ����̏ꍇ
	if (flag)
	{
		// �t�F�[�h�A�E�g���Ԃ�����ꍇ
		if (mBGMFadeOutTime > 0)
		{
			// �t�F�[�h�A�E�g��A�ꎞ��~����
			mBGM->FadeVolume(0, mBGMFadeOutTime, SoundFadeBehavior::Pause);
		}
		// �t�F�[�h�A�E�g���Ԃ��Ȃ��ꍇ
		else
		{
			// �����Ɉꎞ��~
			mBGM->Pause();
		}
	}
	// BGM ���Ȃ��ꍇ�͂������t�J�n
	else
	{
		mME->Play();
		mMEPlaying = true;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::StopME()
{
	if (mME)
	{
		mME->Stop();
		LN_SAFE_RELEASE(mME);

		// BGM �������āA�ꎞ��~���̏ꍇ�͍ĊJ
		if (mBGM)
		{
			SoundPlayingState state = mBGM->GetPlayingState();

			if (state == SoundPlayingState::Playing)
			{
				mBGM->FadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
				mBGM->Resume();
			}
		}

		mMEPlaying = false;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlaySE(const TCHAR* filePath, float volume, float pitch)
{
	RefPtr<Sound> sound(Sound::Create(filePath, mManager), false);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);

	// �{�����[���E�s�b�`�ݒ�
	sound->SetVolume(volume);
	sound->SetPitch(pitch);

	// �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// �Đ�
	sound->SetLoopEnabled(false);
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlaySE3D(const TCHAR* filePath, const Vector3& position, float distance, float volume, float pitch)
{
	// �T�E���h�쐬
	RefPtr<Sound> sound(Sound::Create(filePath, mManager), false);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->Set3DEnabled(true);

	// �ʒu�E�s�b�`�ݒ�
	sound->SetEmitterPosition(position);
	sound->SetEmitterMaxDistance(distance);
	sound->SetVolume(volume);
	sound->SetPitch(pitch);

	// �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// �Đ�
	sound->SetLoopEnabled(false);
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PlaySEFromSound(Sound* srcSound, float volume, float pitch)
{
	// �󂯎���� Sound �������Ă���\�[�X�����ƂɐV���� Sound ���쐬
	RefPtr<Sound> sound(AudioHelper::CreateSound(mManager, AudioHelper::GetAudioStream(srcSound)), false);
	sound->SetPlayingMode(SoundPlayingMode::OnMemory);
	sound->Set3DEnabled(srcSound->Is3DEnabled());

	// �ʒu�E�{�����[���E�s�b�`�ݒ�
	sound->SetVolume(volume);
	sound->SetPitch(pitch);
	if (srcSound->Is3DEnabled())
	{
		sound->SetEmitterPosition(srcSound->GetEmitterPosition());
	}

	// �Đ��r���ŉ������悤�Ƃ��Ă��Đ��I���܂ł͉������Ȃ� & SE �Ƃ��čĐ�����
	AudioHelper::SetGameAudioFlags(sound, GameAudioFlags_SE);
	PushReleaseAtPlayEndList(sound);

	// �Đ�
	sound->SetLoopEnabled(false);
	sound->Play();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::StopSE()
{
	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; ++itr)
	{
		if (AudioHelper::GetGameAudioFlags(*itr) & GameAudioFlags_SE)
		{
			(*itr)->Stop();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::SetMEFadeState(double begin, double end)
{
	if (begin >= 0)
	{
		mBGMFadeOutTime = begin;
	}
	if (end >= 0)
	{
		mBGMFadeInTime = end;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::SetBGMVolume(float volume, double fadeTime)
{
	Threading::MutexScopedLock lock(mLock);

	if (mBGM != NULL)// && !mBGM->IsVolumeFading())
	{
		mBGMVolume = volume;
		mBGM->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::SetBGSVolume(float volume, double fadeTime)
{
	Threading::MutexScopedLock lock(mLock);

	// GameAudioImpl ���ł� SOUNDFADE_STOP_RESET == �t�F�[�h�A�E�g��
	if (mBGS != NULL)// && !mBGM->IsVolumeFading())
	{
		mBGSVolume = volume;
		mBGS->FadeVolume(volume, fadeTime, SoundFadeBehavior::Continue);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//Sound* GameAudioImpl::GetInternalGameSound( InternalGameSound type )
//{
//	switch ( type )
//	{
//		case InternalGameSound_BGM:
//			return mBGM;
//		case InternalGameSound_BGS:
//			return mBGS;
//		case InternalGameSound_ME:
//			return mME;
//	}
//	return NULL;
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::Polling()
{
	Threading::MutexScopedLock lock(mLock);

	// ���t���� ME ������ꍇ
	if (mME)
	{
		SoundPlayingState mestate = mME->GetPlayingState();

		// BGM ������ꍇ
		if (mBGM)
		{
			SoundPlayingState bgmstate = mBGM->GetPlayingState();

			// BGM�̃t�F�[�h�A�E�g���I����Ĉꎞ��~��ԂɂȂ��Ă���ꍇ
			if (bgmstate == SoundPlayingState::Pausing)
			{
				// ME �Đ��J�n
				if (!mMEPlaying)
				{
					mME->SetLoopEnabled(false);
					mME->Play();
					mMEPlaying = true;
				}
				// ME �̍Đ����I�������ꍇ
				else if (mestate != SoundPlayingState::Playing)
				{
					// ME �Đ����� BGM ���X�g�b�v�����Ƃ��ŉ������Ă���ꍇ�͂Ȃɂ����Ȃ�
					if (mBGM)
					{
						mBGM->FadeVolume(mBGMVolume, mBGMFadeInTime, SoundFadeBehavior::Continue);
						mBGM->Resume();
					}
					LN_SAFE_RELEASE(mME);
					mMEPlaying = false;
				}
			}
		}
		// BGM ���Ȃ��ꍇ
		else
		{
			// ME ���I�������ꍇ
			if (mestate != SoundPlayingState::Playing)
			{
				LN_SAFE_RELEASE(mME);
				mMEPlaying = false;
			}
		}
	}

	/// �Đ��I�����Ă��鉹���̓��X�g����O���ĉ������

	ReleaseAtPlayEndList::iterator itr = mReleaseAtPlayEndList.begin();
	ReleaseAtPlayEndList::iterator end = mReleaseAtPlayEndList.end();
	for (; itr != end; )
	{
		SoundPlayingState state = (*itr)->GetPlayingState();
		if (state != SoundPlayingState::Playing)
		{
			(*itr)->Release();
			itr = mReleaseAtPlayEndList.erase(itr);
			end = mReleaseAtPlayEndList.end();
		}
		else
		{
			++itr;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GameAudioImpl::PushReleaseAtPlayEndList(Sound* sound)
{
	if (sound)
	{
		Threading::MutexScopedLock lock(mLock);
		mReleaseAtPlayEndList.push_back(sound);
		LN_SAFE_ADDREF(sound);
	}
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
