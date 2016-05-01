/*
	���Q�l
	Cri API
	http://www.criware.jp/adx2le/docs/windows/index_man.html

*/
#include "Internal.h"
#include "AudioDevice.h"
#ifdef LN_OS_WIN32
	#include "XAudio2/XAudio2AudioDevice.h"
	#include "DirectMusic/DirectMusicAudioDevice.h"
#endif
#include "NullAudioDevice.h"
#include <Lumino/Base/Environment.h>
#include <Lumino/IO/ASyncIOObject.h>
#include <Lumino/Audio/Sound.h>
#include <Lumino/Audio/GameAudio.h>
#include "AudioUtils.h"
#include "WaveDecoder.h"
#include "MidiDecoder.h"
#include "GameAudioImpl.h"
#include "AudioManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

//class ASyncAudioStreamLoadTask
//	: public AsyncIOTask
//{
//public:
//	RefPtr<AudioStream>	m_audioStream;
//	RefPtr<Stream>		m_sourceStream;
//
//public:
//	virtual void Execute()
//	{
//		try
//		{
//			m_audioStream->Create(m_sourceStream);
//		}
//		catch (Exception* e) {
//			m_audioStream->OnCreateFinished(e);
//		}
//	}
//};
//
//=============================================================================
// AudioManagerImplImpl
//=============================================================================

AudioManagerImpl* Internal::AudioManager = NULL;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl* AudioManagerImpl::GetInstance(AudioManagerImpl* priority)
{
	if (priority != nullptr)
		return priority;
	return Internal::AudioManager;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl* AudioManagerImpl::Create(const Settings& settings)
{
	return LN_NEW AudioManagerImpl(settings);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl::AudioManagerImpl(const Settings& settings)
	: m_fileManager(NULL)
	, m_audioDevice(NULL)
	, m_midiAudioDevice(NULL)
	, m_gameAudio(NULL)
	, mOnMemoryLimitSize(100000)
	, m_resourceMutex()
	, m_audioStreamCache(NULL)
	, m_addingSoundList()
	, m_soundList()
	, m_soundListMutex()
	, m_endRequested()
	, m_pollingThread()
{
	m_fileManager = settings.FileManager;

#ifdef LN_OS_WIN32
	if (m_audioDevice == NULL)
	{
		RefPtr<XAudio2AudioDevice> device(LN_NEW XAudio2AudioDevice(), false);
		if (device->Initialize()) {
			device.SafeAddRef();
			m_audioDevice = device;
		}
		//mAudioDevice = LN_NEW NullAudioDevice();
	}
	if (m_midiAudioDevice == NULL)
	{
		RefPtr<DirectMusicAudioDevice> device(LN_NEW DirectMusicAudioDevice(), false);
		DirectMusicAudioDevice::ConfigData data;
		data.DMInitMode = settings.DMInitMode;
		data.hWnd = (HWND)settings.hWnd;
		data.ReverbLevel = settings.DirectMusicReverbLevel;
		device->Initialize(data);
		device.SafeAddRef();
		m_midiAudioDevice = device;
	}
#else
#endif
	if (m_audioDevice == nullptr)
	{
		m_audioDevice = LN_NEW NullAudioDevice();
	}

	// �L���b�V��������
	m_audioStreamCache = LN_NEW CacheManager(settings.StreamCacheObjectCount, settings.StreamSourceCacheMemorySize);

	// GameAudio
	m_gameAudio = LN_NEW GameAudioImpl(this);

	// �|�[�����O�X���b�h�J�n
	m_pollingThread.Start(Delegate<void()>(this, &AudioManagerImpl::Thread_Polling));

	Internal::AudioManager = this;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioManagerImpl::~AudioManagerImpl()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioManagerImpl::Finalize()
{
	// �|�[�����O�X���b�h�̏I����҂�
	m_endRequested.SetTrue();
	m_pollingThread.Wait();

	// GameAudio �̃f�X�g���N�^�ł� Sound::Stop ���Ă΂��̂ŁA
	// �����艺�� Sound �폜�����̑O�� delete ����B
	LN_SAFE_DELETE(m_gameAudio);

	// �����c���Ă���΍폜����
	for (Sound* sound : m_addingSoundList) {
		sound->Release();
	}
	m_addingSoundList.Clear();

	// �����c���Ă���΍폜����
	for (Sound* sound : m_soundList) {
		sound->Release();
	}
	m_soundList.Clear();


	if (m_audioStreamCache != NULL) {
		m_audioStreamCache->Finalize();
		LN_SAFE_RELEASE(m_audioStreamCache);
	}
	LN_SAFE_RELEASE(m_midiAudioDevice);
	LN_SAFE_RELEASE(m_audioDevice);

	if (Internal::AudioManager == this) {
		Internal::AudioManager = NULL;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioStream* AudioManagerImpl::CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
{
	// �L���b�V������������B
	// �������� AudioStream �́A�܂��񓯊����������ł��邱�Ƃ�����B
	RefPtr<AudioStream> audioStream((AudioStream*)m_audioStreamCache->FindObjectAddRef(key), false);

	// �L���b�V���Ɍ�����Ȃ�������V�������
	if (audioStream.IsNull())
	{
		audioStream.Attach(LN_NEW AudioStream(this, stream), false);
		audioStream->Create(loadingMode == SoundLoadingMode::ASync);	// �񓯊��ǂݍ��݊J�n
								// TODO: �����ǂݍ��݂����ɂ��āAPolling �X���b�h�œǂݍ���ł��ǂ������H
		/*
			�񓯊��ǂݍ��݂̊J�n�� FileManager �̃^�X�N���X�g�ɓ������B
			�����ŎQ�ƃJ�E���g�� +1 ����A��������������܂ŎQ�Ƃ��ꑱ����B
		*/

		// �L���b�V���ɓo�^
		if (!key.IsNull()) {
			m_audioStreamCache->RegisterCacheObject(key, audioStream);
		}
	}

	audioStream.SafeAddRef();
	return audioStream;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AudioPlayer* AudioManagerImpl::CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D)
{
	// �Đ����@�̑I��
	SoundPlayingMode playerType = AudioUtils::CheckAudioPlayType(mode, stream, mOnMemoryLimitSize);

	// �쐬
	if (playerType == SoundPlayingMode::Midi) {
		return m_midiAudioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
	else {
		return m_audioDevice->CreateAudioPlayer(stream, enable3D, playerType);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sound* AudioManagerImpl::CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode)
{
	RefPtr<AudioStream> audioStream(CreateAudioStream(stream, key, loadingMode), false);
	RefPtr<Sound> sound(LN_NEW Sound(), false);
	sound->Initialize(this, audioStream);

	if (loadingMode == SoundLoadingMode::Sync) {
		sound->CreateAudioPlayerSync();
	}

	// �Ǘ����X�g�ɒǉ�
	Threading::MutexScopedLock lock(m_soundListMutex);
	m_addingSoundList.Add(sound);
	sound.SafeAddRef();	// �Ǘ����X�g�̎Q��
	sound.SafeAddRef();	// �O�ɏo�����߂̎Q��
	return sound;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void AudioManagerImpl::Thread_Polling()
{
#ifdef LN_OS_WIN32
	// COM ������
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif

	uint64_t lastTime = Environment::GetTickCount();
	while (m_endRequested.IsFalse())
	{
		Threading::Thread::Sleep(10);	// CPU ���� 100% ������邽�߁A�Ƃ肠���� 10ms �҂�

		// �ǉ��҂����X�g�̓��e��{���X�g�Ɉڂ�
		{
			Threading::MutexScopedLock lock(m_soundListMutex);
			for (Sound* sound : m_addingSoundList) {
				m_soundList.Add(sound);
			}
			m_addingSoundList.Clear();
		}

		// �o�ߎ��Ԃ����߂đS Sound �X�V
		uint64_t curTime = Environment::GetTickCount();
		float elapsedTime = static_cast<float>(curTime - lastTime) / 1000.0f;
		for (Sound* sound : m_soundList) {
			sound->Polling(elapsedTime);
		}
		lastTime = curTime;

		// �������烍�b�N
		Threading::MutexScopedLock lock(m_soundListMutex);

		m_audioDevice->Update();
        
        if (m_midiAudioDevice != nullptr)
        {
            m_midiAudioDevice->Update();
        }

		// GC�B���̃��X�g���炵���Q�Ƃ���ĂȂ���� Release ����B
		Array<Sound*>::iterator itr = m_soundList.begin();
		Array<Sound*>::iterator end = m_soundList.end();
		while (itr != end)
		{
			// TODO: �t�F�[�h���͊J�����Ȃ�

			if ((*itr)->GetRefCount() == 1)
			{
				(*itr)->Release();
				itr = m_soundList.erase(itr);
				end = m_soundList.end();
			}
			else {
				++itr;
			}
		}

		if (m_gameAudio != NULL) {
			m_gameAudio->Polling();
		}
	}

#ifdef LN_OS_WIN32
	::CoUninitialize();
#endif
}

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
