﻿
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Audio/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN
class GameAudio;
class AudioDevice;
class AudioStream;
class AudioPlayer;
class Sound;
class GameAudioImpl;

/*
	@brief	音声機能の管理クラスです。
*/
class AudioManagerImpl
	: public RefObject
{
public:
	/// initialize() に渡す初期化データ
	struct Settings
	{
		ln::FileManager*		FileManager;					///< ファイルからの読み込みに使うファイル管理クラス
		uint32_t				StreamCacheObjectCount;			///< キャッシュサイズ (ファイル数)
		uint32_t				StreamSourceCacheMemorySize;	///< キャッシュサイズ (メモリ量(byte))
		DirectMusicMode			DMInitMode;						///< DirectMusic の初期化方法
		void*					hWnd;							///< DirectMusic の初期化に使うウィンドウハンドル
		float					DirectMusicReverbLevel;

		Settings()
			: FileManager(NULL)
			, StreamCacheObjectCount(32)
			, StreamSourceCacheMemorySize(0)
			, DMInitMode(DirectMusicMode::NotUse)
			, hWnd(NULL)
			, DirectMusicReverbLevel(0.75f)
		{}
	};

public:
	static AudioManagerImpl* GetInstance(AudioManagerImpl* priority = nullptr);

	static AudioManagerImpl* Create(const Settings& settings);

public:

	/// 終了処理
	void Finalize();

	/// オンメモリorストリーミング自動選択の音声データバイト数閾値
	void SetAutoPlayTypeSelectThreshold(uint32_t threshold) { mOnMemoryLimitSize = threshold; }

	GameAudioImpl* GetGameAudio() const { return m_gameAudio; }

	AudioDevice* GetAudioDevice() const { return m_audioDevice; }

//LN_INTERNAL_ACCESS:
public:	// TODO
	friend class Sound;	// TODO
	FileManager* GetFileManager() { return m_fileManager; }
	//AudioStream* CreateAudioStream(const TCHAR* filePath);
	AudioStream* CreateAudioStream(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);
	AudioPlayer* CreateAudioPlayer(AudioStream* stream, SoundPlayingMode mode, bool enable3D);		// 初期化完了済みの AudioStream を渡すこと
	Sound* CreateSound(Stream* stream, const CacheKey& key, SoundLoadingMode loadingMode);



	/// キーに対応するオーディオソースを検索する (見つかった場合は addRef して返す)
	//AudioStream* FindAudioSource(lnSharingKey key);

	/// オーディオソースの作成 TODO:internal へ
	//AudioStream* CreateAudioStream(const TCHAR* filePath);
	//AudioStream* CreateAudioStream(Stream* stream, const CacheKey& key);

	//AudioPlayer* CreateAudioPlayer(AudioStream* stream, SoundPlayType type, bool enable3D);

	//

	///// Sound の作成 ( stream_ = NULL でキーを使った検索だけ行う )
	//Sound* createSound(FileIO::Stream* stream, SoundPlayType type, bool enable_3d, lnSharingKey key);

	///// Sound の作成 ( ファイル名指定 )
	//Sound* createSound(const lnChar* filename, SoundPlayType type, bool enable_3d);

	///// Sound の作成 ( IAudioSource 指定 )
	//Sound* createSound(AudioSourceBase* source, SoundPlayType type, bool enable_3d);

	/// グループの停止
	//void stopGroup(lnU32 group);

private:
	AudioManagerImpl(const Settings& configData);
	virtual ~AudioManagerImpl();
	void Thread_Polling();

private:

	FileManager*			m_fileManager;
	AudioDevice*			m_audioDevice;		///< PCM 再生用デバイスクラス
	AudioDevice*			m_midiAudioDevice;	///< MIDI 再生用デバイスクラス (内部処理が PCM とは全然違うので、1つの AudioDevice にまとめない方が管理が楽)
	GameAudioImpl*				m_gameAudio;
	uint32_t				mOnMemoryLimitSize;
	Threading::Mutex			m_resourceMutex;

	CacheManager*				m_audioStreamCache;

	Array<Sound*>				m_addingSoundList;
	Array<Sound*>				m_soundList;
	Threading::Mutex			m_soundListMutex;
	Threading::EventFlag		m_endRequested;
	Threading::DelegateThread	m_pollingThread;
};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
